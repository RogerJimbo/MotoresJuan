#ifndef __ModuleScene_H__
#define __ModuleScene_H__

#include <vector>
#include "Module.h"

#include "SDL\include\SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>
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

	string config_name;
	vector<float> indices;
	vector<vec3> vertexes;
};
#endif
