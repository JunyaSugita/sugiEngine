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
	static ID3D12Device* device;

public:
	//平行光
	static const int DirLightNum = 3;
	//点光源
	static const int PointLightNum = 3;
	//スポットライト
	static const int SpotLightNum = 3;
	//丸影
	static const int CircleShadowNum = 1;

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
	void SetDirLightActive(int index, bool active);
	void SetDirLightDir(int index,const XMVECTOR& lightdir);
	void SetDirLightColor(int index, const XMFLOAT3& lightcolor);
	//点光源
	void SetPointLightActive(int index, bool active);
	void SetPointLightPos(int index,const XMFLOAT3& lightpos);
	void SetPointLightColor(int index, const XMFLOAT3& lightcolor);
	void SetPointLightAtten(int index, const XMFLOAT3& lightAtten);
	//スポットライト
	void SetSpotLightActive(int index, bool active);
	void SetSpotLightDir(int index, const XMVECTOR& lightdir);
	void SetSpotLightPos(int index, const XMFLOAT3& lightpos);
	void SetSpotLightColor(int index, const XMFLOAT3& lightcolor);
	void SetSpotLightAtten(int index, const XMFLOAT3& lightAtten);
	void SetSpotLightFactorAngle(int index, const XMFLOAT2& lightFactorAngle);
	//丸影
	void SetCircleShadowActive(int index, bool active);
	void SetCircleShadowCasterPos(int index, const XMFLOAT3& casterPos);
	void SetCircleShadowDir(int index, const XMVECTOR& lightdir);
	void SetCircleShadowDistanceCaster(int index, float distanceCasterLight);
	void SetCircleShadowAtten(int index, const XMFLOAT3& lightAtten);
	void SetCircleShadowFactorAngle(int index, const XMFLOAT2& lightFactorAngle);
};

