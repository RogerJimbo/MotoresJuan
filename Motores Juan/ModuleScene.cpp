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
	//App->loader->Import("warrior.fbx");  //Loaded from MENU
	//App->renderer3D->ChangeMeshTexture("Baker_House_DDS.dds");

	return true;
}

update_status ModuleScene::PreUpdate(float dt) { return UPDATE_CONTINUE; }
update_status ModuleScene::Update(float dt) { return UPDATE_CONTINUE; }
update_status ModuleScene::PostUpdate(float dt) { return UPDATE_CONTINUE; }

void ModuleScene::Draw()
{
	DrawGrid(GRIDSIZE);
	ArrayCube();
	DrawAxis();

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	for (auto item = mesh.begin(); item != mesh.end(); item++)
	{
		glBindBuffer(GL_ARRAY_BUFFER, (*item)->id_vertices);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, (*item)->id_indices);

		glTexCoordPointer(2,GL_FLOAT, 0, (*item)->texture_coords);
		glBindTexture(GL_TEXTURE_2D, (*item)->texture);

		glVertexPointer(3, GL_FLOAT, 0, NULL);
		glDrawElements(GL_TRIANGLES, (*item)->num_indices, GL_UNSIGNED_INT, NULL);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

void ModuleScene::IndexCube()
{
	GLuint ibo, vbo = 0;		//WTF ARE THOSE?	

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

void ModuleScene::DrawAxis()
{
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

bool ModuleScene::CleanUp()
{
	return true;
}

void ModuleScene::Save_Config(JSON_Object& config) const
{

}


