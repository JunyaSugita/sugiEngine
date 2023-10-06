#include "Matrix4.h"

using namespace DirectX;

Matrix4::Matrix4() {
	for (int32_t i = 0; i < 4; i++) {
		for (int32_t j = 0; j < 4; j++) {
			m[i][j] = 0;
		}
	}
}

Matrix4::Matrix4(float m00, float m01, float m02, float m03, float m10, float m11, float m12, float m13, float m20, float m21, float m22, float m23, float m30, float m31, float m32, float m33) {
	m[0][0] = m00;
	m[0][1] = m01;
	m[0][2] = m02;
	m[0][3] = m03;
	m[1][0] = m10;
	m[1][1] = m11;
	m[1][2] = m12;
	m[1][3] = m13;
	m[2][0] = m20;
	m[2][1] = m21;
	m[2][2] = m22;
	m[2][3] = m23;
	m[3][0] = m30;
	m[3][1] = m31;
	m[3][2] = m32;
	m[3][3] = m33;
}

void Matrix4::Initialize()
{
	for (int32_t i = 0; i < 4; i++) {
		for (int32_t j = 0; j < 4; j++) {
			if (i == j) {
				m[i][j] = 1;
			}
			else {
				m[i][j] = 0;
			}
		}
	}
}

Matrix4& Matrix4::operator*=(const Matrix4& m2)
{
	Matrix4 ans;
	for (int32_t i = 0; i < 4; i++) {
		for (int32_t j = 0; j < 4; j++) {
			for (int32_t k = 0; k < 4; k++) {
				ans.m[i][j] += m[i][k] * m2.m[k][j];
			}
		}
	}

	*this = ans;
	return *this;
}


Matrix4 Matrix4::operator*(const Matrix4& m2)const
{
	Matrix4 ans;
	for (int32_t i = 0; i < 4; i++) {
		for (int32_t j = 0; j < 4; j++) {
			for (int32_t k = 0; k < 4; k++) {
				ans.m[i][j] += m[i][k] * m2.m[k][j];
			}
		}
	}

	return ans;
}

Matrix4 ConvertToMatrix4(const XMMATRIX& xmmatrix)
{
	return Matrix4(
		xmmatrix.r[0].m128_f32[0], xmmatrix.r[0].m128_f32[1], xmmatrix.r[0].m128_f32[2], xmmatrix.r[0].m128_f32[3],
		xmmatrix.r[1].m128_f32[0], xmmatrix.r[1].m128_f32[1], xmmatrix.r[1].m128_f32[2], xmmatrix.r[1].m128_f32[3],
		xmmatrix.r[2].m128_f32[0], xmmatrix.r[2].m128_f32[1], xmmatrix.r[2].m128_f32[2], xmmatrix.r[2].m128_f32[3],
		xmmatrix.r[3].m128_f32[0], xmmatrix.r[3].m128_f32[1], xmmatrix.r[3].m128_f32[2], xmmatrix.r[3].m128_f32[3]
	);
}

XMMATRIX ConvertToXMMATRIX(const Matrix4& matrix4)
{
	return XMMATRIX(
		matrix4.m[0][0], matrix4.m[0][1], matrix4.m[0][2], matrix4.m[0][3],
		matrix4.m[1][0], matrix4.m[1][1], matrix4.m[1][2], matrix4.m[1][3],
		matrix4.m[2][0], matrix4.m[2][1], matrix4.m[2][2], matrix4.m[2][3],
		matrix4.m[3][0], matrix4.m[3][1], matrix4.m[3][2], matrix4.m[3][3]
	);
}
