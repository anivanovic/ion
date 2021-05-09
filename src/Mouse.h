#ifndef __MOUSE_H__
#define __MOUSE_H__

#include <SDL2/SDL.h>

#include <glm/glm.hpp>

class Mouse {
 protected:
  bool m_buttonDown;
  bool m_isRotation;
  float m_yaw = -90.0, m_pitch = 0.0;
  SDL_Event m_event;
  static Mouse* _instance;
  double _x, _y;
  Mouse() {}

 public:
  Mouse(Mouse const&) = delete;
  void operator=(Mouse const&) = delete;

  bool isRotation();
  void setEvent(SDL_Event event);
  glm::vec3 getMove();
  void setRotation(bool isActive);
  float zoom();
  bool isZooming();
  bool isMove();
  void reset();
  static Mouse* get();
};

#endif