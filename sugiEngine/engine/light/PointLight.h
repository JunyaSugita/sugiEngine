/**
 * @file PointLight.h
 * @brief 点光源
 */

#pragma once
#include<DirectXMath.h>
#include "SugiMath.h"

class PointLight
{
private:
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

public:
	struct ConstBufferData {
		XMFLOAT3 lightpos;
		float pad1;
		XMFLOAT3 lightcolor;
		float pad2;
		XMFLOAT3 lightatten;
		uint32_t active;
	};

public:
	//セッター
	inline void SetLightPos(const XMFLOAT3& lightpos) {
		lightpos_ = lightpos;
	}
	inline void SetLightColor(const Vector3& lightcolor) {
		lightcolor_.x = lightcolor.x;
		lightcolor_.y = lightcolor.y;
		lightcolor_.z = lightcolor.z;
	}
	inline void SetLightAtten(const Vector3& lightAtten) {
		lightAtten_.x = lightAtten.x;
		lightAtten_.y = lightAtten.y;
		lightAtten_.z = lightAtten.z;
	}
	inline void SetActive(bool active) {
		active_ = active;
	}
	
	//ゲッター
	inline const XMFLOAT3& GetLightPos() {
		return lightpos_;
	}
	inline const XMFLOAT3& GetLightColor() {
		return lightcolor_;
	}
	inline const XMFLOAT3& GetLightAtten() {
		return lightAtten_;
	}
	inline bool GetIsActive() {
		return active_;
	}

private:
	XMFLOAT3 lightpos_ = { 0,0,0 };
	XMFLOAT3 lightcolor_ = { 1,1,1 };
	XMFLOAT3 lightAtten_ = { 1.0f,1.0f,1.0f };
	bool active_ = false;

};

