#include "stdafx.h"
#include "stream.h"
#include "rect.h"
#include "point.h"
#include "matrix.h"

RECT::RECT()
{
	_x_min = 0;
	_x_max = 0;
	_y_min = 0;
	_y_max = 0;
}

void RECT::read(Stream* in)
{
	in->align();

	int	nbits = in->readUInt(5);
	_x_min = (float)in->readSInt(nbits);
	_x_max = (float)in->readSInt(nbits);
	_y_min = (float)in->readSInt(nbits);
	_y_max = (float)in->readSInt(nbits);
}

void RECT::twipsToPixels()
{
	_x_min = TWIPS_TO_PIXELS(_x_min);
	_x_max = TWIPS_TO_PIXELS(_x_max);
	_y_min = TWIPS_TO_PIXELS(_y_min);
	_y_max = TWIPS_TO_PIXELS(_y_max);
}

void RECT::pixelsToTwips()
{
	_x_min = PIXELS_TO_TWIPS(_x_min);
	_x_max = PIXELS_TO_TWIPS(_x_max);
	_y_min = PIXELS_TO_TWIPS(_y_min);
	_y_max = PIXELS_TO_TWIPS(_y_max);
}

bool RECT::pointTest(float x, float y) const
{
	if (x < _x_min
		|| x > _x_max
		|| y < _y_min
		|| y > _y_max)
	{
		return false;
	}
	else
	{
		return true;
	}
}

bool RECT::boundTest(const RECT& bound) const
{
	if (_x_max < bound._x_min
		|| _x_min > bound._x_max
		|| _y_min > bound._y_max
		|| _y_max < bound._y_min)
	{
		return false;
	}
	else
	{
		return true;
	}
}

void RECT::setToPoint(float x, float y)
{
	_x_min = _x_max = x;
	_y_min = _y_max = y;
}

void RECT::setToPoint(const Point& p)
{
	setToPoint(p.x, p.y);
}

void RECT::expandToPoint(float x, float y)
{
	_x_min = fmin(_x_min, x);
	_y_min = fmin(_y_min, y);
	_x_max = fmax(_x_max, x);
	_y_max = fmax(_y_max, y);
}

void RECT::expandToPoint(const Point& p)
{
	expandToPoint(p.x, p.y);
}

void RECT::expandToRect(const RECT& r)
{
	expandToPoint(r._x_min, r._y_min);
	expandToPoint(r._x_max, r._y_max);
}

Point RECT::getCorner(int i) const
{
	ASSERT(i >= 0 && i < 4);
	return Point((i == 0 || i == 3) ? _x_min : _x_max,
				(i < 2) ? _y_min : _y_max);
}

void RECT::encloseTransformedRect(const Matrix& m, const RECT& r)
{
	// Get the transformed bounding box.
	Point	p0, p1, p2, p3;
	m.transform(&p0, r.getCorner(0));
	m.transform(&p1, r.getCorner(1));
	m.transform(&p2, r.getCorner(2));
	m.transform(&p3, r.getCorner(3));

	// Vitaly: It is necessary to test the case when p0 is not min(p1, p2, p3)
	_x_min = _x_max = p0.x;
	_y_min = _y_max = p0.y;
	expandToPoint(p1.x, p1.y);
	expandToPoint(p2.x, p2.y);
	expandToPoint(p3.x, p3.y);
}

void RECT::setLerp(const RECT& a, const RECT& b, float t)
{
	_x_min = Util::flerp(a._x_min, b._x_min, t);
	_y_min = Util::flerp(a._y_min, b._y_min, t);
	_x_max = Util::flerp(a._x_max, b._x_max, t);
	_y_max = Util::flerp(a._y_max, b._y_max, t);
}
