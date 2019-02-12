#pragma once

#include "timing.h"

class SWFPlayer;
class Window{
public:
  Window();
  void init(const std::string&title);
  void run();

protected:
  void processInput();  
protected:
  bool _exit;
  FPSLimiter _fpsLimiter;

  float _fps;
protected:
  int _width;
  int _height;
  
  SDL_Window* _window;
  SDL_Renderer* _renderer;

  SWFPlayer* _p;
};
