#pragma once
#include "ModelRenderer.h"
#include "ShaderUtils.h"
#include <glm/gtc/matrix_transform.hpp>
#include "../../../Engine/Include/Jett Engine/Time.h"
#include <random>

class OceanModelRenderer : public ModelRenderer
{
	Ocean* ocean;

	GLuint timeUniformId, viewVecUniformId, WaveDirections;

public:

	void Init(Ocean* ocean);

	void Draw(GE::Camera* cam, GE::Texture* texture);

private:
	void SendProgramObjectAndVertexBufferToRam(GLint& vertexShader, GLint& fragmentShader);

	void GetReadyToRender(GLuint vbo);

	void CleanUpAfterRender();
};

