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
	iluInit();
	ilutInit();
	ilutRenderer(ILUT_OPENGL);
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
			const aiMesh* mesh = scene->mMeshes[i];
			Mesh* new_mesh = new Mesh;

			new_mesh->num_vertices = mesh->mNumVertices;
			new_mesh->vertices = new float[new_mesh->num_vertices * 3];
			memcpy(new_mesh->vertices, mesh->mVertices, sizeof(float) * new_mesh->num_vertices * 3);
			LOG("New mesh with %d vertices", new_mesh->num_vertices);

			if (mesh->HasTextureCoords(0))
			{
				new_mesh->texture_coords = new float[mesh->mNumVertices * 2];

				for (uint j = 0; j < mesh->mNumVertices; j += 2)
				{
					new_mesh->texture_coords[j] = mesh->mTextureCoords[0][j / 2].x;
					new_mesh->texture_coords[j + 1] = mesh->mTextureCoords[0][j / 2].y;
				}
				new_mesh->texture = Texturing(pFile.c_str(), new_mesh->texture_width, new_mesh->texture_height);
			}

			if (mesh->HasFaces())
			{
				new_mesh->num_indices = mesh->mNumFaces * 3;
				new_mesh->indices = new uint[new_mesh->num_indices]; // assume each face is a triangle
				for (uint i = 0; i < mesh->mNumFaces; ++i)
				{
					if (mesh->mFaces[i].mNumIndices != 3) { LOG("WARNING, geometry face with != 3 indices!"); }
					else { memcpy(&new_mesh->indices[i * 3], mesh->mFaces[i].mIndices, 3 * sizeof(uint)); }
				}								glGenBuffers(1, (GLuint*)&(new_mesh->id_vertices));
				glBindBuffer(GL_ARRAY_BUFFER, new_mesh->id_vertices);
				glBufferData(GL_ARRAY_BUFFER, sizeof(float)*new_mesh->num_vertices * 3, new_mesh->vertices, GL_STATIC_DRAW);

				glGenBuffers(1, (GLuint*)&(new_mesh->id_indices));
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, new_mesh->id_indices);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float)*new_mesh->num_indices, new_mesh->indices, GL_STATIC_DRAW);				App->modscene->mesh.push_back(new_mesh);
			}

		 // Use scene->mNumMeshes to iterate on scene->mMeshes array
		
		}
	}
	else { LOG("Error loading scene %s", pFile); }

	aiReleaseImport(scene);

	return true;	
}

uint ModuleLoader::Texturing(const char* file_name, uint& texture_width, uint& texture_height)
{
	ILuint imageID = 0;
	GLuint textureID = 0;
	ILenum error;
	ILinfo ImageInfo;

	iluGetImageInfo(&ImageInfo);
	ilGenImages(1, &imageID); 		
	ilBindImage(imageID); 			

	if (ilLoadImage(file_name))
	{
		LOG("Texture loaded succesfuly!")
		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_2D, textureID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		texture_width = ImageInfo.Width; texture_height = ImageInfo.Height;
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture_width, texture_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, ilGetData());

	}
	else 	while (error = ilGetError()) { LOG("Error %d: %s", error, iluErrorString(error)); }

	ilDeleteImages(1, &imageID);
	return textureID;
}


bool ModuleLoader::CleanUp() 
{
	aiDetachAllLogStreams();	 
	return true; 
}
