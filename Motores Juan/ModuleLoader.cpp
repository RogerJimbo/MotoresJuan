#include "ModuleLoader.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/Importer.hpp"
#include "Assimp/include/postprocess.h"

using namespace Assimp;

ModuleLoader::ModuleLoader(Application* app, bool start_enabled = true) : Module(app, start_enabled) { module_name = "loader"; }
ModuleLoader::~ModuleLoader() {}

bool ModuleLoader::Init(const JSON_Object& config) { return true; }
bool ModuleLoader::Start() { return true; }
update_status ModuleLoader::PreUpdate(float dt) { return UPDATE_CONTINUE; }
update_status ModuleLoader::Update(float dt) { return UPDATE_CONTINUE; }
update_status ModuleLoader::PostUpdate(float dt) { return UPDATE_CONTINUE; }
bool ModuleLoader::CleanUp() { return true; }

bool ModuleLoader::Import()
{

}

