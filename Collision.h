#pragma once
#include "CollisionPrimitive.h"

//�����蔻��
class Collision {
public:
	//���ƕ���
	static bool CheckSphere2Plane(const Sphere& sphere, const Plane& plane, DirectX::XMVECTOR* inter = nullptr);
};