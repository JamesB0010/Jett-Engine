#pragma once
#include "Camera.h"
#include "Renderer.h"
#include "TriangleRenderer.h"
#include "Model.h"
#include "Texture.h"
#include "Terrain.h"
#include "Skydome.h"
#include "InstancedModel.h"
#include "Ocean.h";
#include "ShaderUtils.h"
#include "ModelRenderer.h"
#include "SkyboxRenderer.h"
class reflectiveModelRenderer: public ModelRenderer, public GE::TriangleRenderer
{
protected:
	GE::Texture* texture;

	GLint vertexUVLocation;

	GLuint samplerId;

	std::string defaultVertShader;
	std::string defaultFragShader;

	GLint cameraPosUniformName;

	GLuint skyboxSamplerId;
	GLuint skyboxCubeMapName;
public:
	reflectiveModelRenderer() {
		defaultVertShader = GE::loadShaderSourceCode("../../../Kerosene renderer/Shaders/default.vs");
		defaultFragShader = GE::loadShaderSourceCode("../../../Kerosene renderer/Shaders/default.fs");
	}
	virtual	void Init(const  GLchar* const* vertexShaderCode = nullptr, const GLchar* const* fragmentShaderCode = nullptr) override;
	virtual void Draw(GE::Camera* cam, GE::Model* model, bool backfaceCullingEnabled = true) override;
	void Draw(GE::Camera* cam, GE::Terrain* terrain);
	void Draw(GE::Camera* cam, GE::Skydome* skydome);
	
	GLuint getProgramId() {
		return this->programId;
	}

protected:
	void GetReadyToRender(GLuint vbo);
	void CleanUpAfterRender();
	void SendProgramObjectAndVertexBufferToRam(GLint& vertexShader, GLint& fragmentShader);
	void GetUniformIdForSampler();
	void RetrieveVUVColourGLAttrib();
public:
	void setTexture(GE::Texture* _tex);
};
