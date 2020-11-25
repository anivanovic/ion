#include "Mouse.h"
#include <iostream>

const float SENSITIVITY = 0.1;

Mouse* Mouse::_instance = nullptr;

Mouse* Mouse::get()
{
    if (_instance == nullptr)
    {
        _instance = new Mouse();
    }
    return _instance;
}

void Mouse::setEvent(SDL_Event event) {
    m_event = event;
    if (m_event.type == SDL_MOUSEMOTION) {
        _x = event.motion.xrel * 1.0;
        _y = event.motion.yrel * 1.0;
    } else {
        m_buttonDown = m_event.type == SDL_MOUSEBUTTONDOWN;
    }
}

void Mouse::setRotation(bool isActive) {
    m_isRotation = isActive;
}

bool Mouse::isRotation() {
    return m_isRotation && m_buttonDown;
}

bool Mouse::isMove() {
    return m_event.type == SDL_MOUSEMOTION && m_buttonDown;
}

float Mouse::zoom() {
    assert(isZooming());
    int change = m_event.wheel.y;
    return change > 0 ? -0.1 : 0.1;
}

bool Mouse::isZooming() {
    return m_event.type == SDL_MOUSEWHEEL;
}

glm::vec3 Mouse::getMove() {
    isZooming();
    if (isRotation()) {
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
        return direction;
    } else if (isZooming()) {
        return glm::vec3(zoom(), 0.0, 0.0);
    }

    return glm::vec3(_x, _y, 0.0);
}

/**
 * Reset event to null event if we where zooming to prevent
 * continuous zoom when we stop zooming.
 */
void Mouse::reset() {
    if (isZooming()) {
        SDL_Event e;
        m_event = e;
    }
}