#include "../Include/Kerosene Renderer/Model.h"
Assimp::Importer* GE::Model::importer = new Assimp::Importer();

std::unordered_map<std::string, const aiScene*> GE::Model::cachedScenes = std::unordered_map<std::string, const aiScene*>();

void GE::Model::ClearCachedModels() {
	Model::cachedScenes.clear();
	delete Model::importer;
	Model::importer = new Assimp::Importer();
}

GE::Model::Model()
{
	this->numVerticies = 0;
}

GE::Model::~Model()
{
	glDeleteBuffers(1, &this->vbo);
}

void GE::Model::loadFromFile(const char* filename)
{
	std::vector<GE::Vertex>* loadedVerticies = this->PopulateVertexVector(RetrieveSceneFromFile(filename, Model::importer));;
	
	this->numVerticies = loadedVerticies->size();

	//copy verticies into a memory buffer in order to
	//transfer to a vbo later
	glGenBuffers(1, &this->vbo);

	PopulateGlVertexBuffer(*loadedVerticies);

	delete loadedVerticies;
	//remove the buffer from the pipeline
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void GE::Model::loadFromFileWithIntegratedReferencedTexture(const char* filepath) {
	const aiScene* scene = this->RetrieveSceneFromFile(filepath, Model::importer);

	std::vector<GE::Vertex>* loadedVerticies = this->PopulateVertexVector(scene);

	this->numVerticies = loadedVerticies->size();

	aiMesh* mesh = scene->mMeshes[0];

	aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
	aiString texturePath;
	if (material->GetTexture(aiTextureType_DIFFUSE, 0, &texturePath) == AI_SUCCESS) {
		this->textureFilepath = new std::string(texturePath.C_Str());
	}

	glGenBuffers(1, &this->vbo);

	this->PopulateGlVertexBuffer(*loadedVerticies);

	delete loadedVerticies;

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void GE::Model::createFromVertexPosVector(std::vector<glm::vec3> vertexPositions)
{
	std::vector<Vertex> loadedVerticies;

	for (glm::vec3 vertexPos : vertexPositions) {
		glm::vec2 uv = glm::vec2(vertexPos.x, vertexPos.y);
		uv.x += 1;
		uv.y += 1;
		uv.x /= 2;
		uv.y /= 2;

		loadedVerticies.push_back(Vertex::CreateVertexFromCoordinatesAndUV(vertexPos, uv));
	}

	GLuint vertBuffObj;
	glGenBuffers(1, &vertBuffObj);

	glBindBuffer(GL_ARRAY_BUFFER, vertBuffObj);

	glBufferData(GL_ARRAY_BUFFER, vertexPositions.size() * sizeof(GE::Vertex), loadedVerticies.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	this->vbo = vertBuffObj;

	this->numVerticies = vertexPositions.size();
}


const aiScene* GE::Model::RetrieveSceneFromFile(const char* filename, Assimp::Importer* importer)
{
	//first check if the file has already been loaded
	//to do this we will check if the filename is present in the loadedScenesMap

	auto it = Model::cachedScenes.find(std::string(filename));

	bool sceneFound = it != Model::cachedScenes.end();
	if (sceneFound) {
		return it->second;
	}

	//if this model has not yet been loaded then load it

	const aiScene* pScene = Model::importer->ReadFile(filename, aiProcessPreset_TargetRealtime_Quality | aiProcess_FlipUVs);

	if (!pScene)
		throw GE::ModelImportError(Model::importer->GetErrorString());

	//once loaded add the scene to the loadedScenes map
	Model::cachedScenes[std::string(filename)] = pScene;

	return pScene;
}

std::vector<GE::Vertex>* GE::Model::PopulateVertexVector(const aiScene* pScene)
{
	std::vector<GE::Vertex>* loadedVerticies = new std::vector<GE::Vertex>();

	//for each mesh in the scene
	for (int meshIdx = 0; meshIdx < pScene->mNumMeshes; meshIdx++) {
		const aiMesh* mesh = pScene->mMeshes[meshIdx];

		std::vector<Vertex>* meshVerts = ProcessFacesOfMesh(mesh);

		//append returned vector to loadedVerticies
		loadedVerticies->insert(loadedVerticies->end(), meshVerts->begin(), meshVerts->end());
		delete meshVerts;
	}
	return loadedVerticies;
}

std::vector<GE::Vertex>* GE::Model::ProcessFacesOfMesh(const aiMesh* mesh)
{
	std::vector<GE::Vertex>* thisMeshVerticies = new std::vector<Vertex>;
	//for each face of the mesh
	for (int faceIdx = 0; faceIdx < mesh->mNumFaces; faceIdx++) {
		const aiFace& face = mesh->mFaces[faceIdx];

		std::vector<GE::Vertex>* faceVerticies = ProcessVerticiesOfFace(mesh, face);

		//append this face verticies onto the mesh verticies vector
		thisMeshVerticies->insert(thisMeshVerticies->end(), faceVerticies->begin(), faceVerticies->end());

		delete faceVerticies;
	}

	return thisMeshVerticies;
}

std::vector<GE::Vertex>* GE::Model::ProcessVerticiesOfFace(const aiMesh* mesh, const aiFace& face)
{
	std::vector<GE::Vertex>* verticies = new std::vector<GE::Vertex>();
	//for each vertex of the face (triangle
	for (int vertIdx = 0; vertIdx < 3; vertIdx++) {
		int vIndex = this->GetVertexIndexFromFaceRelativeToCurrentMesh(face, vertIdx);
		//get the mesg from the mVerticies collection using the vIndex which is relative to the current mesh
		const aiVector3D* currentVertexPos = &mesh->mVertices[vIndex];

		if (vIndex != -559038737) {
			updateBoundingBoxValues(currentVertexPos->x, currentVertexPos->y, currentVertexPos->z);
		}

		//get the uvs for vertex. this code assumes there are uvs
		//defined in the model verticies. If not, then code will
		//crash
		if (mesh->HasTextureCoords(0) && face.mIndices[vertIdx] < mesh->mNumVertices) {
			const aiVector3D uv = mesh->mTextureCoords[0][face.mIndices[vertIdx]];
			const aiVector3D* norm = &mesh->mNormals[face.mIndices[vertIdx]];
		//create a vertex at the current vertex position with a colour
		verticies->push_back(GE::Vertex::CreateVertexFromCoordinatesAndUVAndNormal(
			glm::vec3(currentVertexPos->x, currentVertexPos->y, currentVertexPos->z),
			glm::vec2(uv.x, uv.y),
			glm::vec3(norm->x, norm->y, norm->z)
		));
		}
		else {
			verticies->push_back(GE::Vertex::CreateVertexFromCoordinatesAndUVAndNormal(
				glm::vec3(0,0,0),
				glm::vec2(0, 0),
				glm::vec3(0,1,0)
			));
		}

	}

	return verticies;
}

void GE::Model::updateBoundingBoxValues(float x, float y, float z)
{
	//---------    X AXIS
	//less that current min x
	if (x < this->meshBoundingBox.minMaxX.x)
		this->meshBoundingBox.minMaxX.x = x;
	

	//greater then current max x
	if (x > this->meshBoundingBox.minMaxX.y)
		this->meshBoundingBox.minMaxX.y = x;
	


	//---------    Y AXIS

	//less than current min y
	if (y < this->meshBoundingBox.minMaxY.x) 
		this->meshBoundingBox.minMaxY.x = y;
	

	//greater than current max y
	if (y > this->meshBoundingBox.minMaxY.y) 
		this->meshBoundingBox.minMaxY.y = y;
	

	//---------    Z AXIS

	//less than current min z

	if (z < this->meshBoundingBox.minMaxZ.x) 
		this->meshBoundingBox.minMaxZ.x = z;
	

	//greater then current max z

	if (z > this->meshBoundingBox.minMaxZ.y) 
		this->meshBoundingBox.minMaxZ.y = z;
	
}

int GE::Model::GetVertexIndexFromFaceRelativeToCurrentMesh(const aiFace& face, int indexRelativeToFace) {
	return face.mIndices[indexRelativeToFace];
}

void GE::Model::PopulateGlVertexBuffer(std::vector<GE::Vertex>& loadedVerticies)
{
	//create the buffer
	glBindBuffer(GL_ARRAY_BUFFER, this->vbo);

	//copy vertex data from vector to buffer
	glBufferData(GL_ARRAY_BUFFER, this->numVerticies * sizeof(GE::Vertex), loadedVerticies.data(), GL_STATIC_DRAW);
}
GLuint GE::Model::getVerticies() const
{
	return this->vbo;
}

int GE::Model::getNumVerticies() const
{
	return this->numVerticies;
}

GLuint GE::Model::getVbo() const
{
	return this->vbo;
}

std::string GE::Model::getTextureFilepath() const {
	return *this->textureFilepath;
}
