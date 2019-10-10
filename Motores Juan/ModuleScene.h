#ifndef __ModuleScene_H__
#define __ModuleScene_H__

#include <vector>
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

	void DrawGrid(int GridSize);
	void DrawAxis();

	string config_name;
	vector<unsigned int> indices;
	vector<vec3> vertexes;
};
#endif
