#pragma once
#include "Renderer.h"
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Camera.h"


namespace GE {

class TriangleRenderer :
    virtual public Renderer
{
public:
    TriangleRenderer();

    void InitializePosRotAndScaleAttributes();

    virtual ~TriangleRenderer();
    void Init() override;

    //Accessors
    float getPosX() const;

    float getPosY() const;

    float getPosZ() const;

    float getRotX() const;

    float getRotY() const;

    float getRotZ() const;

    float getScaleX() const;

    float getScaleY() const;

    float getScaleZ() const;

    //Mutator methods
    void setPos(float x, float y, float z);

    void setRotation(float rx, float ry, float rz);

    void setScale(float sx, float sy, float sz);
protected: 
    struct ShaderCreationalData {
        GLint* shaderIdPointer;
        const GLchar* const* shaderCode;
        int shaderType;
    };

    float posX, posY, posZ;
    float rotX, rotY, rotZ;
    float scaleX, scaleY, scaleZ;

    GLint vertexColorLocation;

    //GLSL uniform variables for the transformation, 
    // view and projection matricies

    GLuint transformuniformId;
    GLuint viewUniformId;
    GLuint projectionUniformId;

    void CreatePipelineShaders(std::vector<ShaderCreationalData> shaderCreationalDatas);
    GLint CreateShader(int shaderType, const GLchar* const* shaderCode);
    const GLint CheckIfShaderCompiledOk(GLint shader);
    void ShaderNotCompiledOkay(const GLuint& shader);
    void SendProgramObjectAndVertexBufferToRam(GLint& vertexShader, GLint& fragmentShader);
    void CreateExcecutableProgramObject(const GLint& vertexShader, const GLint& fragmentShader);
    void RetrieveVertexPos3DGLAttrib();
    void RetrieveVColourGLAttrib();
    void GetUniformIdsForViewProjectionAndTransformMatricies();
    void CreateAndSendVertexBufferToRam();
public:
    void Update() override;
    void Draw(Camera* cam) override;
    glm::mat4 CalculateTransformationMatrixForThisObject();
    void SetShaderUniforms(glm::mat4* transformationMat, glm::mat4* viewMat, glm::mat4* projectionMat);
    void GetReadyToRender();
    void CleanUpAfterRender();
    void Destroy() override;
};
}

