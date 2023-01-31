#pragma once
#include <DirectXMath.h>

//��
struct Sphere {
	//���S
	DirectX::XMVECTOR center = { 0,0,0,1 };
	//���a
	float radius = 0.5f;
};

//����
struct Plane {
	//�@��v
	DirectX::XMVECTOR normal = { 0,1,0,0 };
	//���_(0,0,0)����̋���
	float distance = 0.0f;
};