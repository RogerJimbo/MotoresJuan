#include "ModuleLoader.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/Importer.hpp"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cimport.h"
#include "Assimp/include/cfileio.h"

#pragma comment (lib, "Assimp/libx86/assimp.lib")

using namespace Assimp;

ModuleLoader::ModuleLoader(Application* app, bool start_enabled) : Module(app, start_enabled) { module_name = "loader"; }
ModuleLoader::~ModuleLoader() {}

bool ModuleLoader::Init(const JSON_Object& config) { return true; }
bool ModuleLoader::Start() { return true; }
update_status ModuleLoader::PreUpdate(float dt) { return UPDATE_CONTINUE; }
update_status ModuleLoader::Update(float dt) { return UPDATE_CONTINUE; }
update_status ModuleLoader::PostUpdate(float dt) { return UPDATE_CONTINUE; }
bool ModuleLoader::CleanUp() { aiDetachAllLogStreams();	 return true; }

bool ModuleLoader::Import(const string& pFile)
{
	Importer importer;

	//aiProcess_CalcTangentSpace |aiProcess_Triangulate |aiProcess_JoinIdenticalVertices | aiProcess_SortByPType

	const aiScene* scene = importer.ReadFile(pFile, aiProcessPreset_TargetRealtime_MaxQuality);	

	if (scene && scene->HasMeshes()) { aiReleaseImport(scene); }

	if (!scene) LOG("%s", importer.GetErrorString()); LOG("Error loading scene %s", pFile); return  false;

	//Turn on the Debugger
	struct aiLogStream stream;
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
	aiAttachLogStream(&stream);

	//Create a struct to contain vertex data

	uint id_index = 0;	uint num_index = 0; uint* index = nullptr;		//Index in VRAM
	uint id_vertex = 0; uint num_vertex = 0; uint* vertex = nullptr;	//Unique Vertex In VRAM

	
	 

	return true;
}

