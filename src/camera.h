#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/transform.hpp>
#include <iostream>

#include "display.h"

struct Camera {
 public:
  Camera(Display* dis, glm::vec3 pos, double zNear, double zFar) {
    double aspectRatio = dis->AspectRatio();
    std::cout << "w: " << dis->Width() << "h: " << dis->Height() << std::endl;
    std::cout << "aspect ration: " << aspectRatio << std::endl;
    // m_perspective = glm::ortho(-aspectRatio, aspectRatio, -1.0, 1.0, zNear,
    // zFar);
    m_perspective = glm::perspective(70.0, aspectRatio, zNear, zFar);
    m_position = pos;
    m_forward = glm::vec3(0, 0, -1);
    m_up = glm::vec3(0, 1, 0);
    m_right = glm::vec3(1, 0, 0);
  }

  inline glm::mat4 GetCameraProj() const {
    return m_perspective *
           glm::lookAt(m_position, m_position + m_forward, m_up);
  }

  void move(glm::vec3 move) {
    float camera_rot = glm::acos(glm::dot(glm::vec3(1, 0, 0), m_right));
    glm::mat4 rotation = glm::rotate(camera_rot, glm::vec3(0, 0, 1));
    glm::vec3 forward_xy = rotation * glm::vec4(move, 0);
    m_position += forward_xy;
  }

  void rotate(glm::vec3 direction) {
    float x_rad = direction.x;
    float y_rad = direction.y;

    glm::mat4 x_rot = glm::rotate(x_rad, glm::vec3(0.0f, 0.0f, 1.0f));
    glm::mat4 y_rot = glm::rotate(y_rad, m_right);
    m_forward = glm::normalize(x_rot * y_rot * glm::vec4(m_forward, 0.0f));
    m_up = glm::normalize(x_rot * y_rot * glm::vec4(m_up, 0.0f));
    m_right = glm::normalize(glm::cross(m_forward, m_up));
  }

  void zoom(float zoom) { m_position += (m_forward * zoom); }

 private:
  glm::mat4 m_perspective;
  glm::vec3 m_position;
  glm::vec3 m_forward;
  glm::vec3 m_up;
  glm::vec3 m_right;
};