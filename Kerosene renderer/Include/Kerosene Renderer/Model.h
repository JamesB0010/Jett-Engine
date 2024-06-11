#pragma once
#include <gl/glew.h>
#include <vector>
#include "Renderer.h"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include<assimp/scene.h>
#include <string>
#include <unordered_map>
#include <array>
#include "../../MeshBoundingBox.h"

namespace GE {

    class ModelImportError :
        public std::exception
    {
        std::string message;
    public:
        ModelImportError(std::string msg) : message(msg) {}
        std::string what() {
            return message;
        }
    };


class Model
{
protected:
	GLuint vbo;
	int numVerticies;

    std::string* textureFilepath = nullptr;

    MeshBoundingBox meshBoundingBox;

    static Assimp::Importer* importer;
    static std::unordered_map<std::string, const aiScene*> cachedScenes;
public:

    static void ClearCachedModels();

	Model();

	virtual ~Model();

	void loadFromFile(const char* filename);

    void loadFromFileWithIntegratedReferencedTexture(const char* filepath);

    void createFromVertexPosVector(std::vector<glm::vec3> vertexPositions);

    MeshBoundingBox getMeshBoundingBox() {
        return this->meshBoundingBox;
    }


    const aiScene* RetrieveSceneFromFile(const char* filename, Assimp::Importer* importer);

    std::vector<GE::Vertex>* PopulateVertexVector(const aiScene* pScene);

    std::vector<GE::Vertex>* ProcessFacesOfMesh(const aiMesh* mesh);

    std::vector<GE::Vertex>* ProcessVerticiesOfFace(const aiMesh* mesh, const aiFace& face);

    void updateBoundingBoxValues(float x, float y, float z);

   int GetVertexIndexFromFaceRelativeToCurrentMesh(const aiFace& face, int indexRelativeToFace);

    void PopulateGlVertexBuffer(std::vector<GE::Vertex>& loadedVerticies);

	GLuint getVerticies() const;

	int getNumVerticies() const;

    GLuint getVbo() const;

    std::string getTextureFilepath() const;
};
}

