#ifndef CAMERA_H_INCLUDED
#define CAMERA_H_INCLUDED

#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
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
        m_position += move;
    }

    void rotate (glm::vec3 direction) {
	float x_rad = direction.x;
	float y_rad = direction.y;

	glm::vec3 right = glm::normalize(glm::cross(m_forward, m_up));
	glm::mat4 x_rot = glm::rotate(x_rad, glm::vec3(0.0f, 0.0f, 1.0f));
	glm::mat4 y_rot = glm::rotate(y_rad, right); 
	m_forward = glm::normalize(x_rot * y_rot * glm::vec4(m_forward, 0.0f));
        m_up = glm::normalize(x_rot * y_rot * glm::vec4(m_up, 0.0f));
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
