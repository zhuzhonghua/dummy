#include "stdafx.h"
#include "window.h"
#include "swf_player.h"
#include "render/render.h"

Window::Window()
{
  _width = 960;
  _height = 640;

  _exit = false;
  _p = NULL;
}

void Window::init(const std::string&title)
{
  _p = new SWFPlayer(title);
  _p->readHead();
  
  _width = _p->getWidth();
  _height = _p->getHeight();
  
  SDL_Init(SDL_INIT_EVERYTHING);
  _window = SDL_CreateWindow( title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, _width, _height, SDL_WINDOW_SHOWN );
  _renderer = SDL_CreateRenderer(_window, -1, 0);
  Render::setRenderCtx(_renderer);
  
  _p->readTags();
  
  _fpsLimiter.setMaxFPS(_p->getFrameRate());
}

void Window::run()
{
  SDL_StartTextInput();	
  while(_exit == false){
    _fpsLimiter.begin();
    
    processInput();
    _p->advance();
    _p->display();
    
    SDL_RenderPresent(_renderer);
    
    _fps = _fpsLimiter.end();
  }
  SDL_StopTextInput();

  SDL_DestroyWindow(_window);
  SDL_DestroyRenderer(_renderer);
  SDL_Quit();
}

void Window::processInput()
{
  SDL_Event e;
  while(SDL_PollEvent(&e) != 0){
    switch(e.type){
    case SDL_QUIT:
      _exit = true;
      break;
    case SDL_MOUSEMOTION:
      break;
    case SDL_KEYDOWN:
      break;
    case SDL_KEYUP:
      break;
    case SDL_MOUSEBUTTONDOWN:
      break;
    case SDL_MOUSEBUTTONUP:
      break;
    }
  }
}
