#include "Application.h"
#include "ModuleLoader.h"
#include "ModuleScene.h"
#include "ModuleRenderer3D.h"

#include "Glew/include/glew.h"
#include "SDL\include\SDL_opengl.h"

#include <vector>
#include <gl/GL.h>
#include <gl/GLU.h>

#include "Devil/includes/il.h"
#include "Devil/includes/ilu.h"
#include "Devil/includes/ilut.h"

#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cfileio.h"

#pragma comment (lib, "Assimp/libx86/assimp.lib")
#pragma comment (lib, "Devil/libx86/DevIL.lib")
#pragma comment (lib, "Devil/libx86/ILU.lib")
#pragma comment (lib, "Devil/libx86/ILUT.lib")

typedef unsigned int uint;
using namespace Assimp;

ModuleLoader::ModuleLoader(Application* app, bool start_enabled) : Module(app, start_enabled) 
{ 
	module_name = "Loader";
	struct aiLogStream stream;	
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
	aiAttachLogStream(&stream);

	ilInit();
	glEnable(GL_TEXTURE_2D);
}

ModuleLoader::~ModuleLoader() {}

bool ModuleLoader::Init(const JSON_Object& config) { return true; }
bool ModuleLoader::Start() { return true; }

update_status ModuleLoader::PreUpdate(float dt) { return UPDATE_CONTINUE; }
update_status ModuleLoader::Update(float dt) { return UPDATE_CONTINUE; }
update_status ModuleLoader::PostUpdate(float dt) { return UPDATE_CONTINUE; }

bool ModuleLoader::Import(const string& pFile)
{
	const aiScene* scene = aiImportFile(pFile.c_str(), aiProcessPreset_TargetRealtime_MaxQuality);

	if (scene != nullptr && scene->HasMeshes())
	{
		for (int i = 0; i < scene->mNumMeshes; ++i)
		{
			const aiMesh* m = scene->mMeshes[i];
			Mesh* new_mesh = new Mesh;

			new_mesh->num_vertices = m->mNumVertices;
			new_mesh->vertices = new float[new_mesh->num_vertices * 3];
			memcpy(new_mesh->vertices, m->mVertices, sizeof(float) * new_mesh->num_vertices * 3);
			LOG("New mesh with %d vertices", new_mesh->num_vertices);

			if (m->HasFaces())
			{
				new_mesh->num_indices = m->mNumFaces * 3;
				new_mesh->indices = new uint[new_mesh->num_indices]; // assume each face is a triangle
				for (uint i = 0; i < m->mNumFaces; ++i)
				{
					if (m->mFaces[i].mNumIndices != 3) { LOG("WARNING, geometry face with != 3 indices!"); }
					else { memcpy(&new_mesh->indices[i * 3], m->mFaces[i].mIndices, 3 * sizeof(uint)); }
				}				glGenBuffers(1, (GLuint*)&(new_mesh->id_vertices));
				glBindBuffer(GL_ARRAY_BUFFER, new_mesh->id_vertices);
				glBufferData(GL_ARRAY_BUFFER, sizeof(float)*new_mesh->num_vertices, new_mesh->vertices, GL_STATIC_DRAW);

				glGenBuffers(1, (GLuint*)&(new_mesh->id_indices));
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, new_mesh->id_indices);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float)*new_mesh->num_indices, new_mesh->indices, GL_STATIC_DRAW);				App->modscene->mesh.push_back(new_mesh);
				App->modscene->model_loaded = true;
			}
			// Use scene->mNumMeshes to iterate on scene->mMeshes array
			aiReleaseImport(scene);
		}
	}
	else { LOG("Error loading scene %s", pFile); }
	return true;	
}

uint ModuleLoader::Texturing(const char* file_name)
{
	ILuint imageID = 0;
	GLuint textureID = 0;
	ILboolean success;
	ILenum error;

	success = ilLoadImage(file_name);	
	ilGenImages(1, &imageID); 		
	ilBindImage(imageID); 			

	if (success)	
	{
		LOG("Texture created succesfuly!")
		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_2D, textureID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		glTexImage2D(GL_TEXTURE_2D, 0, ilGetInteger(GL_RGBA), ilGetInteger(IL_IMAGE_WIDTH),
			ilGetInteger(IL_IMAGE_HEIGHT), 0, ilGetInteger(GL_RGBA), GL_UNSIGNED_BYTE, ilGetData());

	}
	else //error = ilGetError(); LOG("Error creating texture: %i", error); 
		LOG("Error loading mesh... %i", error = ilGetError()); 

	ilDeleteImages(1, &imageID);

	return textureID;
}


bool ModuleLoader::CleanUp() 
{
	aiDetachAllLogStreams();	 
	return true; 
}
