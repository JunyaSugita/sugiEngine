#pragma once

#include <DirectXMath.h>

class Object3d;
class BaseCollider;

struct  CollisionInfo{
public:
	CollisionInfo(Object3d* object, BaseCollider* sollider, const DirectX::XMVECTOR& inter) {
		this->object = object;
		this->collider = collider;
		this->inter = inter;
	}

	Object3d* object = nullptr;

	BaseCollider* collider = nullptr;

	DirectX::XMVECTOR inter;
};