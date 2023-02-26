#include "Vector2.h"

float DegreeToRadian(float Degree)
{
	return Degree * PI / 180.f;
}

float RadianToDegree(float Radian)
{
	return Radian * 180.f / PI;
}

_tagVector2::_tagVector2()	:
	x(0.f),
	y(0.f)
{
}

_tagVector2::_tagVector2(float _x, float _y)	:
	x(_x),
	y(_y)
{
}

_tagVector2::_tagVector2(const _tagVector2& v)	:
	x(v.x),
	y(v.y)
{
}

_tagVector2::_tagVector2(const POINT& v) :
	x((float)v.x),
	y((float)v.y)
{
}

void _tagVector2::operator=(const _tagVector2& v)
{
	x = v.x;
	y = v.y;
}

void _tagVector2::operator=(const POINT& v)
{
	x = (float)v.x;
	y = (float)v.y;
}

void _tagVector2::operator=(float Value)
{
	x = Value;
	y = Value;
}

void _tagVector2::operator=(int Value)
{
	x = (float)Value;
	y = (float)Value;
}

bool _tagVector2::operator==(const _tagVector2& v)
{
	if (x == v.x && y == v.y)
		return true;

	return false;
}

bool _tagVector2::operator!=(const _tagVector2& v)
{
	if (x != v.x || y != v.y)
		return true;

	return false;
}

// + 
_tagVector2 _tagVector2::operator+(const _tagVector2& v) const
{
	_tagVector2	result;
	result.x = x + v.x;
	result.y = y + v.y;

	return result;
}

_tagVector2 _tagVector2::operator+(const POINT& v) const
{
	_tagVector2	result;
	result.x = x + (float)v.x;
	result.y = y + (float)v.y;

	return result;
}

_tagVector2 _tagVector2::operator+(float Value) const
{
	_tagVector2	result;
	result.x = x + Value;
	result.y = y + Value;

	return result;
}

_tagVector2 _tagVector2::operator+(int Value) const
{
	_tagVector2	result;
	result.x = x + (float)Value;
	result.y = y + (float)Value;

	return result;
}

_tagVector2 _tagVector2::operator+=(const _tagVector2& v)
{
	x += v.x;
	y += v.y;

	return *this;
}

_tagVector2 _tagVector2::operator+=(const POINT& v)
{
	x += (float)v.x;
	y += (float)v.y;

	return *this;
}

_tagVector2 _tagVector2::operator+=(float Value)
{
	x += Value;
	y += Value;

	return *this;
}

_tagVector2 _tagVector2::operator+=(int Value)
{
	x += (float)Value;
	y += (float)Value;

	return *this;
}



// -

_tagVector2 _tagVector2::operator-(const _tagVector2& v) const
{
	_tagVector2	result;
	result.x = x - v.x;
	result.y = y - v.y;

	return result;
}

_tagVector2 _tagVector2::operator-(const POINT& v) const
{
	_tagVector2	result;
	result.x = x - (float)v.x;
	result.y = y - (float)v.y;

	return result;
}

_tagVector2 _tagVector2::operator-(float Value) const
{
	_tagVector2	result;
	result.x = x - Value;
	result.y = y - Value;

	return result;
}

_tagVector2 _tagVector2::operator-(int Value) const
{
	_tagVector2	result;
	result.x = x - (float)Value;
	result.y = y - (float)Value;

	return result;
}

_tagVector2 _tagVector2::operator-=(const _tagVector2& v)
{
	x -= v.x;
	y -= v.y;

	return *this;
}

_tagVector2 _tagVector2::operator-=(const POINT& v)
{
	x -= (float)v.x;
	y -= (float)v.y;

	return *this;
}

_tagVector2 _tagVector2::operator-=(float Value)
{
	x -= Value;
	y -= Value;

	return *this;
}

_tagVector2 _tagVector2::operator-=(int Value)
{
	x -= (float)Value;
	y -= (float)Value;

	return *this;
}



// *

_tagVector2 _tagVector2::operator*(const _tagVector2& v) const
{
	_tagVector2	result;
	result.x = x * v.x;
	result.y = y * v.y;

	return result;
}

_tagVector2 _tagVector2::operator*(const POINT& v) const
{
	_tagVector2	result;
	result.x = x * (float)v.x;
	result.y = y * (float)v.y;

	return result;
}

_tagVector2 _tagVector2::operator*(float Value) const
{
	_tagVector2	result;
	result.x = x * Value;
	result.y = y * Value;

	return result;
}

_tagVector2 _tagVector2::operator*(int Value) const
{
	_tagVector2	result;
	result.x = x * (float)Value;
	result.y = y * (float)Value;

	return result;
}

_tagVector2 _tagVector2::operator*=(const _tagVector2& v)
{
	x *= v.x;
	y *= v.y;

	return *this;
}

_tagVector2 _tagVector2::operator*=(const POINT& v)
{
	x *= (float)v.x;
	y *= (float)v.y;

	return *this;
}

_tagVector2 _tagVector2::operator*=(float Value)
{
	x *= Value;
	y *= Value;

	return *this;
}

_tagVector2 _tagVector2::operator*=(int Value)
{
	x *= (float)Value;
	y *= (float)Value;

	return *this;
}



// /

_tagVector2 _tagVector2::operator/(const _tagVector2& v) const
{
	_tagVector2	result;
	result.x = x / v.x;
	result.y = y / v.y;

	return result;
}

_tagVector2 _tagVector2::operator/(const POINT& v) const
{
	_tagVector2	result;
	result.x = x / (float)v.x;
	result.y = y / (float)v.y;

	return result;
}

_tagVector2 _tagVector2::operator/(float Value) const
{
	_tagVector2	result;
	result.x = x / Value;
	result.y = y / Value;

	return result;
}

_tagVector2 _tagVector2::operator/(int Value) const
{
	_tagVector2	result;
	result.x = x / (float)Value;
	result.y = y / (float)Value;

	return result;
}

_tagVector2 _tagVector2::operator/=(const _tagVector2& v)
{
	x /= v.x;
	y /= v.y;

	return *this;
}

_tagVector2 _tagVector2::operator/=(const POINT& v)
{
	x /= (float)v.x;
	y /= (float)v.y;

	return *this;
}

_tagVector2 _tagVector2::operator/=(float Value)
{
	x /= Value;
	y /= Value;

	return *this;
}

_tagVector2 _tagVector2::operator/=(int Value)
{
	x /= (float)Value;
	y /= (float)Value;

	return *this;
}

_tagVector2 _tagVector2::operator++()
{
	x += 1.f;
	y += 1.f;

	return *this;
}

_tagVector2 _tagVector2::operator++(int)
{
	x += 1.f;
	y += 1.f;

	return *this;
}

_tagVector2 _tagVector2::operator--()
{
	x -= 1.f;
	y -= 1.f;

	return *this;
}

_tagVector2 _tagVector2::operator--(int)
{
	x -= 1.f;
	y -= 1.f;

	return *this;
}

float _tagVector2::Length() const
{
	return sqrtf(x * x + y * y);
}

void _tagVector2::Normalize()
{
	float	fLength = Length();

	x /= fLength;
	y /= fLength;
}

float _tagVector2::GetAngle(const _tagVector2& Target) const
{
	Vector2	vLength = Target - *this;

	float	Dist = sqrtf(vLength.x * vLength.x + vLength.y * vLength.y);
	float	Width = vLength.x;

	float	Angle = Width / Dist;

	Angle = RadianToDegree(acosf(Angle));

	if (Target.y < y)
		Angle = 360.f - Angle;

	return Angle;
}

float _tagVector2::Distance(const _tagVector2& Target) const
{
	_tagVector2	Result = *this - Target;

	return sqrtf(Result.x * Result.x + Result.y * Result.y);
}


