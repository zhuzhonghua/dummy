#include "stdafx.h"
#include "basic_types.h"
#include "matrix.h"

Matrix::Matrix()
{
	setIdentity();
}

void Matrix::setIdentity()
{
	memset(&_m[0], 0, sizeof(_m));
	_m[0][0] = 1;
	_m[1][1] = 1;
}

void Matrix::concatenate(const Matrix& m)
{
	Matrix	t;
	t._m[0][0] = _m[0][0] * m._m[0][0] + _m[0][1] * m._m[1][0];
	t._m[1][0] = _m[1][0] * m._m[0][0] + _m[1][1] * m._m[1][0];
	t._m[0][1] = _m[0][0] * m._m[0][1] + _m[0][1] * m._m[1][1];
	t._m[1][1] = _m[1][0] * m._m[0][1] + _m[1][1] * m._m[1][1];
	t._m[0][2] = _m[0][0] * m._m[0][2] + _m[0][1] * m._m[1][2] + _m[0][2];
	t._m[1][2] = _m[1][0] * m._m[0][2] + _m[1][1] * m._m[1][2] + _m[1][2];

	*this = t;
}

void Matrix::concatenateTranslation(float tx, float ty)
{
	_m[0][2] += _m[0][0] * tx + _m[0][1] * ty;
	_m[1][2] += _m[1][0] * tx + _m[1][1] * ty;
}

void Matrix::concatenateScale(float scale)
{
	_m[0][0] *= scale;
	_m[0][1] *= scale;
	_m[1][0] *= scale;
	_m[1][1] *= scale;
}

void Matrix::setLerp(const Matrix& m1, const Matrix& m2, float t)
{
	_m[0][0] = Util::flerp(m1._m[0][0], m2._m[0][0], t);
	_m[1][0] = Util::flerp(m1._m[1][0], m2._m[1][0], t);
	_m[0][1] = Util::flerp(m1._m[0][1], m2._m[0][1], t);
	_m[1][1] = Util::flerp(m1._m[1][1], m2._m[1][1], t);
	_m[0][2] = Util::flerp(m1._m[0][2], m2._m[0][2], t);
	_m[1][2] = Util::flerp(m1._m[1][2], m2._m[1][2], t);
}

void Matrix::setScaleRotation(float x_scale, float y_scale, float angle)
{
	float	cos_angle = cosf(angle);
	float	sin_angle = sinf(angle);
	_m[0][0] = x_scale * cos_angle;
	_m[0][1] = y_scale * -sin_angle;
	_m[1][0] = x_scale * sin_angle;
	_m[1][1] = y_scale * cos_angle;
}

void Matrix::read(Stream* in)
{
	in->align();

	setIdentity();

	int	has_scale = in->readUInt(1);
	if (has_scale)
	{
		int	scale_nbits = in->readUInt(5);
		_m[0][0] = in->readSInt(scale_nbits) / 65536.0f;
		_m[1][1] = in->readSInt(scale_nbits) / 65536.0f;
	}
	int	has_rotate = in->readUInt(1);
	if (has_rotate)
	{
		int	rotate_nbits = in->readUInt(5);
		_m[1][0] = in->readSInt(rotate_nbits) / 65536.0f;
		_m[0][1] = in->readSInt(rotate_nbits) / 65536.0f;
	}

	int	translate_nbits = in->readUInt(5);
	if (translate_nbits > 0)
	{
		_m[0][2] = (float)in->readSInt(translate_nbits);
		_m[1][2] = (float)in->readSInt(translate_nbits);
	}
}

void Matrix::transform(Point* result, const Point& p) const
{
	result->x = _m[0][0] * p.x + _m[0][1] * p.y + _m[0][2];
	result->y = _m[1][0] * p.x + _m[1][1] * p.y + _m[1][2];
}

void Matrix::transform(RECT* bound) const
{
	Point p[4];
	transform(p + 0, Point(bound->getCorner(0)));
	transform(p + 1, Point(bound->getCorner(1)));
	transform(p + 2, Point(bound->getCorner(2)));
	transform(p + 3, Point(bound->getCorner(3)));

	// Build bound that covers transformed bound
	bound->setToPoint(p[0]);
	bound->expandToPoint(p[1]);
	bound->expandToPoint(p[2]);
	bound->expandToPoint(p[3]);
}

void Matrix::transformVector(Point* result, const Point& v) const
{
	result->x = _m[0][0] * v.x + _m[0][1] * v.y;
	result->y = _m[1][0] * v.x + _m[1][1] * v.y;
}

void Matrix::transformByInverse(Point* result, const Point& p) const
{
	Matrix	m;
	m.setInverse(*this);
	m.transform(result, p);
}

void Matrix::transformByInverse(RECT* bound) const
{
	Matrix	m;
	m.setInverse(*this);
	m.transform(bound);
}

void Matrix::setInverse(const Matrix& m)
{
	// Invert the rotation part.
	float	det = m._m[0][0] * m._m[1][1] - m._m[0][1] * m._m[1][0];
	if (det == 0.0f)
	{
		// Not invertible.
		//assert(0);	// castano: this happens sometimes! (ie. sample6.swf)

		// Arbitrary fallback.
		setIdentity();
		_m[0][2] = -m._m[0][2];
		_m[1][2] = -m._m[1][2];
	}
	else
	{
		float	inv_det = 1.0f / det;
		_m[0][0] = m._m[1][1] * inv_det;
		_m[1][1] = m._m[0][0] * inv_det;
		_m[0][1] = -m._m[0][1] * inv_det;
		_m[1][0] = -m._m[1][0] * inv_det;

		_m[0][2] = -(_m[0][0] * m._m[0][2] + _m[0][1] * m._m[1][2]);
		_m[1][2] = -(_m[1][0] * m._m[0][2] + _m[1][1] * m._m[1][2]);
	}
}

bool Matrix::doesFlip() const
{
	float	det = _m[0][0] * _m[1][1] - _m[0][1] * _m[1][0];

	return det < 0.0f;
}

float Matrix::getDeterminant() const
{
	return _m[0][0] * _m[1][1] - _m[1][0] * _m[0][1];
}

float Matrix::getMaxScale() const
{
	float	basis0_length2 = _m[0][0] * _m[0][0] + _m[0][1] * _m[0][1];
	float	basis1_length2 = _m[1][0] * _m[1][0] + _m[1][1] * _m[1][1];
	float	max_length2 = fmax(basis0_length2, basis1_length2);
	return Util::sqr(max_length2);
}

float Matrix::getXScale() const
{
	float scale = Util::sqr(_m[0][0] * _m[0][0] + _m[1][0] * _m[1][0]);

	// Are we turned inside out?
	if (getDeterminant() < 0.f)
	{
		scale = -scale;
	}

	return scale;
}

float Matrix::getYScale() const
{
	return Util::sqr(_m[1][1] * _m[1][1] + _m[0][1] * _m[0][1]);
}

float Matrix::getRotation() const
{
	if (getDeterminant() < 0.f)
	{
		// We're turned inside out; negate the
		// x-component used to compute rotation.
		//
		// Matches get_x_scale().
		//
		// @@ this may not be how Macromedia does it!  Test this!
		return atan2f(_m[1][0], -_m[0][0]);
	}
	else
	{
		return atan2f(_m[1][0], _m[0][0]);
	}
}
