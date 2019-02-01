#pragma once

namespace Image{
class RGB;
};

class Render{
protected:
  static SDL_Renderer* renderer;
public:
  static void setRenderCtx(SDL_Renderer* ren);
  static BitmapInfo* createBitmapInfoRGB(Image::RGB* im);
};
