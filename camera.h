#ifndef CAMERA_H_INCLUDED
#define CAMERA_H_INCLUDED

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include "display.h"
#include <iostream>

struct Camera
{
public:
    Camera(Display* dis, glm::vec3 pos, double zNear, double zFar) {
        double aspectRatio = dis->AspectRatio();
        std::cout << "w: " << dis->Width() << "h: " << dis->Height() <<std::endl;
        std::cout << "aspect ration: " << aspectRatio << std::endl;
        // m_perspective = glm::ortho(-aspectRatio, aspectRatio, -1.0, 1.0, zNear, zFar);
        m_perspective = glm::perspective(70.0, aspectRatio, zNear, zFar);
        m_position = pos;
        m_forward = glm::vec3(0, 0, -1);
        m_up = glm::vec3(0, 1, 0);
    }

    inline glm::mat4 GetCameraProj() const {
        return m_perspective * glm::lookAt(m_position, m_position + m_forward, m_up);
    }

    void move(glm::vec3 move) {
        m_position = m_position + move;
    }

    void rotate (glm::vec3 direction) {
        m_forward = direction;
    }

    void zoom(float zoom) {
        m_position += (m_forward * zoom);
    }

private:
    glm::mat4 m_perspective;
    glm::vec3 m_position;
    glm::vec3 m_forward;
    glm::vec3 m_up;
};

#endif