#include "Application.h"
#include "ModuleScene.h"

//#include "GL/glew.h"
#include "SDL\include\SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>


ModuleScene::ModuleScene(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	//name = "scene";
}


ModuleScene::~ModuleScene()
{
}

update_status ModuleScene::Update(float dt)
{
	return UPDATE_CONTINUE;
}

void ModuleScene::Draw()
{
	
}


