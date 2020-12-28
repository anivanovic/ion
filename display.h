#ifndef DISPLAY_INCLUDED_H
#define DISPLAY_INCLUDED_H

#include <SDL2/SDL.h>

#include <string>

class Display {
 public:
  Display(int width, int height, const std::string& title);

  void Clear(float r, float g, float b, float a);
  void Update();
  bool isClosed();
  int Height();
  int Width();
  double AspectRatio();

  virtual ~Display();

 protected:
 private:
  void operator=(const Display& display) {}
  Display(const Display& display) {}

  SDL_Window* m_window;
  SDL_GLContext m_glContext;
  bool m_isClosed;

  int m_width, m_height;
};

#endif