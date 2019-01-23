#include "stdafx.h"
#include "jpeg.h"
#include "image.h"

namespace Image{

ImageBase::ImageBase(Uint8* data, int width, int height, int pitch, IDImage type)
{
  this->data = data;
  this->width = width;
  this->height = height;
  this->pitch = pitch;
  this->type = type;
}

ImageBase::~ImageBase()
{
  if(data)
  {
    delete [] data;
  }
  data = NULL;
}

Uint8*	scanline(ImageBase* surf, int y)
{
  ASSERT(surf);
  ASSERT(y >= 0 && y < surf->height);
  return ((Uint8*) surf->data) + surf->pitch * y;
}


const Uint8*	scanline(const ImageBase* surf, int y)
{
  ASSERT(surf);
  ASSERT(y >= 0 && y < surf->height);
  return ((const Uint8*) surf->data) + surf->pitch * y;
}

RGB::RGB(int width, int height)
    :ImageBase(0,width,height,(width * 3 + 3) & ~3, IDImage::RGB)
{
  ASSERT(width > 0);
  ASSERT(height > 0);
  ASSERT(pitch >= width * 3);
  ASSERT((pitch & 3) == 0);

//		m_data = (Uint8*) dlmalloc(m_pitch * m_height);
  data = new Uint8[pitch * height];
}

// Create an system-memory rgb surface.  The data order is
// packed 24-bit, RGBRGB..., regardless of the endian-ness of
// the CPU.
RGB*	createRGB(int width, int height)
{
  RGB*	s = new RGB(width, height);

  return s;
}

RGBA::RGBA(int width, int height)
    :ImageBase(0, width, height, width * 4, IDImage::RGBA)
{
  ASSERT(width > 0);
  ASSERT(height > 0);
  ASSERT(pitch >= width * 4);
  ASSERT((pitch & 3) == 0);

//		m_data = (Uint8*) dlmalloc(m_pitch * m_height);
  data = new Uint8[pitch * height];
}

// Create an system-memory rgb surface.  The data order is
// packed 32-bit, RGBARGBA..., regardless of the endian-ness
// of the CPU.
RGBA*	createRGBA(int width, int height)
{
  RGBA*	s = new RGBA(width, height);

  return s;
}

void RGBA::setPixel(int x, int y, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
  ASSERT(x >= 0 && x < width);
  ASSERT(y >= 0 && y < height);

  Uint8*	data = scanline(this, y) + 4 * x;

  data[0] = r;
  data[1] = g;
  data[2] = b;
  data[3] = a;
}

// Create an system-memory 8-bit alpha surface.
Alpha*	createAlpha(int width, int height)
{
  Alpha*	s = new Alpha(width, height);

  return s;
}

Alpha::Alpha(int width, int height)
    :ImageBase(0, width, height, width, IDImage::ALPHA)
{
  ASSERT(width > 0);
  ASSERT(height > 0);

//		m_data = (Uint8*) dlmalloc(m_pitch * m_height);
  data = new Uint8[pitch * height];
}

void Alpha::setPixel(int x, int y, Uint8 a)
{
  ASSERT(x >= 0 && x < width);
  ASSERT(y >= 0 && y < height);

  Uint8*	data = scanline(this, y) + x;

  data[0] = a;
}

// Bitwise content comparison.
bool Alpha::operator==(const Alpha& a) const
{
  if (width != a.width || height != a.height)
  {
    return false;
  }

  for (int j = 0, n = height; j < n; j++)
  {
    if (memcmp(scanline(this, j), scanline(&a, j), width))
    {
      // Mismatch.
      return false;
    }
  }

  // Images are identical.
  return true;
}

// Return a hash code based on the image contents.
unsigned int Alpha::computeHash() const
{
  unsigned int	h = (unsigned int) Util::bernsteinHash(&width, sizeof(width));
  h = (unsigned int) Util::bernsteinHash(&height, sizeof(height), h);

  for (int i = 0, n = height; i < n; i++)
  {
    h = (unsigned int) Util::bernsteinHash(scanline(this, i), width, h);
  }

  return h;
}

// For reading SWF JPEG2-style image data (slight variation on
// ordinary JPEG).
// Create and read a new image from the stream.  Image is in
// SWF JPEG2-style format (the encoding tables come first in a
// separate "stream" -- otherwise it's just normal JPEG).  The
// IJG documentation describes this as "abbreviated" format.
RGB* readSWFJPEG2(File* in)
{
  JPEG::Input*	j_in = JPEG::Input::createSWFJPEG2HeaderOnly(in);
  if (j_in == NULL) return NULL;
		
  RGB*	im = readSWFJPEG2WithTables(j_in);

  delete j_in;

  return im;
}

// For reading SWF JPEG2-style image data, using pre-loaded
// headers stored in the given jpeg::input object.
RGB*	readSWFJPEG2WithTables(JPEG::Input* j_in)
{
  ASSERT(j_in);

  j_in->startImage();

  RGB*	im = Image::createRGB(j_in->getWidth(), j_in->getHeight());

  for (int y = 0; y < j_in->getHeight(); y++) {
    j_in->readScanline(scanline(im, y));
  }

  j_in->finishImage();

  return im;
}
};
