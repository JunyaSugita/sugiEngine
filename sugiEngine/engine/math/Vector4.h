/**
 * @file Vector4.h
 * @brief float x4
 */

#pragma once
class Vector4
{
public:
	float x;
	float y;
	float z;
	float w;

public:
	Vector4();
	Vector4(float x, float y, float z, float w);

	//単項演算子オーバーロード
	Vector4 operator+() const;
	Vector4 operator-() const;

	//代入演算子オーバーロード
	Vector4& operator+=(const Vector4& v);
	Vector4& operator-=(const Vector4& v);
	Vector4& operator*=(float s);
	Vector4& operator/=(float s);
};

//2項演算子オーバーロード
//いろんな引数のパターンに対応するためいっぱい準備
const Vector4 operator+(const Vector4& v1, const Vector4& v2);
const Vector4 operator-(const Vector4& v1, const Vector4& v2);
const Vector4 operator*(const Vector4& v, float s);
const Vector4 operator*(float s, const Vector4& v);
const Vector4 operator*(const Vector4& s, const Vector4& v);
const Vector4 operator/(const Vector4& v, float s);
