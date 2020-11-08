#ifndef __MOUSE_H__
#define __MOUSE_H__

#include <SDL2/SDL.h>

class Mouse
{
protected:
    bool m_active;
    SDL_Event* m_event;
    static Mouse* _instance;
    Mouse() {}

public:
    Mouse(Mouse const&) = delete;
    void operator=(Mouse const&) = delete;

    bool isActive();
    void setEvent(SDL_Event* event);
    static Mouse* get();
};

#endif