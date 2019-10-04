#ifndef __ModuleScene_H__
#define __ModuleScene_H__

#include <vector>
#include "Module.h"

class ModuleScene : public Module
{
public:
	ModuleScene(Application* app, bool start_enabled = true);
	~ModuleScene();

	update_status Update(float dt);
	std::vector<float> vert_array;

	void Draw();

};
#endif
