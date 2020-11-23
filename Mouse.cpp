#include "Mouse.h"
#include <iostream>

const float SENSITIVITY = 0.1;

Mouse* Mouse::_instance = nullptr;

bool Mouse::isActive()
{
    return m_active;
}

Mouse* Mouse::get()
{
    if (_instance == nullptr)
    {
        _instance = new Mouse();
    }
    return _instance;
}

void Mouse::setEvent(SDL_Event* event) {
    m_event = event;
    if (m_event->type == SDL_MOUSEMOTION) {
        _x = event->motion.xrel * 1.0;
        _y = event->motion.yrel * 1.0;
    } else {
        if (m_event->button.button != SDL_BUTTON_LEFT) {

            std::cout << "not left button " << std::endl;
            return;
        }

        if (m_event->type == SDL_MOUSEBUTTONDOWN) {
            m_active = true;
        } else if (m_event->type == SDL_MOUSEBUTTONUP) {
            m_active = false;
        }
    }
}

void Mouse::setRotation(bool isActive) {
    m_isRotation = isActive;
}

bool Mouse::isRotation() {
    return m_isRotation;
}

glm::vec3 Mouse::getMove() {
    if (m_isRotation) {
        _x *= SENSITIVITY;
        _y *= -SENSITIVITY;

        m_yaw += _x;
        m_pitch += _y;

        if (m_pitch > 89.0) m_pitch = 89.0;
        if (m_pitch < -89.0) m_pitch = -89.0;
        if (m_yaw > 180.0) m_yaw = 180.0;
        if (m_yaw < -180.0) m_yaw = -180.0;

        glm::vec3 direction;
        direction.x = glm::cos(glm::radians(m_yaw)) * glm::cos(glm::radians(m_pitch));
        direction.y = glm::sin(glm::radians(m_pitch));
        direction.z = glm::sin(glm::radians(m_yaw)) * glm::cos(glm::radians(m_pitch));
        direction = glm::normalize(direction);
        std::cout << "rotation " << direction.x << " " << direction.y << " " << direction.z << std::endl;
        return direction;
    }

    return glm::vec3(_x, _y, 0.0);
}