#include "Globals.h"
#include "Application.h"
#include "ModuleCamera3D.h"
#include <math.h>

#include "ImGuizmos/ImGuizmo.h"
#include "MathGeoLib/Geometry/LineSegment.h"


ModuleCamera3D::ModuleCamera3D(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	CalculateViewMatrix();

	X = vec3(1.0f, 0.0f, 0.0f);
	Y = vec3(0.0f, 1.0f, 0.0f);
	Z = vec3(0.0f, 0.0f, 1.0f);

	newPos = vec3(0.0f, 0.0f, 0.0f);
	cameraRef = vec3(0.0f, 0.0f, 0.0f);
	cameraPos = vec3(0.0f, 4.0f, 10.0f);
	cameraTarget = vec3(0.0f, 0.0f, 0.0f);
	cameraFront = vec3(0.0f, 0.0f, 1.0f);

	cameraDirection = normalize(cameraPos - cameraTarget);
	cameraRight = normalize(cross(Y, cameraDirection));
	cameraUp = cross(cameraDirection, cameraRight);

	Camera_view = At(cameraRight, cameraUp, cameraDirection);
}

ModuleCamera3D::~ModuleCamera3D() {}

bool ModuleCamera3D::CleanUp()
{
	LOG("Cleaning camera");
	return true;
}

update_status ModuleCamera3D::Update(float dt)
{
	SDL_Event event;

	float Sensitivity = 0.4f;
	float speed = 9.0f * dt; 
	float wheel = 100.0f * dt;

	int mouse_x = +App->input->GetMouseXMotion();
	int mouse_y = -App->input->GetMouseYMotion();

	float DeltaX = (float)mouse_x * Sensitivity;
	float DeltaY = (float)mouse_y * Sensitivity;;

	if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT) { speed /= 16.0f * dt; }
	if (App->input->GetMouseZ() != 0) { if (App->input->GetMouseZ() > 0) { cameraPos -= Z * wheel; } else cameraPos += Z * wheel; }

	if (App->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN) 
	{
		cameraPos.y = 6.0f;	cameraPos.z = 5.0f;
		Look(cameraPos, Camera_view.translation());
	}

	if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT)	//Normal movement 
	{
		if (mouse_x != 0) { X = rotate(X, DeltaX, vec3(0.0f, 1.0f, 0.0f)); Y = rotate(Y, DeltaX, vec3(0.0f, 1.0f, 0.0f)); Z = rotate(Z, DeltaX, vec3(0.0f, 1.0f, 0.0f)); }		//Mouse X Right
		if (mouse_y != 0) { Y = rotate(Y, DeltaY, X); Z = rotate(Z, DeltaY, X); if (Y.y < 0.0f) { Z = vec3(0.0f, Z.y > 0.0f ? 1.0f : -1.0f, 0.0f);	Y = cross(Z, X); } }				//Mouse Y Left

		if (App->input->GetKey(SDL_SCANCODE_Q) == KEY_REPEAT) cameraPos.y += speed;
		if (App->input->GetKey(SDL_SCANCODE_E) == KEY_REPEAT) cameraPos.y -= speed;
		if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) cameraPos -= speed * rotate(Z, DeltaX, vec3(0.0f, 0.0f, 1.0f));
		if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)  cameraPos += speed * rotate(Z, DeltaX, vec3(0.0f, 0.0f, 1.0f));
		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)  cameraPos += speed * rotate(X, DeltaX, vec3(1.0f, 0.0f, 0.0f));
		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)  cameraPos -= speed * rotate(X, DeltaX, vec3(1.0f, 0.0f, 0.0f));
	}

	if (App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT && App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_REPEAT)	//Orbit movement
	{
		if (mouse_x != 0) { X = rotate(X, DeltaX, vec3(0.0f, 1.0f, 0.0f)); Y = rotate(Y, DeltaX, vec3(0.0f, 1.0f, 0.0f)); Z = rotate(Z, DeltaX, vec3(0.0f, 1.0f, 0.0f)); }		//Mouse X Right
		if (mouse_y != 0) { Y = rotate(Y, DeltaY, X); Z = rotate(Z, DeltaY, X); if (Y.y < 0.0f) { Z = vec3(0.0f, Z.y > 0.0f ? 1.0f : -1.0f, 0.0f);	Y = cross(Z, X); } }				//Mouse Y Left
		cameraPos = Z * length(cameraPos);
	}

	if (App->input->GetMouseButton(SDL_BUTTON_MIDDLE) == KEY_REPEAT)	//Move camera
	{
		float mspeed = 9.0f * dt;

		if (mouse_x < 0) cameraPos -= mspeed * rotate(X, DeltaX, vec3(1.0f, 0.0f, 0.0f));
		if (mouse_x > 0)   cameraPos += mspeed * rotate(X, DeltaX, vec3(1.0f, 0.0f, 0.0f));
		if (mouse_y < 0)   cameraPos += mspeed * rotate(Y, DeltaX, vec3(1.0f, 0.0f, 0.0f));
		if (mouse_y > 0)   cameraPos -= mspeed * rotate(Y, DeltaX, vec3(1.0f, 0.0f, 0.0f));
	}

	if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN && App->input->GetKey(SDL_SCANCODE_LALT) != KEY_REPEAT)
	{
		if (ImGuizmo::IsOver() == false) { App->modscene->SelectGameObject(MousePicking()); }
	}

	Move(newPos);
	Camera_view = At(cameraPos, cameraPos + cameraFront, cameraUp);

	return UPDATE_CONTINUE;
}

GameObject* ModuleCamera3D::MousePicking()
{
	GameObject* ret = nullptr;

	//1st Calculate Mouse Position
	float mouseX = App->input->GetMouseX();
	float mouseY = App->input->GetMouseY();
	//2nd Convert to OpenGL Coordinates Normalized
	float windowX = App->window->window_width;
	float windowY = App->window->window_height;
	vec2 MouseCoords = vec2((mouseX * 2.0f / windowX -1), -(mouseY * 2.0f/ windowY -1));
	//3rd Convert to Clip Coords 
	vec4 ClipCoords = vec4(MouseCoords.x, MouseCoords.y, -1.0f, 1.0f);
	//4th Convert to Eye Space 
	mat4x4 InvertedProjection = inverse(App->renderer3D->ProjectionMatrix);
	vec4 eyeCoords = InvertedProjection * ClipCoords;
	vec4 EyeSpace = vec4(eyeCoords.x, eyeCoords.y, -1.0f, 0.0f);
	//5th Convert to World Space
	vec4 rayWorld = inverse(ViewMatrix) * eyeCoords;
	vec3 MouseRay = normalize(vec3(rayWorld.x, rayWorld.y, rayWorld.z));

	LOG("%d %d %d", MouseRay.x, MouseRay.y, MouseRay.z);
	
	//llista de GO que han colisionat amb el ray
	//ordenar x distancia llista de GO

		//comprobar per ordre i mirar triangles
		//guardar el que esta mes aprop i passar als eguent semore mirant els triangles
		//si el 2n GO la AABB esta tocant amb la del primer mirar si es sobreposant
		//si el ray no toca cap triangle del mesh es descarta i comprobves el seguent triangle




	return ret;
}

void ModuleCamera3D::Look(const vec3 &Position, const vec3 &Reference)
{
	this->cameraPos = Position;
	this->cameraRef = Reference;

	Z = normalize(Position - Reference);
	X = normalize(cross(vec3(0.0f, 1.0f, 0.0f), Z));
	Y = cross(Z, X);

	CalculateViewMatrix();
}

void ModuleCamera3D::LookAt( const vec3 &Spot)
{
	cameraRef = Spot;

	Z = normalize(cameraPos - cameraRef);
	X = normalize(cross(vec3(0.0f, 1.0f, 0.0f), Z));
	Y = cross(Z, X);

	CalculateViewMatrix();
}

mat4x4 ModuleCamera3D::At(const vec3 &right, const vec3 &up, const vec3 &dir)
{
	mat4x4 matrix_1 = { right.x, right.y, right.z, 0, up.x, up.y, up.z, 0, dir.x, dir.y, dir.z, 0, 0, 0, 0, 1 };
	mat4x4 matrix_2 = { X.x, X.y, X.z, -cameraPos.x, Y.x, Y.y, Y.z, -cameraPos.y, Z.x, Z.y, Z.z, -cameraPos.z, 0, 0, 0, 1 };

	return matrix_1*matrix_2;
}

void ModuleCamera3D::Move(const vec3 &Movement)
{
	cameraPos += Movement;
	cameraRef += Movement;

	CalculateViewMatrix();
}

float* ModuleCamera3D::GetViewMatrix()
{
	return &ViewMatrix;
}

void ModuleCamera3D::CalculateViewMatrix()
{
	ViewMatrix = mat4x4(X.x, Y.x, Z.x, 0.0f, X.y, Y.y, Z.y, 0.0f, X.z, Y.z, Z.z, 0.0f, -dot(X, cameraPos), -dot(Y, cameraPos), -dot(Z, cameraPos), 1.0f);
	ViewMatrixInverse = inverse(ViewMatrix);
}

void ModuleCamera3D::CameraGuizmos()
{

}

void ModuleCamera3D::FieldOfView(float angle, float aspectRatio, float near_plane, float far_plane, float* camera)
{
	//Use this to handle FOV
	//angle has to be GUI_Condig->FOV

	
	

}
