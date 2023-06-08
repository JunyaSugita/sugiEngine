#pragma once
#include <DirectXMath.h>
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "CircleShadow.h"

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
	static ID3D12Device* device_;

public:
	//平行光
	static const uint32_t DirLightNum = 3;
	//点光源
	static const uint32_t PointLightNum = 3;
	//スポットライト
	static const uint32_t SpotLightNum = 3;
	//丸影
	static const uint32_t CircleShadowNum = 1;

public:
	struct ConstBufferData
	{
		//環境光の色
		XMFLOAT3 ambientColor;
		float pad1;
		//平行光線用
		DirectionalLight::ConstBufferData dirLights[DirLightNum];
		//点光源
		PointLight::ConstBufferData pointLights[PointLightNum];
		//スポットライト
		SpotLight::ConstBufferData spotLights[SpotLightNum];
		//丸影
		CircleShadow::ConstBufferData circleShadows[CircleShadowNum];
	};

	static void StaticInitialize(ID3D12Device* device);

	static LightGroup* Create();

private:
	//定数バッファ
	ComPtr<ID3D12Resource>constBuff_;
	//環境光の色
	XMFLOAT3 ambientColor_ = { 1,1,1 };
	//平行光線の配列
	DirectionalLight dirLights_[DirLightNum];
	//点光源の配列
	PointLight pointLights_[PointLightNum];
	//点光源の配列
	SpotLight spotLights_[SpotLightNum];
	//丸影の配列
	CircleShadow circleShadows_[CircleShadowNum];
	//ダーティフラグ
	bool dirty_ = false;

public:
	void Initialize();

	void TransferConstBuffer();

	void Update();

	void DefaultLightSetting();

	void Draw(ID3D12GraphicsCommandList* cmdList, UINT rootParameterIndex);

	void SetAmbientColor(const XMFLOAT3& color);
	//平行光
	void SetDirLightActive(uint32_t index, bool active);
	void SetDirLightDir(uint32_t index,const XMVECTOR& lightdir);
	void SetDirLightColor(uint32_t index, const XMFLOAT3& lightcolor);
	//点光源
	void SetPointLightActive(uint32_t index, bool active);
	void SetPointLightPos(uint32_t index,const XMFLOAT3& lightpos);
	void SetPointLightColor(uint32_t index, const XMFLOAT3& lightcolor);
	void SetPointLightAtten(uint32_t index, const XMFLOAT3& lightAtten);
	//スポットライト
	void SetSpotLightActive(uint32_t index, bool active);
	void SetSpotLightDir(uint32_t index, const XMVECTOR& lightdir);
	void SetSpotLightPos(uint32_t index, const XMFLOAT3& lightpos);
	void SetSpotLightColor(uint32_t index, const XMFLOAT3& lightcolor);
	void SetSpotLightAtten(uint32_t index, const XMFLOAT3& lightAtten);
	void SetSpotLightFactorAngle(uint32_t index, const XMFLOAT2& lightFactorAngle);
	//丸影
	void SetCircleShadowActive(uint32_t index, bool active);
	void SetCircleShadowCasterPos(uint32_t index, const XMFLOAT3& casterPos);
	void SetCircleShadowDir(uint32_t index, const XMVECTOR& lightdir);
	void SetCircleShadowDistanceCaster(uint32_t index, float distanceCasterLight);
	void SetCircleShadowAtten(uint32_t index, const XMFLOAT3& lightAtten);
	void SetCircleShadowFactorAngle(uint32_t index, const XMFLOAT2& lightFactorAngle);
};

