#pragma once
#include "Module.h"
#include "Globals.h"
#include "glmath.h"

#define CAMERA_SPEED 10.0f

class ComponentCamera;

class ModuleCamera3D : public Module
{
public:
	ModuleCamera3D(Application* app, bool start_enabled = true);
	~ModuleCamera3D();

	bool Init(const JSON_Object& config);
	update_status Update(float dt);
	bool CleanUp();

	void Look(const vec3 &Position, const vec3 &Reference);
	void LookAt(const vec &Spot);

	mat4x4 At(const vec3 &right, const vec3 &up, const vec3 &direction);

	void Move(const vec &Movement);
	float* GetViewMatrix();

	void CameraGuizmos();
	void FieldOfView(float angle, float aspectRatio, float min, float max, float* camera);

private:
	void CalculateViewMatrix();

public:
	vec X = { 1.0f, 0.0f, 0.0f };
	vec Y = { 0.0f, 1.0f, 0.0f };
	vec Z = { 0.0f, 0.0f, 1.0f };
	mat4x4 Camera_view;

	vec cameraRef;
	vec cameraPos;
	vec3 cameraTarget;
	vec3 cameraFront;

	vec3 cameraDirection;
	vec3 cameraRight;
	vec3 cameraUp;
	vec3 newPos;

	ComponentCamera* camera;

	bool Clicked = false;
private:
	mat4x4 ViewMatrix, ViewMatrixInverse;
	Timer timer;
};