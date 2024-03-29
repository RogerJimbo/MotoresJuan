#include "Application.h"
#include "ModuleLoader.h"
#include "ModuleScene.h"
#include "ModuleRenderer3D.h"

#include "GameObject.h"
#include "Component.h"
#include "ComponentMesh.h"
#include "ComponentTransform.h"

#include "Glew/include/glew.h"
#include "SDL\include\SDL_opengl.h"

#include "MathGeoLib/MathGeoLib.h"

#include "ModuleOwnFileSystem.h"

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

void ModuleLoader::ReadFile(string path)
{
	//ModuleFileSystem* filesystem;
	//string fextension;
	//string fname;

	//ModuleFileSystem::SplitFilePath(path.c_str(), NULL, &fname, &fextension);

	//filesystem->SplitFilePath(path.c_str(), NULL, &fname, &fextension);

	//if (fextension == "fbx" || fextension == "FBX") { ImportModel(path, fname); }
	//if (fextension == "png" || fextension == "PNG" || fextension == "dds" || fextension == "DDS") {/*IMPORT TEXTURE*/ }
}

bool ModuleLoader::ImportModel(string path, string fname)
{
	/*const aiScene* scene = aiImportFile(path.c_str(), aiProcessPreset_TargetRealtime_MaxQuality);

	if (scene && scene->HasMeshes())
	{
		GameObject* gameobject = App->modscene->root->AddChildren(fname);
		LoadNodes(scene->mRootNode, scene, gameobject, path);
	}
	else LOG("Error loading scene.")

	aiReleaseImport(scene);*/

	return true;
}

bool ModuleLoader::LoadNodes(aiNode* node, const aiScene* scene, GameObject* parent, std::string& path)
{
	/*GameObject* gameobject;
	gameobject = parent->AddChildren(node->mName.C_Str());

	if(node->mNumMeshes > 0)
	{
		for (int iter = 0; iter < node->mNumMeshes; ++iter)
		{
			LoadGameObject(scene->mMeshes[node->mMeshes[iter]], scene, node, App->modscene->root, path);
		}
	}

	if (node->mTransformation.IsIdentity() == false)
	{
		LoadTransformations(node, gameobject);
	}

	for (int iter = 0; iter < node->mNumChildren; ++iter)
	{
		LoadNodes(node->mChildren[iter], scene, gameobject, path);
	}*/

	return true;
}

bool ModuleLoader::Import(const string& pFile, GameObject* parent)	//TODO
{
	const aiScene* scene = aiImportFile(pFile.c_str(), aiProcessPreset_TargetRealtime_MaxQuality);

	if (scene != nullptr && scene->HasMeshes())
	{
		aiNode* node = scene->mRootNode;
		node->mName = pFile;
		loadNodes(scene, node, App->modscene->root, pFile);
		aiReleaseImport(scene); 
	}

	else { LOG("Error loading scene %s", pFile.c_str()); }

	return true;
}

void ModuleLoader::LoadTransformations(aiNode* node, GameObject* gameobject)
{
	/*aiVector3D position;
	aiVector3D size;
	aiQuaternion rotation;

	node->mTransformation.Decompose(size, rotation, position);

	float3 Position = { position.x, position.y, position.z };
	float3 Scalation = { size.x, size.y, size.z };
	Quat Rotation = { rotation.x, rotation.y, rotation.z, rotation.w };

	ComponentTransform* transform = (ComponentTransform*)gameobject->AddComponent(TRANSFORM);

	transform->setPosition(Position);
	transform->setQuaternion(Rotation);
	transform->setScale(Scalation);*/
}

bool ModuleLoader::loadNodes(const aiScene* scene, aiNode* node, GameObject* parent, string path_file)
{
	aiVector3D position;
	aiVector3D size;
	aiQuaternion rotation;

	node->mTransformation.Decompose(size, rotation, position);

	float3 Position = { position.x, position.y, position.z };
	float3 Scale = { size.x, size.y, size.z };
	Quat Rotation = { rotation.x, rotation.y, rotation.z, rotation.w };

	GameObject* GO = new GameObject();
	GO->name = path_file;

	if (parent != nullptr)
	{
		GO->parent = parent;
		parent->children.push_back(GO);
	}

	if (node->mNumMeshes > 0)
	{
		for (int i = 0; i < node->mNumMeshes; ++i)
		{
			GO = LoadGameObject(scene, scene->mMeshes[node->mMeshes[i]], parent, path_file);
		}
	}

	for (uint i = 0; i < node->mNumChildren; i++)
	{
		if (GO)
		{
			loadNodes(scene, node->mChildren[i], GO, node->mName.C_Str());
		}
		else
		{
			loadNodes(scene, node->mChildren[i], parent, node->mName.C_Str());
		}
	}
	
	return true;
}

GameObject* ModuleLoader::LoadGameObject(const aiScene* scene, aiMesh* node, GameObject* parent, string path_file)
{
	
		/*ComponentTransform* transform = (ComponentTransform*)parent->AddComponent(TRANSFORM);

		transform->setPosition(Position);
		transform->setQuaternion(Rotation);
		transform->setScale(Scale);
		transform->local_matrix.Set(float4x4::FromTRS(Position, Rotation, Scale));*/

		/*GO->name = node->mName.C_Str();
		GO->parent = parent;

		if (parent != nullptr) 
		{ 
			parent->children.push_back(GO); 
		}*/

		
	const aiMesh* mesh = node;
	GameObject* newGO = new GameObject();
	newGO->name = mesh->mName.C_Str();
	newGO = parent->AddChildren(node->mName.C_Str());

	/*ComponentTransform* transform = (ComponentTransform*)newGO->AddComponent(TRANSFORM);

	transform->setPosition(Position);
	transform->setQuaternion(Rotation);
	transform->setScale(Scale);
	transform->local_matrix.Set(float4x4::FromTRS(Position, Rotation, Scale));*/

	ComponentMesh* new_mesh = (ComponentMesh*)newGO->AddComponent(MESH);

	new_mesh->num_vertices = mesh->mNumVertices;
	new_mesh->vertices = new float[new_mesh->num_vertices * 3];
	memcpy(new_mesh->vertices, mesh->mVertices, sizeof(float) * new_mesh->num_vertices * 3);
	LOG("New mesh with %d vertices", new_mesh->num_vertices);

	if (mesh->HasTextureCoords(0))
	{
		new_mesh->texture_coords = new float[mesh->mNumVertices * 2];

		for (uint j = 0; j < mesh->mNumVertices * 2; j += 2)
		{
			new_mesh->texture_coords[j] = mesh->mTextureCoords[0][j / 2].x;
			new_mesh->texture_coords[j + 1] = mesh->mTextureCoords[0][j / 2].y;
		}
	}

	if (mesh->HasFaces())
	{
		new_mesh->num_indices = mesh->mNumFaces * 3;
		new_mesh->indices = new uint[new_mesh->num_indices]; // assume each face is a triangle
		for (uint i = 0; i < mesh->mNumFaces; ++i)
		{
			if (mesh->mFaces[i].mNumIndices != 3) { LOG("WARNING, geometry face with != 3 indices!"); }
			else { memcpy(&new_mesh->indices[i * 3], mesh->mFaces[i].mIndices, 3 * sizeof(uint)); }
		}

		aiString material_path;
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		material->GetTexture(aiTextureType_DIFFUSE, 0, &material_path);
		string material_name = material_path.C_Str();

		for (int i = path_file.size() - 1; i >= 0; i--)
		{
			if (path_file[i] == '/' || path_file[i] == '\\') break;
			else path_file.pop_back();
		}

		path_file += material_name;
		new_mesh->texture = Texturing(path_file.c_str());

		if (new_mesh->texture == 0)
		{
			path_file = "Assets/Textures/" + material_name;
			new_mesh->texture = Texturing(path_file.c_str());
			LOG("%s", path_file.c_str());
		}

		glGenBuffers(1, (GLuint*)&(new_mesh->id_indices));
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, new_mesh->id_indices);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float)*new_mesh->num_indices, new_mesh->indices, GL_STATIC_DRAW);		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);		glGenBuffers(1, (GLuint*) & (new_mesh->id_texcoords));
		glBindBuffer(GL_TEXTURE_COORD_ARRAY, new_mesh->id_texcoords);
		glBufferData(GL_TEXTURE_COORD_ARRAY, sizeof(uint) * new_mesh->num_vertices * 2, new_mesh->texture_coords, GL_STATIC_DRAW);		glBindBuffer(GL_TEXTURE_COORD_ARRAY, 0);
	}
		
	
	/*if (node->mNumMeshes > 0)
	{
		LOG("There are %i meshes", node->mNumMeshes);
	}
	else
	{
		LOG("No meshes were found");
	}*/
	return newGO;
}

bool ModuleLoader::loadFBXScene(const string& pFile)
{

	return true;
}

uint ModuleLoader::Texturing(const char* file_name)
{
	ILuint imageID = 0;
	GLuint textureID = 0;
	ILenum error;
	ILinfo ImageInfo;

	ilGenImages(1, &imageID);
	ilBindImage(imageID);

	if (ilLoadImage(file_name))
	{
		ilEnable(IL_ORIGIN_SET);
		ilOriginFunc(IL_ORIGIN_LOWER_LEFT);

		ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
		iluGetImageInfo(&ImageInfo);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_2D, textureID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		uint texture_width = ImageInfo.Width; uint texture_height = ImageInfo.Height;
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture_width, texture_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, ilGetData());
		glBindTexture(GL_TEXTURE_2D, 0);
		LOG("Texture loaded succesfuly!");

		TextureSize = { (float)texture_width, (float)texture_height };
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
