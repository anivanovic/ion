#ifndef CAMERA_H_INCLUDED
#define CAMERA_H_INCLUDED

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include "display.h"

struct Camera
{
public:
    Camera(Display* dis, glm::vec3 pos, double zNear, double zFar) {
        double aspectRatio = (1.0 * dis->Width()) / dis->Height();
        m_perspective = glm::ortho(-1.0, 1.0, -1.0, 1.0, zNear, zFar);
        m_position = pos;
        m_forward = glm::vec3(0, 0, -1);
        m_up = glm::vec3(0, 1, 0);
    }

    inline glm::mat4 GetCameraProj() const {
        return m_perspective * glm::lookAt(m_position, m_position + m_forward, m_up);
    }

private:
    glm::mat4 m_perspective;
    glm::vec3 m_position;
    glm::vec3 m_forward;
    glm::vec3 m_up;
};

#endif