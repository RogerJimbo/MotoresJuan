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
	App->loader->Import("BakerHouse.fbx");  

	return true;
}

void ModuleScene::Draw()
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	for (auto item = mesh.begin(); item != mesh.end(); item++)
	{
		glColor3f(1.0, 1.0, 1.0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, (*item)->id_indices);
		glVertexPointer(3, GL_FLOAT, 0, &(*item)->vertices[0]);
		glBindTexture(GL_TEXTURE_2D, (*item)->texture);
		glTexCoordPointer(2, GL_FLOAT, 0, &(*item)->texture_coords[0]);
		glDrawElements(GL_TRIANGLES, (*item)->num_indices, GL_UNSIGNED_INT, NULL);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);	
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
void ModuleScene::IndexSphere() {}

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

void ModuleScene::ArrayCube(float x, float y, float z, float posx, float posy, float posz)
{
	glLineWidth(2.0f);
	glBegin(GL_TRIANGLES);
	glColor3f(1.0, 1.0, 1.0);

	//Front
	glVertex3f(posx+x, posz, posy);	glVertex3f(posx, posz+z, posy);			glVertex3f(posx, posz, posy);
	glVertex3f(posx+x, posz, posy);	glVertex3f(posx+x, posz + z, posy);		glVertex3f(posx, posz + z, posy);

	//Right
	glVertex3f(posx + x, posz + z, posy);		glVertex3f(posx + x, posz, posy);				glVertex3f(posx + x, posz, -posy - y);
	glVertex3f(posx + x, posz + z, -posy-y);	glVertex3f(posx + x, posz + z, posy);		glVertex3f(posx + x, posz, -posy - y);

	//Left
	glVertex3f(posx, posz + z, -posy - y);	glVertex3f(posx, posz, posy);	glVertex3f(posx, posz + z, posy);
	glVertex3f(posx, posz + z, -posy - y);	glVertex3f(posx, posz, -posy - y);	glVertex3f(posx, posz, posy);

	//Back
	glVertex3f(posx + x, posz + z, -posy - y);	glVertex3f(posx + x, posz, -posy - y);	glVertex3f(posx, posz + z, -posy - y);
	glVertex3f(posx, posz + z, -posy - y);	glVertex3f(posx + x, posz, -posy - y);	glVertex3f(posx, posz, -posy - y);

	//Top
	glVertex3f(posx + x, posz + z, posy);		glVertex3f(posx, posz + z, -posy - y);	glVertex3f(posx, posz + z, posy);
	glVertex3f(posx, posz + z, -posy - y);	glVertex3f(posx + x, posz + z, posy);		glVertex3f(posx + x, posz + z, -posy - y);

	//Bot
	glVertex3f(posx + x, posz, posy);	glVertex3f(posx, posz, posy);	glVertex3f(posx, posz, -posy - y);
	glVertex3f(posx, posz, -posy - y);	glVertex3f(posx + x, posz, -posy - y);	glVertex3f(posx + x, posz, posy);

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

bool ModuleScene::CleanUp() { return true; }
void ModuleScene::Save_Config(JSON_Object& config) const { }



