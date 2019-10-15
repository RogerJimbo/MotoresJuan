#include "Application.h"
#include "ModuleScene.h"
#include "ModuleRenderer3D.h"
#include "ModuleWindow.h"

#include "Glew/include/glew.h"
#include "SDL\include\SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>

ModuleScene::ModuleScene(Application* app, bool start_enabled) : Module(app, start_enabled) { config_name = "Scene"; }
ModuleScene::~ModuleScene() {}

bool ModuleScene::Init(const JSON_Object& config) { return true; }

bool ModuleScene::Start()
{
	//App->loader->Import("BakerHouse.fbx");  //REQUIREMENT 1

	//App->renderer3D->ChangeMeshTexture("Baker_House_DDS.dds");

	return true;
}

update_status ModuleScene::PreUpdate(float dt) { return UPDATE_CONTINUE; }
update_status ModuleScene::Update(float dt) { return UPDATE_CONTINUE; }
update_status ModuleScene::PostUpdate(float dt) { return UPDATE_CONTINUE; }

void ModuleScene::Draw()
{
	if(active_grid) DrawGrid(GRIDSIZE);
	App->hierarchy->CreatePrimitives();
	if(active_axis) DrawAxis(&active_axis);

	glEnableClientState(GL_VERTEX_ARRAY);

	for (auto item = mesh.begin(); item != mesh.end(); item++)
	{
		glBindBuffer(GL_ARRAY_BUFFER, (*item)->id_vertices);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, (*item)->id_indices);
		glVertexPointer(3, GL_FLOAT, 0, NULL);
		glDrawElements(GL_TRIANGLES, (*item)->num_indices, GL_UNSIGNED_INT, NULL);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
	glDisableClientState(GL_VERTEX_ARRAY);	
}

void ModuleScene::IndexCube()
{
	GLuint ibo, vbo = 0;		

	indices = { 0, 1, 2, 0, 3, 2,			//FRONT
					 3, 0, 4, 5, 3, 4,			//RIGHT
					 6, 2, 1, 6, 7, 2,			//LEFT
				     5, 4, 6, 6, 4, 7,			//BACK
					 3, 6, 1, 6, 3, 5,			//TOP
					 0, 2, 7, 7, 4, 0 };		//BOT
	
	vertexs.push_back((1.0f, 0.0f, 0.0f));
	vertexs.push_back((0.0f, 1.0f, 0.0f));
	vertexs.push_back((0.0f, 0.0f, 0.0f));
	vertexs.push_back((1.0f, 1.0f, 0.0f));

	vertexs.push_back((1.0f, 0.0f, -1.0f));
	vertexs.push_back((1.0f, 1.0f, -1.0f));
	vertexs.push_back((0.0f, 1.0f, -1.0f));
	vertexs.push_back((0.0f, 0.0f, -1.0f));

	glGenBuffers(1, (GLuint*) & (ibo));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size(), &indices, GL_STATIC_DRAW);

	glGenBuffers(1, (GLuint*) & (vbo));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, vertexs.size(), &vertexs, GL_STATIC_DRAW);
}
void ModuleScene::ArrayCube()
{
	glLineWidth(2.0f);
	glBegin(GL_TRIANGLES);
	glColor3f(1.0, 1.0, 1.0);

	//Front
	glVertex3f(1.0f, 0.0f, 0.0f);	glVertex3f(0.0f, 1.0f, 0.0f);	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(1.0f, 0.0f, 0.0f);	glVertex3f(1.0f, 1.0f, 0.0f);		glVertex3f(0.0f, 1.0f, 0.0f);

	//Right
	glVertex3f(1.0f, 1.0f, 0.0f);		glVertex3f(1.0f, 0.0f, 0.0f);	glVertex3f(1.0f, 0.0f, -1.0f);
	glVertex3f(1.0f, 1.0f, -1.0f);	glVertex3f(1.0f, 1.0f, 0.0f);		glVertex3f(1.0f, 0.0f, -1.0f);

	//Left
	glVertex3f(0.0f, 1.0f, -1.0f);	glVertex3f(0.0f, 0.0f, 0.0f);	glVertex3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0.0f, 1.0f, -1.0f);	glVertex3f(0.0f, 0.0f, -1.0f);	glVertex3f(0.0f, 0.0f, 0.0f);

	//Back
	glVertex3f(1.0f, 1.0f, -1.0f);	glVertex3f(1.0f, 0.0f, -1.0f);	glVertex3f(0.0f, 1.0f, -1.0f);
	glVertex3f(0.0f, 1.0f, -1.0f);	glVertex3f(1.0f, 0.0f, -1.0f);	glVertex3f(0.0f, 0.0f, -1.0f);

	//Top
	glVertex3f(1.0f, 1.0f, 0.0f);		glVertex3f(0.0f, 1.0f, -1.0f);	glVertex3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0.0f, 1.0f, -1.0f);	glVertex3f(1.0f, 1.0f, 0.0f);		glVertex3f(1.0f, 1.0f, -1.0f);

	//Bot
	glVertex3f(1.0f, 0.0f, 0.0f);	glVertex3f(0.0f, 0.0f, 0.0f);	glVertex3f(0.0f, 0.0f, -1.0f);
	glVertex3f(0.0f, 0.0f, -1.0f);	glVertex3f(1.0f, 0.0f, -1.0f);	glVertex3f(1.0f, 0.0f, 0.0f);

	glEnd();
}
void ModuleScene::IndexSphere()
{
	//GLuint plane = 0;
	//GLuint k1, k2 = 0;
	//indices = { 0, 1, 2, 0, 3, 2 };




	//vertexs.push_back((1.0f, 0.0f, 0.0f));
	//vertexs.push_back((0.0f, 1.0f, 0.0f));
	//vertexs.push_back((0.0f, 0.0f, 0.0f));
	//vertexs.push_back((1.0f, 1.0f, 0.0f));


	//glGenBuffers(1, (GLuint*) & (plane));
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, plane);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size(), &indices, GL_STATIC_DRAW);






}
void ModuleScene::ArraySphere()
{
	glLineWidth(2.0f);	
	glBegin(GL_POLYGON_BIT);
	glColor3f(1.0, 1.0, 1.0);

	float radius = 1.5f;
	float x, y, z, xy;

	int stackCount = 32;
	int sectorCount = 36;
	#define PI 3.1415

	float sectorStep = 2 * PI / sectorCount;
	float stackStep = PI / stackCount;
	float sectorAngle, stackAngle;
	
	for (int i = 0; i <= stackCount; ++i)
	{
		stackAngle = PI / 2 - i * stackStep;        
		xy = radius * cosf(stackAngle);             
		z = radius * sinf(stackAngle);              

		for (int j = 0; j <= sectorCount; ++j)
		{
			sectorAngle = j+0.5f * sectorStep;           

		
			x = xy * cosf(sectorAngle);             
			y = xy * sinf(sectorAngle);       

			glVertex3f(x, y, z);			

		}
	}
	glEnd();
}

void ModuleScene::ArrayPlane(float x, float y, float posx, float posy, float posz)
{
	glBegin(GL_TRIANGLES);
	glColor3f(1.0, 1.0, 1.0);

	glEnable(GL_CULL_FACE); glCullFace(GL_FRONT); glFrontFace(GL_CCW);	//TODO

	glVertex3f(x+ posx, posz, posy); glVertex3f(posx, posz, y+ posy); glVertex3f(posx, posz, posy);
	glVertex3f(posx, posz, y+ posy); glVertex3f(x+ posx, posz, posy); glVertex3f(x+ posx, posz, y+ posy);

	glEnd();
}

void ModuleScene::DrawAxis(bool active)
{
	glLineWidth(2.0f);
	glBegin(GL_LINES);

	glColor3f(1.0, 0.0, 0.0);	glVertex3f(0.0, 0.0, 0.0);		glVertex3f(1.0, 0.0, 0.0);		// X
	glColor3f(0.0, 1.0, 0.0);	glVertex3f(0.0, 0.0, 0.0);		glVertex3f(0.0, 1.0, 0.0);		// Y
	glColor3f(0.0, 0.0, 1.0);	glVertex3f(0.0, 0.0, 0.0);		glVertex3f(0.0, 0.0, 1.0);		// Z

	glEnd();
}

void ModuleScene::DrawGrid(int GridSize)
{
	glBegin(GL_LINES);
	glColor3f(0.75f, 0.75f, 0.75f);

	for (int i = -GridSize; i <= GridSize; i++)
	{
		glVertex3f((float)i, 0, (float)-GridSize);
		glVertex3f((float)i, 0, (float)GridSize);
		glVertex3f((float)-GridSize, 0, (float)i);
		glVertex3f((float)GridSize, 0, (float)i);
	}
	glEnd();
}

bool ModuleScene::CleanUp() { return true; }

void ModuleScene::Save_Config(JSON_Object& config) const { }



