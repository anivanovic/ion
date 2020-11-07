#ifndef _SHADER_INCLUDES_H_
#define _SHADER_INCLUDES_H_

#include <string>
#include <OpenGL/gl3.h>
#include "transform.h"

class Shader
{
public:
    Shader(const std::string& file_name);
    virtual ~Shader();

    void Bind();
    void UpdateTransform(const Transform& transform, const Camera& camera);

    void CheckShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage);
    std::string LoadShader(const std::string& file_name);
    GLuint CreateShader(const std::string& text, unsigned int type);
private:
    // static const unsigned int NUM_SHADERS = 2;
    static const unsigned int NUM_SHADERS = 1;
    Shader(const Shader& other) {}
    void operator=(const Shader& other) {}

    enum {
        TRANSFORM_U,
        NUM_UNIFORMS
    };

    GLuint m_program;
    GLuint m_shaders[NUM_SHADERS];
    GLuint m_uniforms[NUM_UNIFORMS];
};

#endif