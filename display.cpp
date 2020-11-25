#include "display.h"
#ifdef __APPLE__
#include <OpenGL/gl3.h>
#include <OpenGL/glext.h>
#else
#include <GL/glew.h>
#endif
#include <iostream>
#include "stb_image.h"
#include "Mouse.h"

Display::Display(int width, int height, const std::string &title) : m_height(height),
                                                                    m_width(width)
{
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    m_window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    m_glContext = SDL_GL_CreateContext(m_window);
    SDL_GL_GetDrawableSize(m_window, &m_width, &m_height);

    std::cout << "width: " << m_width << " height: " << m_height << std::endl;

#ifndef __APPLE__
    if (glewInit() != GLEW_OK)
    {
        std::cerr << "Glew failed to initialize!" << std::endl;
    }
#endif

    glEnable(GL_DEPTH_TEST);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    m_isClosed = false;
}

Display::~Display()
{
    SDL_GL_DeleteContext(m_glContext);
    SDL_DestroyWindow(m_window);
    SDL_Quit();
}

void Display::Clear(float r, float g, float b, float a)
{
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Display::Update()
{
    SDL_GL_SwapWindow(m_window);

    SDL_Event e;
    Mouse::get()->reset();
    while (SDL_PollEvent(&e))
    {
        switch (e.type)
        {
        case SDL_QUIT:
            m_isClosed = true;
            break;
        case SDL_MOUSEMOTION:
        case SDL_MOUSEBUTTONDOWN:
        case SDL_MOUSEBUTTONUP:
        case SDL_MOUSEWHEEL:
            Mouse::get()->setEvent(e);
            break;
        case SDL_KEYDOWN:
            if (e.key.keysym.sym == SDLK_LCTRL) {
                Mouse::get()->setRotation(true);
            }
            break;
        case SDL_KEYUP:
            if (e.key.keysym.sym == SDLK_LCTRL) {
                Mouse::get()->setRotation(false);
            }
            break;
        default:
            break;
        }
    }
}

int Display::Height()
{
    return m_height;
}

int Display::Width()
{
    return m_width;
}

double Display::AspectRatio() {
    return (1.0 * m_width) / (1.0 * m_height);
}

bool Display::isClosed()
{
    return m_isClosed;
}