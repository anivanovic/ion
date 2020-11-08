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
        int x = event->motion.xrel;
        int y = event->motion.yrel;
        std::cout << "mouse x: " << x << ", y: " << y << std::endl;
    } else {
        if (m_event->button.button != SDL_BUTTON_LEFT) {
            std::cout << "not left button " << std::endl;
            return;
        }

        if (m_event->type == SDL_MOUSEBUTTONDOWN) {
            std::cout << "active " << std::endl;
            m_active = true;
        } else if (m_event->type == SDL_MOUSEBUTTONUP) {
            std::cout << "disabled " << std::endl;
            m_active = false;
        }
    }
}