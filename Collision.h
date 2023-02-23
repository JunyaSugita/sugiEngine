#pragma once
#include "CollisionPrimitive.h"

//当たり判定
class Collision {
public:
	
	/// <summary>
	/// 球と平面
	/// </summary>
	/// <param name="sphere">球</param>
	/// <param name="plane">平面</param>
	/// <param name="inter">交点</param>
	/// <returns>交差しているか</returns>
	static bool CheckSphere2Plane(const Sphere& sphere, const Plane& plane, DirectX::XMVECTOR* inter = nullptr);

	/// <summary>
	/// 点と三角形
	/// </summary>
	/// <param name="point">点</param>
	/// <param name="triangle">三角形</param>
	/// <param name="closest">接点</param>
	static void ClosestPtPoint2Triangle(const DirectX::XMVECTOR& point, const Triangle& triangle, DirectX::XMVECTOR* closest);

	/// <summary>
	/// 球と法線付き三角形
	/// </summary>
	/// <param name="sphere">球</param>
	/// <param name="triangle">三角形</param>
	/// <param name="inter">交点</param>
	/// <returns>交差しているか</returns>
	static bool CheckSphere2Triangle(const Sphere& sphere, const Triangle& triangle, DirectX::XMVECTOR* inter = nullptr);

	/// <summary>
	/// レイと平面
	/// </summary>
	/// <param name="ray">レイ</param>
	/// <param name="plane">平面</param>
	/// <param name="distance">距離</param>
	/// <param name="inter">交点</param>
	/// <returns>交差しているか</returns>
	static bool CheckRay2Plane(const Ray& ray, const Plane& plane, float* distance, DirectX::XMVECTOR* inter = nullptr);

	/// <summary>
	/// レイと法線付き三角形の当たり判定
	/// </summary>
	/// <param name="ray">レイ</param>
	/// <param name="triangle">三角形</param>
	/// <param name="distance">距離</param>
	/// <param name="inter">交点</param>
	/// <returns>交差しているか</returns>
	static bool ChackRay2Triangle(const Ray& ray, const Triangle& triangle, float* distance = nullptr, DirectX::XMVECTOR* inter = nullptr);

	/// <summary>
	/// レイと球の当たり判定
	/// </summary>
	/// <param name="ray">レイ</param>
	/// <param name="sphere">球</param>
	/// <param name="distance">距離</param>
	/// <param name="inter">交点</param>
	/// <returns>交差しているか</returns>
	static bool CheckRay2Sphere(const Ray& ray, const Sphere& sphere, float* distance = nullptr, DirectX::XMVECTOR* inter = nullptr);
};