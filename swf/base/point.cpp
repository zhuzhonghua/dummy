#include "stdafx.h"
#include "point.h"

bool Point::bitwiseEqual(const Point& p) const
{
	return memcmp(this, &p, sizeof(p)) == 0;
}

float Point::getLength() const
{
	return Util::sqr(x * x + y * y);
}

void Point::twipsToPixels()
{
	x = TWIPS_TO_PIXELS(x);
	y = TWIPS_TO_PIXELS(y);
}

void Point::pixelsToTwips()
{
	x = PIXELS_TO_TWIPS(x);
	y = PIXELS_TO_TWIPS(y);
}
