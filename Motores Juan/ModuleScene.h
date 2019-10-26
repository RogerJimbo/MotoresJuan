#ifndef __ModuleScene_H__
#define __ModuleScene_H__

#include <vector>
#include "ModuleLoader.h"
#include "Module.h"
#include "GameObject.h"

#define GRIDSIZE 50

using namespace std;

class ModuleScene : public Module
{
public:
	ModuleScene(Application* app, bool start_enabled = true);
	~ModuleScene();

	bool Init(const JSON_Object& config);
	bool Start();
	bool CleanUp();

	void Save_Config(JSON_Object& config) const;
	void Draw();

	void IndexCube();
	void IndexSphere();

	void ArraySphere();
	void ArrayCube(float x, float y, float z, float posx, float posy, float posz);
	void ArrayPlane(float x, float y, float posx, float posy, float posz);

	uint texture;
	string config_name;
	
	vector<uint> indices;
	vector<vec3> vertexs;
	vector<Mesh*> mesh;

	GameObject* root;
	vector<GameObject*> gameobjects;
	
};
#endif
