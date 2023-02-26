#pragma once

#include <math.h>
#include <Windows.h>

#define	PI	3.141592f

float DegreeToRadian(float Degree);
float RadianToDegree(float Radian);


typedef struct _tagVector2
{
	float	x, y;

	_tagVector2();
	_tagVector2(float _x, float _y);
	_tagVector2(const _tagVector2& v);
	_tagVector2(const POINT& v);

public:
	void operator = (const _tagVector2& v);
	void operator = (const POINT& v);
	void operator = (float Value);
	void operator = (int Value);

public:
	bool operator == (const _tagVector2& v);
	bool operator != (const _tagVector2& v);

public:
	_tagVector2 operator + (const _tagVector2& v)	const;
	_tagVector2 operator + (const POINT& v)	const;
	_tagVector2 operator + (float Value)	const;
	_tagVector2 operator + (int Value)	const;

public:
	_tagVector2 operator += (const _tagVector2& v);
	_tagVector2 operator += (const POINT& v);
	_tagVector2 operator += (float Value);
	_tagVector2 operator += (int Value);

public:
	_tagVector2 operator - (const _tagVector2& v)	const;
	_tagVector2 operator - (const POINT& v)	const;
	_tagVector2 operator - (float Value)	const;
	_tagVector2 operator - (int Value)	const;

public:
	_tagVector2 operator -= (const _tagVector2& v);
	_tagVector2 operator -= (const POINT& v);
	_tagVector2 operator -= (float Value);
	_tagVector2 operator -= (int Value);

public:
	_tagVector2 operator * (const _tagVector2& v)	const;
	_tagVector2 operator * (const POINT& v)	const;
	_tagVector2 operator * (float Value)	const;
	_tagVector2 operator * (int Value)	const;

public:
	_tagVector2 operator *= (const _tagVector2& v);
	_tagVector2 operator *= (const POINT& v);
	_tagVector2 operator *= (float Value);
	_tagVector2 operator *= (int Value);

public:
	_tagVector2 operator / (const _tagVector2& v)	const;
	_tagVector2 operator / (const POINT& v)	const;
	_tagVector2 operator / (float Value)	const;
	_tagVector2 operator / (int Value)	const;

public:
	_tagVector2 operator /= (const _tagVector2& v);
	_tagVector2 operator /= (const POINT& v);
	_tagVector2 operator /= (float Value);
	_tagVector2 operator /= (int Value);

public:
	_tagVector2 operator ++ ();
	_tagVector2 operator ++ (int);
	_tagVector2 operator -- ();
	_tagVector2 operator -- (int);

public:
	float Length()	const;
	void Normalize();
	float GetAngle(const _tagVector2& Target)	const;
	float Distance(const _tagVector2& Target)	const;
}Vector2, *PVector2;
