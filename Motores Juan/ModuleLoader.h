#pragma once
#include "Module.h"
#include "Globals.h"
#include "Parson\parson.h"
#include <list>

#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cfileio.h"

class Application;
class scene;
class GameObject;
using namespace std;

class ModuleLoader : public Module
{
public:
	ModuleLoader(Application* app, bool start_enabled = true);
	~ModuleLoader();

	bool Init(const JSON_Object& config);
	bool Start();

	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status PostUpdate(float dt);

	bool CleanUp();
	bool Import(const string& pFile, GameObject* parent);

	GameObject* LoadGameObject(const aiScene* scene, aiNode* node, GameObject* parent, string path_file);

	uint Texturing(const char* file_name);

	string module_name;
	string path;

	vec2 TextureSize;
};