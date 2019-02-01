#pragma once

#include "jpeg.h"

namespace Image{
struct ImageBase{
  enum IDImage
    {
      INVALID,
      RGB,
      RGBA,
      ALPHA
    };

  IDImage type;
  Uint8*	data;
  int	width;
  int	height;
  int	pitch;	// byte offset from one row to the next

  ImageBase(Uint8* data, int width, int height, int pitch, IDImage type);
  virtual ~ImageBase();
};

// 24-bit RGB image.  Packed data, red byte first (RGBRGB...)
struct RGB : public ImageBase
{
  RGB(int width, int height);
  ~RGB(){}
};

// 32-bit RGBA image.  Packed data, red byte first (RGBARGBA...)
struct RGBA : public ImageBase
{
  RGBA(int width, int height);
  ~RGBA(){}

  void	setPixel(int x, int y, Uint8 r, Uint8 g, Uint8 b, Uint8 a);
};

// 8-bit alpha image.
struct Alpha : public ImageBase
{
  Alpha(int width, int height);
  ~Alpha(){}

  void	setPixel(int x, int y, Uint8 a);

  // Bitwise content comparison.
  bool	operator==(const Alpha& a) const;

  // Return a hash code based on the image contents.
  unsigned int	computeHash() const;
};

// Make a system-memory 24-bit bitmap surface.  24-bit packed
// data, red byte first.
RGB*	createRGB(int width, int height);


// Make a system-memory 32-bit bitmap surface.  Packed data,
// red byte first.
RGBA*	createRGBA(int width, int height);


// Make a system-memory 8-bit bitmap surface.
Alpha*	createAlpha(int width, int height);

	
Uint8*	scanline(ImageBase* surf, int y);
const Uint8*	scanline(const ImageBase* surf, int y);

// For reading SWF JPEG2-style image data (slight variation on
// ordinary JPEG).
RGB*	readSWFJPEG2(File* in);

// For reading SWF JPEG2-style image data, using pre-loaded
// headers stored in the given jpeg::input object.
RGB*	readSWFJPEG2WithTables(JPEG::Input* loader);
};
