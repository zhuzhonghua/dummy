#pragma once

#include "def.h"

class BitmapInfoImpl: public BitmapInfo{
public:
  virtual int getWidth() const { return _width; }
  virtual int getHeight() const { return _height; }

  void setTex(SDL_Texture* t) { _tex=t; }
  SDL_Texture* getTex() { return _tex; }

	void setWidth(int w) { _width = w; }
	void setHeight(int h) { _height = h; }
protected:
  SDL_Texture* _tex;
  int _width;
  int _height;
};
