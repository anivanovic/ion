#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include <OpenGL/gl3.h>

class Texture
{
private:
    Texture(const Texture& other) {}
    void operator =(const Texture& other) {}

    GLuint m_texture;
public:
    Texture(const std::string& fileName);
    ~Texture();

    void Bind(unsigned int unit);
};

#endif
