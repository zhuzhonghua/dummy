#include "stdafx.h"

#include "render.h"
#include "image.h"
#include "bitmap_info_impl.h"

SDL_Renderer* Render::renderer;

void Render::setRenderCtx(SDL_Renderer* ren)
{
  renderer = ren;
}

BitmapInfo* Render::createBitmapInfoRGB(Image::RGB* im)
{
  BitmapInfoImpl* bi = new BitmapInfoImpl();
  
  SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA4444, SDL_TEXTUREACCESS_TARGET, im->width, im->height);
  SDL_UpdateTexture(texture, NULL, im->data, im->pitch);
  bi->setTex(texture);

  return bi;
}
