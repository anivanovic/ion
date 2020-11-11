#ifndef __MOUSE_H__
#define __MOUSE_H__

#include <SDL2/SDL.h>
#include <glm/glm.hpp>

class Mouse
{
protected:
    bool m_active;
    SDL_Event* m_event;
    static Mouse* _instance;
    double _x, _y;
    Mouse() {}

public:
    Mouse(Mouse const&) = delete;
    void operator=(Mouse const&) = delete;

    bool isActive();
    void setEvent(SDL_Event* event);
    glm::vec3 getMove();
    static Mouse* get();
};

#endif