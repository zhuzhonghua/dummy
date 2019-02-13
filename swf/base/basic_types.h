#pragma once

// tu_float is used in matrix & cxform because
// Flash converts inf to zero when works with matrix & cxform
class Float
{
public:
  Float()	{ _float = 0.0f; }
  Float(float x)	{ operator=(x); }
  Float(double x)	{ operator=((float) x); }

  operator float() const { return _float; }
  inline void	operator=(float x)
  {
    _float = x >= -3.402823466e+38F && x <= 3.402823466e+38F ? x : 0.0f;
  }
  void	operator+=(const float x) { operator=(_float + x); }
  void	operator-=(const float x) { operator=(_float - x); }
  void	operator*=(const float x) { operator=(_float * x); }
  void	operator/=(const float x) { operator=(_float / x); }

private:
  float _float;
};


#include "rect.h"
#include "point.h"
#include "matrix.h"
#include "stream.h"

class RGBA
{
public:
  RGBA();
  RGBA(UInt8 r, UInt8 g, UInt8 b, UInt8 a);

  RGBA(double color);
  void	read(Stream* in, const TagInfo &info);
  void	read(Stream* in, int tag_type);
  void	readRGBA(Stream* in);
  void	readRGB(Stream* in);

  void	set(UInt8 r, UInt8 g, UInt8 b, UInt8 a);

  void	set(double color);

  void	setLerp(RGBA& a, RGBA& b, float f);

public:
  UInt8	r, g, b, a;
};

//
// cxform: color transform type, used by render handler
//
struct CXForm
{
  Float	m[4][2];	// [RGBA][mult, add]

  CXForm();
  void	concatenate(const CXForm& c);
  RGBA	transform(RGBA& in) const;
  void	readRGB(Stream* in);
  void	readRGBA(Stream* in);
  void	clamp();  // Force component values to be in range.
  //void	print() const;

  static CXForm	identity;
};
