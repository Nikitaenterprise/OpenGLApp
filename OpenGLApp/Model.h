#pragma once

#include <vector>
#include <string>
#include <iostream>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.h"
#include "Texture.h"

class Model
{
public:

	Model();

	void loadModel(const std::string &_fileName);
	void renderModel();
	void clearModel();

	~Model();

private:

	void loadNode(aiNode *_node, const aiScene *_scene);
	void loadMesh(aiMesh *_mesh, const aiScene *_scene);
	void loadMaterials(const aiScene *_scene);

	std::vector<Mesh*> meshVector;
	std::vector<Texture*> textureVector;
	std::vector<unsigned int> meshToTexture;
};

