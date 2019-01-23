#pragma once

class Stream;
class Point;
class Matrix;

class RECT
{
public:
	RECT();
	void		read(Stream* in);

	bool		pointTest(float x, float y) const;
	bool		boundTest(const RECT& bound) const;
	void		setToPoint(float x, float y);
	void		setToPoint(const Point& p);
	void		expandToPoint(float x, float y);
	void		expandToPoint(const Point& p);
	void		expandToRect(const RECT& r);

	Point		getCorner(int i) const;

	void		encloseTransformedRect(const Matrix& m, const RECT& r);

	void		setLerp(const RECT& a, const RECT& b, float t);

	float		width() const { return _x_max - _x_min; }
	float		height() const { return _y_max - _y_min; }

	void		twipsToPixels();
	void		pixelsToTwips();

	float		xMin() { return _x_min; }
	float		xMax() { return _x_max; }
	float		yMin() { return _y_min; }
	float		yMax() { return _y_max; }
public:
	float	_x_min;
	float	_x_max;
	float	_y_min;
	float	_y_max;
};
