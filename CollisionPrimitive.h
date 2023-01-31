#pragma once
#include <DirectXMath.h>

//‹…
struct Sphere {
	//’†S
	DirectX::XMVECTOR center = { 0,0,0,1 };
	//”¼Œa
	float radius = 0.5f;
};

//•½–Ê
struct Plane {
	//–@üv
	DirectX::XMVECTOR normal = { 0,1,0,0 };
	//Œ´“_(0,0,0)‚©‚ç‚Ì‹——£
	float distance = 0.0f;
};