#pragma once
#include "CollisionPrimitive.h"

//当たり判定
class Collision {
public:
	//球と平面
	static bool CheckSphere2Plane(const Sphere& sphere, const Plane& plane, DirectX::XMVECTOR* inter = nullptr);
};