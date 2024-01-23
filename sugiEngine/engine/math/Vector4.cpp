#include "Vector4.h"

Vector4::Vector4()
{
}

Vector4::Vector4(float x, float y, float z, float w)
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
}

Vector4 Vector4::operator+() const {
	return *this;
}

Vector4 Vector4::operator-() const {
	return Vector4(-x, -y, -z, -w);
}

Vector4& Vector4::operator+=(const Vector4& v) {
	x += v.x;
	y += v.y;
	z += v.z;
	w += v.w;
	return *this;
}

Vector4& Vector4::operator-=(const Vector4& v) {
	x -= v.x;
	y -= v.y;
	z -= v.z;
	w -= v.w;
	return *this;
}

Vector4& Vector4::operator*=(float s) {
	x *= s;
	y *= s;
	z *= s;
	w *= s;
	return *this;
}

Vector4& Vector4::operator/=(float s) {
	x /= s;
	y /= s;
	z /= s;
	w /= s;
	return *this;
}

//Vector3 クラスに属さない関数群
//2項演算子
const Vector4 operator+(const Vector4& v1, const Vector4& v2) {
	Vector4  temp(v1);
	return temp += v2;
}

const Vector4 operator-(const Vector4& v1, const Vector4& v2) {
	Vector4 temp(v1);
	return temp -= v2;
}

const Vector4 operator*(const Vector4& v, float s) {
	Vector4 temp(v);
	return temp *= s;
}

const Vector4 operator*(float s, const Vector4& v) {
	return v * s;
}

const Vector4 operator/(const Vector4& v, float s) {
	Vector4 temp(v);
	temp.x /= s;
	temp.y /= s;
	temp.z /= s;
	temp.w /= s;
	return temp;
}