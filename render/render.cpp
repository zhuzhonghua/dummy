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

void Render::setBGColor(RGBA c)
{
  SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, c.a);
  SDL_RenderClear(renderer);
}

void Render::displayBitmapInfo(BitmapInfo* bi)
{
	SDL_Rect dst;
	dst.x = 0;
	dst.y = 0;
	dst.w = 100;
	dst.h = 100;
			
	BitmapInfoImpl* bii = dynamic_cast<BitmapInfoImpl*>(bi);
	SDL_RenderCopy(renderer, bii->getTex(), NULL, &dst);
}

