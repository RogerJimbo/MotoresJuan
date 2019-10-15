#ifndef __ModuleScene_H__
#define __ModuleScene_H__

#include <vector>
#include "ModuleLoader.h"
#include "Module.h"

#define GRIDSIZE 50

using namespace std;

class ModuleScene : public Module
{
public:
	ModuleScene(Application* app, bool start_enabled = true);
	~ModuleScene();

	bool Init(const JSON_Object& config);
	bool Start();

	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status PostUpdate(float dt);

	bool CleanUp();
	void Save_Config(JSON_Object& config) const;

	void Draw();
	void IndexCube();
	void ArrayCube();
	void IndexSphere();
	void ArraySphere();
	void ArrayPlane(float x, float y, float posx, float posy, float posz);

	void DrawGrid(int GridSize);
	void DrawAxis();

	string config_name;
	vector<uint> indices;
	vector<vec3> vertexs;
	vector<Mesh*> mesh;
};
#endif
