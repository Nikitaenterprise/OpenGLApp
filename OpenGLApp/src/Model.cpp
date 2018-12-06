#include "Model.h"



Model::Model()
{
}

void Model::loadModel(const std::string & _fileName)
{
	Assimp::Importer importer;
	const aiScene *scene = importer.ReadFile(_fileName, aiProcess_Triangulate | 
		aiProcess_FlipUVs | aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices);
	if (!scene)
		std::cout << "Model " << _fileName << " failed to load " << importer.GetErrorString() << std::endl;

	loadNode(scene->mRootNode, scene);
	loadMaterials(scene);
}

void Model::renderModel()
{
	for (size_t i = 0; i < meshVector.size(); i++)
	{
		unsigned int materialIndex = meshToTexture[i];

		if (materialIndex < textureVector.size() && textureVector[materialIndex])
			textureVector[materialIndex]->useTexture();
		meshVector[i]->renderMesh();
	}
}

void Model::clearModel()
{
	for (size_t i = 0; i < meshVector.size(); i++)
	{
		if (meshVector[i])
		{
			delete meshVector[i];
			meshVector[i] = nullptr;
		}
	}

	for (size_t i = 0; i < textureVector.size(); i++)
	{
		if (textureVector[i])
		{
			delete textureVector[i];
			textureVector[i] = nullptr;
		}
	}
}


Model::~Model()
{
}

void Model::loadNode(aiNode *_node, const aiScene *_scene)
{
	for (size_t i = 0; i < _node->mNumMeshes; i++)
		loadMesh(_scene->mMeshes[_node->mMeshes[i]], _scene);
	
	for (size_t i = 0; i < _node->mNumChildren; i++)
		loadNode(_node->mChildren[i], _scene);
}

void Model::loadMesh(aiMesh *_mesh, const aiScene *_scene)
{
	std::vector<GLfloat> vertices;
	std::vector<unsigned int> indeces;

	for (size_t i = 0; i < _mesh->mNumVertices; i++)
	{
		vertices.insert(vertices.end(), { _mesh->mVertices[i].x, _mesh->mVertices[i].y , _mesh->mVertices[i].z });
		
		if (_mesh->mTextureCoords[0])
			vertices.insert(vertices.end(), { _mesh->mTextureCoords[0][i].x, _mesh->mTextureCoords[0][i].y });
		else
			vertices.insert(vertices.end(), { 0.0f, 0.0f });

		vertices.insert(vertices.end(), { -_mesh->mNormals[i].x, -_mesh->mNormals[i].y , -_mesh->mNormals[i].z });
	}

	for (size_t i = 0; i < _mesh->mNumFaces; i++)
	{
		aiFace face = _mesh->mFaces[i];
		for (size_t j = 0; j < face.mNumIndices; j++)
			indeces.push_back(face.mIndices[j]);
	}

	Mesh *newMesh = new Mesh();
	newMesh->createMesh(&vertices[0], &indeces[0], vertices.size(), indeces.size());
	meshVector.push_back(newMesh);
	meshToTexture.push_back(_mesh->mMaterialIndex);
}

void Model::loadMaterials(const aiScene * _scene)
{
	textureVector.resize(_scene->mNumMaterials);

	for (size_t i = 0; i < _scene->mNumMaterials; i++)
	{
		aiMaterial *material = _scene->mMaterials[i];
		textureVector[i] = nullptr;

		if (material->GetTextureCount(aiTextureType_DIFFUSE))
		{
			aiString path;
			if (material->GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS)
			{
				int index = std::string(path.data).rfind("\\");
				std::string fileName = std::string(path.data).substr(index + 1);

				std::string texPath = std::string("Textures/") + fileName;
				textureVector[i] = new Texture(texPath.c_str());

				if (!textureVector[i]->loadTexture())
				{
					std::cout << "Failed to load texture at " << texPath << "\n";
					delete textureVector[i];
					textureVector[i] = nullptr;
				}
			}
		}
		if (!textureVector[i])
		{
			textureVector[i] = new Texture("Textures/white.png");
			textureVector[i]->loadTextureA();
		}
	}

}
