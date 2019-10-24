#include "Globals.h"
#include "Application.h"
#include "ModuleCamera3D.h"
#include "GLFW/includes/glfw3.h"
#include <math.h>

#pragma comment (lib, "GLFW/libx86/glfw3.lib")

ModuleCamera3D::ModuleCamera3D(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	CalculateViewMatrix();

	X = vec3(1.0f, 0.0f, 0.0f);
	Y = vec3(0.0f, 1.0f, 0.0f);
	Z = vec3(0.0f, 0.0f, 1.0f);

	Position = vec3(0.0f, 0.0f, 5.0f);
	Reference = vec3(0.0f, 0.0f, 0.0f);
	Rotation = vec3(0.0f, 0.0f, 0.0f);

	cameraPos = vec3(0.0f, 0.0f, 3.0f);

	vec3 cameraTarget = vec3(0.0f, 0.0f, 0.0f);
	vec3 up = vec3(0.0f, 1.0f, 0.0f);

	vec3 cameraDirection = normalize(cameraPos - cameraTarget);
	vec3 cameraRight = normalize(cross(up, cameraDirection));
	vec3 cameraUp = cross(cameraDirection, cameraRight);

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
	vec3 newPos = { 0,0,0 };

	float speed = 3.0f * dt; 
	float wheel = 150.0f * dt;

	if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT) speed = 8.0f * dt;
	if (App->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN) { LookAt(newPos); Position = Reference + 5.0f * Z; }

	if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT)
	{
		//	KEYS
		if (App->input->GetKey(SDL_SCANCODE_Q) == KEY_REPEAT) newPos.y += speed;
		if (App->input->GetKey(SDL_SCANCODE_E) == KEY_REPEAT) newPos.y -= speed;
		if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) newPos -= Z * speed;
		if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) newPos += Z * speed;
		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) newPos += X * speed;
		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) newPos -= X * speed;

		float Sensitivity = 0.4f;
		int mouse_x = +App->input->GetMouseXMotion();
		int mouse_y = -App->input->GetMouseYMotion();

		if (mouse_x != 0) 		//Mouse X
		{
			float DeltaX = (float)mouse_x * Sensitivity;
			X = rotate(X, DeltaX, vec3(0.0f, 1.0f, 0.0f));
			Y = rotate(Y, DeltaX, vec3(0.0f, 1.0f, 0.0f));
			Z = rotate(Z, DeltaX, vec3(0.0f, 1.0f, 0.0f));
		}

		if (mouse_y != 0) 		//Mouse Y
		{ 
			float DeltaY = (float)mouse_y * Sensitivity;
			Y = rotate(Y, DeltaY, X);
			Z = rotate(Z, DeltaY, X);
			if (Y.y < 0.0f) { Z = vec3(0.0f, Z.y > 0.0f ? 1.0f : -1.0f, 0.0f);	Y = cross(Z, X); }
		}

		Position = Z * length(Position);

	}

	if (App->input->GetMouseZ() != 0)	//	Wheel
	{
		if(App->input->GetMouseZ() > 0) { newPos -= Z * wheel; }
		else { newPos += Z * wheel; }
	}

	if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_LALT)) {}

	if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT && App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_REPEAT) { Rotate(); }

	Move(newPos);
	Look(Position, Camera_view.translation());

	//CalculateViewMatrix();
	return UPDATE_CONTINUE;
}

void ModuleCamera3D::Rotate() //{ this->Position += X * 0.20f; }	//TODO: It moves faster when closer to reference.
{
	float radius = 10.0f;
	//float camX = sin(glfwGetTime()) * radius; float camZ = cos(glfwGetTime()) * radius;


	//Camera_view = At(vec3(camX, 0.0, camZ), vec3(0.0, 0.0, 0.0), vec3(0.0, 1.0, 0.0));

}

void ModuleCamera3D::Look(const vec3 &Position, const vec3 &Reference)
{
	this->Position = Position;
	this->Reference = Reference;

	Z = normalize(Position - Reference);
	X = normalize(cross(vec3(0.0f, 1.0f, 0.0f), Z));
	Y = cross(Z, X);

	CalculateViewMatrix();
}

void ModuleCamera3D::LookAt( const vec3 &Spot)
{
	Reference = Spot;


	Z = normalize(Position - Reference);
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
	Position += Movement;
	Reference += Movement;

	CalculateViewMatrix();
}

float* ModuleCamera3D::GetViewMatrix()
{
	return &ViewMatrix;
}

void ModuleCamera3D::CalculateViewMatrix()
{
	ViewMatrix = mat4x4(X.x, Y.x, Z.x, 0.0f, X.y, Y.y, Z.y, 0.0f, X.z, Y.z, Z.z, 0.0f, -dot(X, Position), -dot(Y, Position), -dot(Z, Position), 1.0f);
	ViewMatrixInverse = inverse(ViewMatrix);
}