#pragma once
#include <DirectXMath.h>

//球
struct Sphere {
	//中心
	DirectX::XMVECTOR center = { 0,0,0,1 };
	//半径
	float radius = 0.5f;
};

//平面
struct Plane {
	//法線v
	DirectX::XMVECTOR normal = { 0,1,0,0 };
	//原点(0,0,0)からの距離
	float distance = 0.0f;
};

class Triangle {
public:
	DirectX::XMVECTOR p0;
	DirectX::XMVECTOR p1;
	DirectX::XMVECTOR p2;

	DirectX::XMVECTOR normal;
};

struct Ray {
	//始点
	DirectX::XMVECTOR start = { 0,0,0,1 };
	//方向
	DirectX::XMVECTOR dir = { 1,0,0,0 };
};