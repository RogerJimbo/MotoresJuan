#include "Globals.h"
#include "Application.h"
#include "ModuleRenderer3D.h"
#include "Glew/include/glew.h"
#include "SDL\include\SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>
#include "ImGui/imgui_impl_opengl2.h"

#pragma comment (lib, "glu32.lib")    /* link OpenGL Utility lib     */
#pragma comment (lib, "opengl32.lib") /* link Microsoft OpenGL lib   */
#pragma comment (lib, "Glew/libx86/glew32.lib")

ModuleRenderer3D::ModuleRenderer3D(Application* app, bool start_enabled) : Module(app, start_enabled) { module_name = "Renderer3D"; }

ModuleRenderer3D::~ModuleRenderer3D() {}

bool ModuleRenderer3D::Init(const JSON_Object& config)
{
	LOG("Creating 3D Renderer context");
	bool ret = true;
	
	context = SDL_GL_CreateContext(App->window->window);
	if (context == NULL) { LOG("OpenGL context could not be created! SDL_Error: %s\n", SDL_GetError()); ret = false; }
	
	if(ret == true)
	{
		if(VSYNC && SDL_GL_SetSwapInterval(1) < 0) { LOG("Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError()); }
			
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		GLenum error = glGetError();
		if (error != GL_NO_ERROR) { LOG("Error initializing OpenGL! %s\n", gluErrorString(error)); ret = false; }

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		error = glGetError();
		if (error != GL_NO_ERROR) { LOG("Error initializing OpenGL! %s\n", gluErrorString(error)); ret = false; }
		
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
		glClearDepth(1.0f);
		glClearColor(0.f, 0.f, 0.f, 1.f);

		error = glGetError();
		if (error != GL_NO_ERROR) { LOG("Error initializing OpenGL! %s\n", gluErrorString(error));	ret = false; }
		
		GLfloat LightModelAmbient[] = {0.0f, 0.0f, 0.0f, 1.0f};
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, LightModelAmbient);
		
		lights[0].ref = GL_LIGHT0;
		lights[0].ambient.Set(0.25f, 0.25f, 0.25f, 1.0f);
		lights[0].diffuse.Set(0.75f, 0.75f, 0.75f, 1.0f);
		lights[0].SetPos(0.0f, 0.0f, 2.5f);
		lights[0].Init();
		
		GLfloat MaterialAmbient[] = {1.0f, 1.0f, 1.0f, 1.0f};
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, MaterialAmbient);

		GLfloat MaterialDiffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MaterialDiffuse);
		
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		lights[0].Active(true);
		glEnable(GL_LIGHTING);
		glEnable(GL_COLOR_MATERIAL);
	}

	glewInit();
	GLenum err = glewInit();

	OnResize(SCREEN_WIDTH, SCREEN_HEIGHT);

	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	glGenTextures(1, &buffer_tex);
	glBindTexture(GL_TEXTURE_2D, buffer_tex);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCREEN_WIDTH, SCREEN_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, buffer_tex, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return ret;
}

update_status ModuleRenderer3D::PreUpdate(float dt)
{
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glBindTexture(GL_TEXTURE_2D, buffer_tex);
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(App->camera->GetViewMatrix());

	lights[0].SetPos(App->camera->Position.x, App->camera->Position.y, App->camera->Position.z);

	for(uint i = 0; i < MAX_LIGHTS; ++i) lights[i].Render();
		
	return UPDATE_CONTINUE;
}

// PostUpdate present buffer to screen
update_status ModuleRenderer3D::PostUpdate(float dt)
{
	int result = 0;
	glGetIntegerv(GL_FRAMEBUFFER_BINDING, &result);

	App->modscene->Draw();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	ImGui::Render();

	glGetIntegerv(GL_FRAMEBUFFER_BINDING, &result);

	ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());

	SDL_GL_SwapWindow(App->window->window);
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleRenderer3D::CleanUp()
{
	LOG("Destroying 3D Renderer");
	glDeleteFramebuffers(1, &fbo);
	SDL_GL_DeleteContext(context);
	return true;
}


void ModuleRenderer3D::OnResize(int width, int height)
{
	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	ProjectionMatrix = perspective(60.0f, (float)width / (float)height, 0.125f, 512.0f);
	glLoadMatrixf(&ProjectionMatrix);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void ModuleRenderer3D::ChangeMeshTexture(const char* path)
{
	uint width, height = 1024;		//TODO
	GLuint tex_id = App->loader->Texturing(path, width, height);

	for (vector<Mesh*>::iterator iter = App->modscene->mesh.begin(); iter != App->modscene->mesh.end(); iter++)
	{
		(*iter)->texture = tex_id;
		(*iter)->texture_width = width;
		(*iter)->texture_height = height;
	}
}
