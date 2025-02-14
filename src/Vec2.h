#pragma once
#include <cmath>


struct Vec2
{
	float x;
	float y;

	void operator+=(const Vec2& b)
	{
		x += b.x;
		y += b.y;
	}
	void operator-=(const Vec2& b)
	{
		x -= b.x;
		y -= b.y;
	}
	void operator-=(float b)
	{
		x -= b;
		y -= b;
	}
	void operator+=(float b)
	{
		x += b;
		y += b;
	}

	Vec2 operator*(int b)
	{
		return { x * b, y * b };
	}
	Vec2 operator*(const Vec2& b)
	{
		return { x * b.x, y * b.y };
	}

	Vec2 operator/(float b)
	{
		return { x / b, y / b };
	}

	Vec2 operator-(int b)
	{
		return { x - b, y - b };
	}
	Vec2 operator-(const Vec2& b)
	{
		return { x - b.x, y - b.y };
	}
	Vec2 operator-()
	{
		return { -x, -y };
	}

	Vec2 operator+(const Vec2& b)
	{
		return { x + b.x, y + b.y };
	}
	Vec2 operator+(float b)
	{
		return { x + b, y + b };
	}

	inline float mag();
};

static float dot(const Vec2& a, const Vec2& b)
{
	return a.x * b.x + a.y * b.y;
}

static float mag(const Vec2& v)
{
	return sqrt(dot(v, v));
}

float Vec2::mag()
{
	return sqrt(dot(*this, *this));
}

static Vec2 norm(Vec2 v)
{
	return v / mag(v);
}