#include "Collision.h"

using namespace DirectX;

bool Collision::CheckSphere2Plane(const Sphere& sphere, const Plane& plane, XMVECTOR* inter)
{
	//座標系の原点から球の中心座標への距離
	XMVECTOR distV = XMVector3Dot(sphere.center, plane.normal);
	//平面の原点距離を減算することで、平面と球の中心との距離が出る
	float dist = distV.m128_f32[0] - plane.distance;
	//距離の絶対値が半径より大きいと当たってない
	if (fabsf(dist) > sphere.radius) {
		return false;
	}

	//疑似交点
	if (inter) {
		*inter = -dist * plane.normal + sphere.center;
	}

	return true;
}
