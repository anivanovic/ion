#include "Mouse.h"
#include <iostream>

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
        // std::cout << "x: " << _x << ", y: " << _y << std::endl;
        // std::cout << "pos x: " << m_event->motion.x << ", y: " << m_event->motion.y << std::endl;
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

glm::vec3 Mouse::getMove() {
    return glm::vec3(-_x/100, _y/100, 0.0);
}