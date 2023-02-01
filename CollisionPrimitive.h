#pragma once
#include <DirectXMath.h>

//
struct Sphere {
	//S
	DirectX::XMVECTOR center = { 0,0,0,1 };
	//ĵa
	float radius = 0.5f;
};

//½Ê
struct Plane {
	//@üv
	DirectX::XMVECTOR normal = { 0,1,0,0 };
	//´_(0,0,0)İçÌ£
	float distance = 0.0f;
};

class Triangle {
public:
	DirectX::XMVECTOR p0;
	DirectX::XMVECTOR p1;
	DirectX::XMVECTOR p2;

	DirectX::XMVECTOR normal;
};