#pragma once
#include "Module.h"
#include "Globals.h"
#include "glmath.h"

#define CAMERA_SPEED 10.0f

class ModuleCamera3D : public Module
{
public:
	ModuleCamera3D(Application* app, bool start_enabled = true);
	~ModuleCamera3D();

	bool Init(JSON_Object* config);
	update_status Update(float dt);
	bool CleanUp();

	void Look(const vec3 &Position, const vec3 &Reference);
	void Rotate();
	void LookAt(const vec3 &Spot);

	mat4x4 At(const vec3 &right, const vec3 &up, const vec3 &direction);

	void Move(const vec3 &Movement);
	float* GetViewMatrix();

private:
	void CalculateViewMatrix();

public:
	vec3 X, Y, Z, Position, Reference, Rotation;
	mat4x4 Camera_view;
	vec3 cameraPos;

private:
	mat4x4 ViewMatrix, ViewMatrixInverse;

};