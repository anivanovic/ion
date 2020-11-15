#include "shader.h"
#include <iostream>
#include <fstream>

Shader::Shader(const std::string& file_name)
{
    m_program = glCreateProgram();
    m_shaders[0] = CreateShader(LoadShader(file_name + ".vs"), GL_VERTEX_SHADER);
    m_shaders[1] = CreateShader(LoadShader(file_name + ".fs"), GL_FRAGMENT_SHADER);

    glAttachShader(m_program, m_shaders[0]);
    glAttachShader(m_program, m_shaders[1]);

    glBindAttribLocation(m_program, 0, "position");
    glBindAttribLocation(m_program, 1, "texCoor");

    glLinkProgram(m_program);
    CheckShaderError(m_program, GL_LINK_STATUS, true, "Program failed to link");
    glValidateProgram(m_program);
    CheckShaderError(m_program, GL_VALIDATE_STATUS, true, "Program failed to validate");

    m_uniforms[TRANSFORM_U] = glGetUniformLocation(m_program, "transform");
    glDeleteShader(m_shaders[0]);
    glDeleteShader(m_shaders[1]);
}

Shader::~Shader()
{
    glDeleteProgram(m_program);
}

void Shader::Bind() {
    glUseProgram(m_program);
}

void Shader::UpdateTransform(const Transform& transform, const Camera& camera) {
	glm::mat4 model = camera.GetCameraProj() * transform.GetModel();
	glUniformMatrix4fv(m_uniforms[TRANSFORM_U], 1, GL_FALSE, &model[0][0]);
}

std::string Shader::LoadShader(const std::string& file_name)
{
    std::ifstream file;
    file.open((file_name).c_str());

    std::string output;
    std::string line;

    if(file.is_open())
    {
        while(file.good())
        {
            getline(file, line);
			output.append(line + "\n");
        }
    }
    else
    {
		std::cerr << "Unable to load shader: " << file_name << std::endl;
    }

    return output;
}

void Shader::CheckShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage)
{
    GLint success = 0;
    GLchar error[1024] = { 0 };

    if(isProgram)
        glGetProgramiv(shader, flag, &success);
    else
        glGetShaderiv(shader, flag, &success);

    if(success == GL_FALSE)
    {
        if(isProgram)
            glGetProgramInfoLog(shader, sizeof(error), NULL, error);
        else
            glGetShaderInfoLog(shader, sizeof(error), NULL, error);

        std::cerr << errorMessage << ": '" << error << "'" << std::endl;
    }
}

GLuint Shader::CreateShader(const std::string& text, unsigned int type)
{
    GLuint shader = glCreateShader(type);
    if (shader == 0) {
        std::cerr << "Error: Shader creation fail" << std::endl;
    }

    const GLchar* shaderSouceString[1];
    GLint shaderSouceLength[1];
    shaderSouceString[0] = text.c_str();
    shaderSouceLength[0] = text.length();

    glShaderSource(shader, 1, shaderSouceString, shaderSouceLength);
    glCompileShader(shader);
    CheckShaderError(shader, GL_COMPILE_STATUS, false, "Error compiling shader");

    return shader;
}