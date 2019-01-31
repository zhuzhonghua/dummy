#include "stdafx.h"

#include "basic_types.h"
#include "def.h"
#include "stream.h"

RGBA::RGBA() : 
_r(255), 
_g(255), 
_b(255), 
_a(255) 
{

}

RGBA::RGBA(UInt8 r, UInt8 g, UInt8 b, UInt8 a) :
_r(r), 
_g(g), 
_b(b), 
_a(a)
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
	_a = in->readUI8();
}

void RGBA::readRGB(Stream* in)
{
	_r = in->readUI8();
	_g = in->readUI8();
	_b = in->readUI8();

	_a = 0xFF;
}

void RGBA::set(UInt8 r, UInt8 g, UInt8 b, UInt8 a)
{
	_r = r;
	_g = g;
	_b = b;
	_a = a;
}

void RGBA::set(double color)
{
	int rgb = int(color);
	_r = (UInt8)(rgb >> 16) & 0xFF;
	_g = (UInt8)(rgb >> 8) & 0xFF;
	_b = (UInt8)rgb & 0xFF;
	_a = 0xFF;
}

void RGBA::setLerp(const RGBA& a, const RGBA& b, float f)
{
	_r = (UInt8)Util::frnd(Util::flerp(a._r, b._r, f));
	_g = (UInt8)Util::frnd(Util::flerp(a._g, b._g, f));
	_b = (UInt8)Util::frnd(Util::flerp(a._b, b._b, f));
	_a = (UInt8)Util::frnd(Util::flerp(a._a, b._a, f));
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

RGBA	CXForm::transform(const RGBA& in) const
// Apply our transform to the given color; return the result.
{
	RGBA	result;

	result._r = (Uint8) Util::fclamp(in._r * m[0][0] + m[0][1], 0, 255);
	result._g = (Uint8) Util::fclamp(in._g * m[1][0] + m[1][1], 0, 255);
	result._b = (Uint8) Util::fclamp(in._b * m[2][0] + m[2][1], 0, 255);
	result._a = (Uint8) Util::fclamp(in._a * m[3][0] + m[3][1], 0, 255);

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
