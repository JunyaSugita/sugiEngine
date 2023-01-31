#pragma once
#include "CollisionPrimitive.h"

//“–‚½‚è”»’è
class Collision {
public:
	//‹…‚Æ•½–Ê
	static bool CheckSphere2Plane(const Sphere& sphere, const Plane& plane, DirectX::XMVECTOR* inter = nullptr);
};