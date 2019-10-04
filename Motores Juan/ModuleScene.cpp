#include "Application.h"
#include "ModuleScene.h"

//#include "GL/glew.h"

#include "SDL\include\SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>


ModuleScene::ModuleScene(Application* app, bool start_enabled) : Module(app, start_enabled) { config_name = "Scene"; }
ModuleScene::~ModuleScene() {}

bool ModuleScene::Init(const JSON_Object& config)
{

}

bool ModuleScene::Start()
{

}

update_status ModuleScene::PreUpdate(float dt)
{

}

update_status ModuleScene::Update(float dt)
{

}

update_status ModuleScene::PostUpdate(float dt)
{

}

void ModuleScene::Draw()
{
	
}

bool ModuleScene::CleanUp()
{

}

void ModuleScene::Save_Config(JSON_Object& config) const
{

}


