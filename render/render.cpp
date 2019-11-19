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
  //SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB444, SDL_TEXTUREACCESS_TARGET, im->width, im->height);
  //SDL_UpdateTexture(texture, NULL, im->data, im->pitch);

	//SDL_RWops* rw = SDL_RWFromMem(im->data, im->pitch*im->height);
	//SDL_Surface* image = IMG_Load_RW(rw, 0);	
	//SDL_RWclose(rw);
	//
	//SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, image);  
  //SDL_FreeSurface(image);

	Uint32 rmask, gmask, bmask, amask;
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
  rmask = 0xff000000;
  gmask = 0x00ff0000;
  bmask = 0x0000ff00;
  amask = 0x000000ff;
#else
  rmask = 0x000000ff;
  gmask = 0x0000ff00;
  bmask = 0x00ff0000;
  amask = 0xff000000;
#endif
	
	SDL_Surface* image = SDL_CreateRGBSurfaceFrom(im->data, im->width, im->height, 24, im->pitch, rmask, gmask, bmask, amask);
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, image);  
  SDL_FreeSurface(image);
	
	BitmapInfoImpl* bi = new BitmapInfoImpl();
  bi->setTex(texture);
	bi->setWidth(im->width);
	bi->setHeight(im->height);
	
  return bi;
}

BitmapInfo* Render::createBitmapInfoRGB(Stream* in)
{
	int size = in->getTagEndPosition() - in->getPosition();
	byte* jpegData = new byte[size];
	ASSERT(in->readBytes(jpegData, size) == size);
	SDL_RWops* rw = SDL_RWFromMem(jpegData, size);
	SDL_Surface* image = IMG_Load_RW(rw, 0);	
	SDL_RWclose(rw);	
	delete jpegData;

	SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, image);  
  SDL_FreeSurface(image);

	BitmapInfoImpl* bi = new BitmapInfoImpl();
  bi->setTex(tex);
	//bi->setWidth(im->width);
	//bi->setHeight(im->height);
	return bi;
}

void Render::setBGColor(RGBA c)
{
  SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, c.a);
  SDL_RenderClear(renderer);
}

void Render::displayBitmapInfo(BitmapInfo* bi, RECT* pdst)
{
	//SDL_Rect dst;
	//dst.x = 0;
	//dst.y = 0;
	//dst.w = 307;
	//dst.h = 204;

	SDL_Rect dst;
	dst.x = pdst->xMin();
	dst.y = pdst->yMin();
	dst.w = pdst->width();
	dst.h = pdst->height();
	
	BitmapInfoImpl* bii = dynamic_cast<BitmapInfoImpl*>(bi);
	SDL_RenderCopy(renderer, bii->getTex(), NULL, &dst);
}

