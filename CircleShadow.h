#pragma once
#include <DirectXMath.h>

class CircleShadow {
private:
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

public:
	struct ConstBufferData
	{
		XMVECTOR dir;
		XMFLOAT3 casterPos;
		float distanceCasterLight;
		XMFLOAT3 atten;
		float pad3;
		XMFLOAT2 factorAngleCos;
		unsigned int active;
		float pad4;
	};

public:
	inline void SetDir(const XMVECTOR& dir) {
		dir_ = DirectX::XMVector3Normalize(dir);
	}
	inline void SetCasterPos(const XMFLOAT3& casterPos) {
		casterPos_ = casterPos;
	}
	inline void SetDistanceCasterLight(const float distanceCasterLight) {
		distanceCasterLight_ = distanceCasterLight;
	}
	inline void SetAtten(const XMFLOAT3& atten) {
		atten_ = atten;
	}
	inline void SetFactorAngle(const XMFLOAT2& factorAngle) {
		factorAngleCos_.x = cosf(DirectX::XMConvertToRadians(factorAngle.x));
		factorAngleCos_.y = cosf(DirectX::XMConvertToRadians(factorAngle.y));
	}
	inline void SetActive(bool active) {
		active_ = active;
	}

	inline const XMVECTOR& GetDir() {
		return dir_;
	}
	inline const XMFLOAT3& GetCasterPos() {
		return casterPos_;
	}
	inline float GetDistanceCasterLight() {
		return distanceCasterLight_;
	}
	inline const XMFLOAT3& GetAtten() {
		return atten_;
	}
	inline const XMFLOAT2& GetFactorAngleCos() {
		return factorAngleCos_;
	}
	inline bool GetIsActive() {
		return active_;
	}


private:
	XMVECTOR dir_ = { 1,0,0,0 };
	float distanceCasterLight_ = 100.0f;
	XMFLOAT3 casterPos_ = { 0,0,0 };
	XMFLOAT3 atten_ = { 0.5f,0.6f,0.0f };
	XMFLOAT2 factorAngleCos_ = { 0.2f,0.5f };
	bool active_ = false;
};