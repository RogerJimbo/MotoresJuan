#pragma once
#include "Module.h"
#include "Globals.h"
#include "glmath.h"
#include "Light.h"
#include "ModuleLoader.h"

#include "MathGeoLib/MathGeoLib.h"
#include "MathGeoLib/Geometry/AABB.h"

#include <vector>

#define MAX_LIGHTS 8

class ModuleRenderer3D : public Module
{
public:
	ModuleRenderer3D(Application* app, bool start_enabled = true);
	~ModuleRenderer3D();

	bool Init(const JSON_Object& config);
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status PostUpdate(float dt);

	bool CleanUp();
	void OnResize(int width, int height);
	void ActivateWireframe();
	void DrawBoundingBox(const AABB& BB);

public:

	Light lights[MAX_LIGHTS];
	SDL_GLContext context;
	mat3x3 NormalMatrix;
	mat4x4 ModelMatrix, ViewMatrix, ProjectionMatrix;

	uint fbo = 0;
	uint rbo;
	uint buffer_tex = 0;

	bool fieldofview = false;


	bool vsync = false;
	bool lightning = true;
	bool backface = true;
	bool lights_on = false;
	bool wireframe = false;
	bool textured = true;
	bool boundingbox = false;

};