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

bool ModuleScene::Start() { return true; }

update_status ModuleScene::PreUpdate(float dt) { return UPDATE_CONTINUE; }

update_status ModuleScene::Update(float dt) { return UPDATE_CONTINUE; }

update_status ModuleScene::PostUpdate(float dt) { return UPDATE_CONTINUE; }


void ModuleScene::Draw()
{
	DrawGrid(10);
	ArrayCube();
	DrawAxis();
}

void ModuleScene::IndexCube()
{
	GLuint ibo, vbo = 0;		//WTF ARE THOSE?	

	indices = { 0, 1, 2, 0, 3, 2,			//FRONT
					 3, 0, 4, 5, 3, 4,		//RIGHT
					 6, 2, 1, 6, 7, 2,			//LEFT
				     5, 4, 6, 6, 4, 7,		//BACk
					 3, 6, 1, 6, 3, 5,			//TOP
					 0, 2, 7, 7, 4, 0 };		//BOT
	
	vertexes.push_back((1.0f, 0.0f, 0.0f));
	vertexes.push_back((0.0f, 1.0f, 0.0f));
	vertexes.push_back((0.0f, 0.0f, 0.0f));
	vertexes.push_back((1.0f, 1.0f, 0.0f));

	vertexes.push_back((1.0f, 0.0f, -1.0f));
	vertexes.push_back((1.0f, 1.0f, -1.0f));
	vertexes.push_back((0.0f, 1.0f, -1.0f));
	vertexes.push_back((0.0f, 0.0f, -1.0f));

	glGenBuffers(1, (GLuint*) & (ibo));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size(), &indices, GL_STATIC_DRAW);

	glGenBuffers(1, (GLuint*) & (vbo));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, vertexes.size(), &vertexes, GL_STATIC_DRAW);
	
}

void ModuleScene::ArrayCube()
{
	glLineWidth(2.0f);
	glBegin(GL_TRIANGLES);
	glColor3f(1.0, 0.0, 1.0);
	//Front
	glVertex3f(1.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);

	glVertex3f(1.0f, 0.0f, 0.0f);
	glVertex3f(1.0f, 1.0f, 0.0f);
	glVertex3f(0.0f, 1.0f, 0.0f);

	//Right
	glVertex3f(1.0f, 1.0f, 0.0f);
	glVertex3f(1.0f, 0.0f, 0.0f);
	glVertex3f(1.0f, 0.0f, -1.0f);

	glVertex3f(1.0f, 1.0f, -1.0f);
	glVertex3f(1.0f, 1.0f, 0.0f);
	glVertex3f(1.0f, 0.0f, -1.0f);

	//Left
	glVertex3f(0.0f, 1.0f, -1.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 1.0f, 0.0f);

	glVertex3f(0.0f, 1.0f, -1.0f);
	glVertex3f(0.0f, 0.0f, -1.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);

	//Back
	glVertex3f(1.0f, 1.0f, -1.0f);
	glVertex3f(1.0f, 0.0f, -1.0f);
	glVertex3f(0.0f, 1.0f, -1.0f);

	glVertex3f(0.0f, 1.0f, -1.0f);
	glVertex3f(1.0f, 0.0f, -1.0f);
	glVertex3f(0.0f, 0.0f, -1.0f);

	//Top
	glVertex3f(1.0f, 1.0f, 0.0f);
	glVertex3f(0.0f, 1.0f, -1.0f);
	glVertex3f(0.0f, 1.0f, 0.0f);

	glVertex3f(0.0f, 1.0f, -1.0f);
	glVertex3f(1.0f, 1.0f, 0.0f);
	glVertex3f(1.0f, 1.0f, -1.0f);

	//Bot
	glVertex3f(1.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, -1.0f);

	glVertex3f(0.0f, 0.0f, -1.0f);
	glVertex3f(1.0f, 0.0f, -1.0f);
	glVertex3f(1.0f, 0.0f, 0.0f);

	glEnd();
}

void ModuleScene::DrawAxis()
{
	glBegin(GL_LINES);
	// draw line for x axis
	glColor3f(1.0, 0.0, 0.0);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(1.0, 0.0, 0.0);
	// draw line for y axis
	glColor3f(0.0, 1.0, 0.0);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(0.0, 1.0, 0.0);
	// draw line for Z axis
	glColor3f(0.0, 0.0, 1.0);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(0.0, 0.0, 1.0);
	glEnd();
}

void ModuleScene::DrawGrid(int HALF_GRID_SIZE)
{
	glBegin(GL_LINES);
	glColor3f(0.75f, 0.75f, 0.75f);
	for (int i = -HALF_GRID_SIZE; i <= HALF_GRID_SIZE; i++)
	{
		glVertex3f((float)i, 0, (float)-HALF_GRID_SIZE);
		glVertex3f((float)i, 0, (float)HALF_GRID_SIZE);

		glVertex3f((float)-HALF_GRID_SIZE, 0, (float)i);
		glVertex3f((float)HALF_GRID_SIZE, 0, (float)i);
	}
	glEnd();
}

bool ModuleScene::CleanUp()
{
	return true;
}

void ModuleScene::Save_Config(JSON_Object& config) const
{

}


