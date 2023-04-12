#pragma once
#include <DirectXMath.h>

class SpotLight {
private:
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

public:
	struct ConstBufferData
	{
		XMVECTOR lightv;
		XMFLOAT3 lightpos;
		float pad1;
		XMFLOAT3 lightcolor;
		float pad2;
		XMFLOAT3 lightatten;
		float pad3;
		XMFLOAT2 lightfactoranglecos;
		unsigned int active;
		float pad4;
	};
public:
	inline void SetLightDir(const XMVECTOR& lightdir) {
		lightdir_ = DirectX::XMVector3Normalize(lightdir);
	}
	inline void SetLightPos(const XMFLOAT3& lightpos) {
		lightpos_ = lightpos;
	}
	inline void SetLightColor(const XMFLOAT3& lightcolor) {
		lightcolor_ = lightcolor;
	}
	inline void SetLightAtten(const XMFLOAT3& lightAtten) {
		lightAtten_ = lightAtten;
	}
	inline void SetLightFactorAngle(const XMFLOAT2& lightFactorAngle) {
		lightFactorAngleCos_.x = cosf(DirectX::XMConvertToRadians(lightFactorAngle.x));
		lightFactorAngleCos_.y = cosf(DirectX::XMConvertToRadians(lightFactorAngle.y));
	}
	inline void SetActive(bool active) {
		active_ = active;
	}

	inline const XMVECTOR& GetLightDir() {
		return lightdir_;
	}
	inline const XMFLOAT3& GetLightPos() {
		return lightpos_;
	}
	inline const XMFLOAT3& GetLightColor() {
		return lightcolor_;
	}
	inline const XMFLOAT3& GetLightAtten() {
		return lightAtten_;
	}
	inline const XMFLOAT2& GetLightFactorAngleCos() {
		return lightFactorAngleCos_;
	}
	inline bool GetIsActive() {
		return active_;
	}


private:
	XMVECTOR lightdir_ = { 1,0,0,0 };
	XMFLOAT3 lightpos_ = { 0,0,0 };
	XMFLOAT3 lightcolor_ = { 1,1,1 };
	XMFLOAT3 lightAtten_ = { 1.0f,1.0f,1.0f };
	XMFLOAT2 lightFactorAngleCos_ = { 0.5f,0.2f };
	bool active_ = false;
};