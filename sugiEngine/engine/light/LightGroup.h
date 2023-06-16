#pragma once
#include <DirectXMath.h>
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "CircleShadow.h"
#include <memory>

class LightGroup
{
	//エイリアス
private:
	template<class T>using ComPtr = Microsoft::WRL::ComPtr<T>;
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

private:
	static ID3D12Device* sDevice;

public:
	//平行光
	static const int32_t sDirLightNum = 3;
	//点光源
	static const int32_t sPointLightNum = 3;
	//スポットライト
	static const int32_t sSpotLightNum = 3;
	//丸影
	static const int32_t sCircleShadowNum = 1;

public:
	struct ConstBufferData
	{
		//環境光の色
		XMFLOAT3 ambientColor;
		float pad1;
		//平行光線用
		DirectionalLight::ConstBufferData dirLights[sDirLightNum];
		//点光源
		PointLight::ConstBufferData pointLights[sPointLightNum];
		//スポットライト
		SpotLight::ConstBufferData spotLights[sSpotLightNum];
		//丸影
		CircleShadow::ConstBufferData circleShadows[sCircleShadowNum];
	};

	static void StaticInitialize(ID3D12Device* device);

	static std::unique_ptr<LightGroup> Create();

private:
	//定数バッファ
	ComPtr<ID3D12Resource>constBuff_;
	//環境光の色
	XMFLOAT3 ambientColor_ = { 1,1,1 };
	//平行光線の配列
	DirectionalLight dirLights_[sDirLightNum];
	//点光源の配列
	PointLight pointLights_[sPointLightNum];
	//点光源の配列
	SpotLight spotLights_[sSpotLightNum];
	//丸影の配列
	CircleShadow circleShadows_[sCircleShadowNum];
	//ダーティフラグ
	bool dirty_ = false;

public:
	void Initialize();

	void TransferConstBuffer();

	void Update();

	void DefaultLightSetting();

	void Draw(ID3D12GraphicsCommandList* cmdList, uint32_t rootParameterIndex);

	void SetAmbientColor(const XMFLOAT3& color);
	//平行光
	void SetDirLightActive(int32_t index, bool active);
	void SetDirLightDir(int32_t index,const XMVECTOR& lightdir);
	void SetDirLightColor(int32_t index, const XMFLOAT3& lightcolor);
	//点光源
	void SetPointLightActive(int32_t index, bool active);
	void SetPointLightPos(int32_t index,const XMFLOAT3& lightpos);
	void SetPointLightColor(int32_t index, const XMFLOAT3& lightcolor);
	void SetPointLightAtten(int32_t index, const XMFLOAT3& lightAtten);
	//スポットライト
	void SetSpotLightActive(int32_t index, bool active);
	void SetSpotLightDir(int32_t index, const XMVECTOR& lightdir);
	void SetSpotLightPos(int32_t index, const XMFLOAT3& lightpos);
	void SetSpotLightColor(int32_t index, const XMFLOAT3& lightcolor);
	void SetSpotLightAtten(int32_t index, const XMFLOAT3& lightAtten);
	void SetSpotLightFactorAngle(int32_t index, const XMFLOAT2& lightFactorAngle);
	//丸影
	void SetCircleShadowActive(int32_t index, bool active);
	void SetCircleShadowCasterPos(int32_t index, const XMFLOAT3& casterPos);
	void SetCircleShadowDir(int32_t index, const XMVECTOR& lightdir);
	void SetCircleShadowDistanceCaster(int32_t index, float distanceCasterLight);
	void SetCircleShadowAtten(int32_t index, const XMFLOAT3& lightAtten);
	void SetCircleShadowFactorAngle(int32_t index, const XMFLOAT2& lightFactorAngle);
};

