#include "texture.h"

#include <iostream>

#include "stb_image.h"

Texture::Texture(const std::string& fileName) {
  int width, height, numComponents;
  stbi_set_flip_vertically_on_load(true);
  stbi_uc* texture =
      stbi_load(fileName.c_str(), &width, &height, &numComponents, 4);

  if (texture == NULL) {
    std::cerr << "Error loading texture from file: " << fileName << std::endl;
  }

  glGenTextures(1, &m_texture);
  glBindTexture(GL_TEXTURE_2D, m_texture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
               GL_UNSIGNED_BYTE, texture);

  stbi_image_free(texture);
}

Texture::~Texture() { glDeleteTextures(1, &m_texture); }

void Texture::Bind(unsigned int unit) {
  assert(unit >= 0 && unit <= 31);

  glActiveTexture(GL_TEXTURE0 + unit);
  glBindTexture(GL_TEXTURE_2D, m_texture);
}