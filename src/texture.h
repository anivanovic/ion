#pragma once

#include <OpenGL/gl3.h>

#include <string>

class Texture {
 private:
  GLuint m_texture;
  Texture(const Texture& other) {}
  void operator=(const Texture& other) {}

 public:
  Texture(const std::string& fileName);
  ~Texture();

  void Bind(unsigned int unit);
};
