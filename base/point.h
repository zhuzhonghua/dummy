#pragma once

class Point
{
public:
	Point() : x(0), y(0) {}
	Point(float x, float y) : x(x), y(y) {}

	void	setLerp(const Point& a, const Point& b, float t)
		// Set to a + (b - a) * t
	{
		x = a.x + (b.x - a.x) * t;
		y = a.y + (b.y - a.y) * t;
	}

	bool operator==(const Point& p) const { return x == p.x && y == p.y; }

	bool	bitwiseEqual(const Point& p) const;

	float getLength() const;

	void twipsToPixels();
	void pixelsToTwips();

	float	x, y;
};
