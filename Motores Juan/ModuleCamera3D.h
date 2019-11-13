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

	update_status Update(float dt);
	bool CleanUp();

	void Look(const vec3 &Position, const vec3 &Reference);
	void LookAt(const vec3 &Spot);

	mat4x4 At(const vec3 &right, const vec3 &up, const vec3 &direction);

	void Move(const vec3 &Movement);
	float* GetViewMatrix();

	void CameraGuizmos();
	void FieldOfView(float angle, float aspectRatio, float min, float max, float* camera);

private:
	void CalculateViewMatrix();

public:
	vec3 X, Y, Z;
	mat4x4 Camera_view;

	vec3 cameraRef;
	vec3 cameraPos;
	vec3 cameraTarget;
	vec3 cameraFront;

	vec3 cameraDirection;
	vec3 cameraRight;
	vec3 cameraUp;
	vec3 newPos;

	bool Clicked = false;
private:
	mat4x4 ViewMatrix, ViewMatrixInverse;
	Timer timer;
};