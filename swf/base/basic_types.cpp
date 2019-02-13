#include "stdafx.h"

#include "basic_types.h"
#include "def.h"
#include "stream.h"

RGBA::RGBA() : 
  r(255), 
  g(255), 
  b(255), 
  a(255) 
{

}

RGBA::RGBA(UInt8 r, UInt8 g, UInt8 b, UInt8 a) :
  r(r), 
  g(g), 
  b(b), 
  a(a)
{
}

RGBA::RGBA(double color)
{
  set(color);
}

void RGBA::read(Stream* in, const TagInfo& info)
{
  if (info.tagType <= Tag::DEFINESHAPE2)
  {
    readRGB(in);
  }
  else
  {
    readRGBA(in);
  }
}

void RGBA::read(Stream* in, int tag_type)
{
  TagInfo ti;
  ti.tagType = (Tag)tag_type;
  read(in, ti);
}

void RGBA::readRGBA(Stream* in)
{
  readRGB(in);
  a = in->readUI8();
}

void RGBA::readRGB(Stream* in)
{
  r = in->readUI8();
  g = in->readUI8();
  b = in->readUI8();

  a = 0xFF;
}

void RGBA::set(UInt8 r, UInt8 g, UInt8 b, UInt8 a)
{
  r = r;
  g = g;
  b = b;
  a = a;
}

void RGBA::set(double color)
{
  int rgb = int(color);
  r = (UInt8)(rgb >> 16) & 0xFF;
  g = (UInt8)(rgb >> 8) & 0xFF;
  b = (UInt8)rgb & 0xFF;
  a = 0xFF;
}

void RGBA::setLerp(RGBA& a, RGBA& b, float f)
{
  r = (UInt8)Util::frnd(Util::flerp(a.r, b.r, f));
  g = (UInt8)Util::frnd(Util::flerp(a.g, b.g, f));
  b = (UInt8)Util::frnd(Util::flerp(a.b, b.b, f));
  a = (UInt8)Util::frnd(Util::flerp(a.a, b.a, f));
}

CXForm	CXForm::identity;


CXForm::CXForm()
// Initialize to identity transform.
{
  m[0][0] = 1;
  m[1][0] = 1;
  m[2][0] = 1;
  m[3][0] = 1;
  m[0][1] = 0;
  m[1][1] = 0;
  m[2][1] = 0;
  m[3][1] = 0;
}

void	CXForm::concatenate(const CXForm& c)
// Concatenate c's transform onto ours.  When
// transforming colors, c's transform is applied
// first, then ours.
{
  m[0][1] += m[0][0] * c.m[0][1];
  m[1][1] += m[1][0] * c.m[1][1];
  m[2][1] += m[2][0] * c.m[2][1];
  m[3][1] += m[3][0] * c.m[3][1];

  m[0][0] *= c.m[0][0];
  m[1][0] *= c.m[1][0];
  m[2][0] *= c.m[2][0];
  m[3][0] *= c.m[3][0];
}

RGBA	CXForm::transform(RGBA& in) const
// Apply our transform to the given color; return the result.
{
  RGBA	result;

  result.r = (Uint8) Util::fclamp(in.r * m[0][0] + m[0][1], 0, 255);
  result.g = (Uint8) Util::fclamp(in.g * m[1][0] + m[1][1], 0, 255);
  result.b = (Uint8) Util::fclamp(in.b * m[2][0] + m[2][1], 0, 255);
  result.a = (Uint8) Util::fclamp(in.a * m[3][0] + m[3][1], 0, 255);

  return result;
}


void	CXForm::readRGB(Stream* in)
{
  in->align();

  int	has_add = in->readUInt(1);
  int	has_mult = in->readUInt(1);
  int	nbits = in->readUInt(4);

  if (has_mult) {
    m[0][0] = in->readSInt(nbits) / 255.0f;
    m[1][0] = in->readSInt(nbits) / 255.0f;
    m[2][0] = in->readSInt(nbits) / 255.0f;
    m[3][0] = 1;
  }
  else {
    for (int i = 0; i < 4; i++) { m[i][0] = 1; }
  }
  if (has_add) {
    m[0][1] = (float) in->readSInt(nbits);
    m[1][1] = (float) in->readSInt(nbits);
    m[2][1] = (float) in->readSInt(nbits);
    m[3][1] = 1;
  }
  else {
    for (int i = 0; i < 4; i++) { m[i][1] = 0; }
  }
}

void	CXForm::readRGBA(Stream* in)
{
  in->align();

  int	has_add = in->readUInt(1);
  int	has_mult = in->readUInt(1);
  int	nbits = in->readUInt(4);

  if (has_mult) {
    m[0][0] = in->readSInt(nbits) / 256.0f;
    m[1][0] = in->readSInt(nbits) / 256.0f;
    m[2][0] = in->readSInt(nbits) / 256.0f;
    m[3][0] = in->readSInt(nbits) / 256.0f;
  }
  else {
    for (int i = 0; i < 4; i++) { m[i][0] = 1; }
  }
  if (has_add) {
    m[0][1] = (float) in->readSInt(nbits);
    m[1][1] = (float) in->readSInt(nbits);
    m[2][1] = (float) in->readSInt(nbits);
    m[3][1] = (float) in->readSInt(nbits);
  }
  else {
    for (int i = 0; i < 4; i++) { m[i][1] = 0; }
  }
}

void CXForm::clamp()
// Force component values to be in legal range.
{
  m[0][0] = Util::fclamp(m[0][0], 0, 1);
  m[1][0] = Util::fclamp(m[1][0], 0, 1);
  m[2][0] = Util::fclamp(m[2][0], 0, 1);
  m[3][0] = Util::fclamp(m[3][0], 0, 1);

  m[0][1] = Util::fclamp(m[0][1], -255.0f, 255.0f);
  m[1][1] = Util::fclamp(m[1][1], -255.0f, 255.0f);
  m[2][1] = Util::fclamp(m[2][1], -255.0f, 255.0f);
  m[3][1] = Util::fclamp(m[3][1], -255.0f, 255.0f);
}


//void	CXForm::print() const
//// Debug log.
//{
//	log_msg("    *         +\n");
//	log_msg("| %4.4f %4.4f|\n", float(m[0][0]), float(m[0][1]));
//	log_msg("| %4.4f %4.4f|\n", float(m[1][0]), float(m[1][1]));
//	log_msg("| %4.4f %4.4f|\n", float(m[2][0]), float(m[2][1]));
//	log_msg("| %4.4f %4.4f|\n", float(m[3][0]), float(m[3][1]));
//}
