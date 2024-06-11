#pragma once
#include "ModelRenderer.h"
#include "ShaderUtils.h"
#include "../../../Engine/Include/Jett Engine/UsefulStructs.h"
class InstancedModelRenderer :public ModelRenderer
{
	GLuint translationOffsetsLocation;
public:
	void Init() override;

	void Draw(GE::Camera* cam, InstancedModel instancedModel);

	void RetrieveTranslationOffsets();

private:
	void SendProgramObjectAndVertexBufferToRam(GLint& vertexShader, GLint& fragmentShader);

	void SetShaderUniforms(glm::mat4* transformationMat, glm::mat4* viewMat, glm::mat4* projectionMat, glm::vec3* translateOffsets, int numberOfInstances);
};

