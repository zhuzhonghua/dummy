#pragma once

class Stream;
class Point;
class RECT;

class Matrix
{
public:
	static Matrix	identity;

	Matrix();
	void	setIdentity();
	void	concatenate(const Matrix& m);
	void	concatenateTranslation(float tx, float ty);
	void	concatenateScale(float s);
	void	setLerp(const Matrix& m1, const Matrix& m2, float t);
	void	setScaleRotation(float x_scale, float y_scale, float rotation);
	void	read(Stream* in);
	void	transform(Point* result, const Point& p) const;
	void	transform(RECT* bound) const;
	void	transformVector(Point* result, const Point& p) const;
	void	transformByInverse(Point* result, const Point& p) const;
	void	transformByInverse(RECT* bound) const;
	void	setInverse(const Matrix& m);
	bool	doesFlip() const;	// return true if we flip handedness
	float	getDeterminant() const;	// determinant of the 2x2 rotation/scale part only
	float	getMaxScale() const;	// return the maximum scale factor that this transform applies
	float	getXScale() const;	// return the magnitude scale of our x coord output
	float	getYScale() const;	// return the magnitude scale of our y coord output
	float	getRotation() const;	// return our rotation component (in radians)

	bool operator==(const Matrix& m) const
	{
		return
			_m[0][0] == m._m[0][0] &&
			_m[0][1] == m._m[0][1] &&
			_m[0][2] == m._m[0][2] &&
			_m[1][0] == m._m[1][0] &&
			_m[1][1] == m._m[1][1] &&
			_m[1][2] == m._m[1][2];
	}

	bool operator!=(const Matrix& m) const
	{
		return !(*this == m);
	}
public:
	/*
		scalex  rorate1 translatex
		rotate0 scaley  translatey

		x_ = x * scalex + y * rotate1 + translatex
		y_ = x * rotate0 + y * scaley + translatey
	 */
	Float	_m[2][3];
};

