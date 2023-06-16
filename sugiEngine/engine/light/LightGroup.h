#pragma once
#include <DirectXMath.h>
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "CircleShadow.h"
#include <memory>

class LightGroup
{
	//�G�C���A�X
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
	//���s��
	static const int32_t sDirLightNum = 3;
	//�_����
	static const int32_t sPointLightNum = 3;
	//�X�|�b�g���C�g
	static const int32_t sSpotLightNum = 3;
	//�ۉe
	static const int32_t sCircleShadowNum = 1;

public:
	struct ConstBufferData
	{
		//�����̐F
		XMFLOAT3 ambientColor;
		float pad1;
		//���s�����p
		DirectionalLight::ConstBufferData dirLights[sDirLightNum];
		//�_����
		PointLight::ConstBufferData pointLights[sPointLightNum];
		//�X�|�b�g���C�g
		SpotLight::ConstBufferData spotLights[sSpotLightNum];
		//�ۉe
		CircleShadow::ConstBufferData circleShadows[sCircleShadowNum];
	};

	static void StaticInitialize(ID3D12Device* device);

	static std::unique_ptr<LightGroup> Create();

private:
	//�萔�o�b�t�@
	ComPtr<ID3D12Resource>constBuff_;
	//�����̐F
	XMFLOAT3 ambientColor_ = { 1,1,1 };
	//���s�����̔z��
	DirectionalLight dirLights_[sDirLightNum];
	//�_�����̔z��
	PointLight pointLights_[sPointLightNum];
	//�_�����̔z��
	SpotLight spotLights_[sSpotLightNum];
	//�ۉe�̔z��
	CircleShadow circleShadows_[sCircleShadowNum];
	//�_�[�e�B�t���O
	bool dirty_ = false;

public:
	void Initialize();

	void TransferConstBuffer();

	void Update();

	void DefaultLightSetting();

	void Draw(ID3D12GraphicsCommandList* cmdList, uint32_t rootParameterIndex);

	void SetAmbientColor(const XMFLOAT3& color);
	//���s��
	void SetDirLightActive(int32_t index, bool active);
	void SetDirLightDir(int32_t index,const XMVECTOR& lightdir);
	void SetDirLightColor(int32_t index, const XMFLOAT3& lightcolor);
	//�_����
	void SetPointLightActive(int32_t index, bool active);
	void SetPointLightPos(int32_t index,const XMFLOAT3& lightpos);
	void SetPointLightColor(int32_t index, const XMFLOAT3& lightcolor);
	void SetPointLightAtten(int32_t index, const XMFLOAT3& lightAtten);
	//�X�|�b�g���C�g
	void SetSpotLightActive(int32_t index, bool active);
	void SetSpotLightDir(int32_t index, const XMVECTOR& lightdir);
	void SetSpotLightPos(int32_t index, const XMFLOAT3& lightpos);
	void SetSpotLightColor(int32_t index, const XMFLOAT3& lightcolor);
	void SetSpotLightAtten(int32_t index, const XMFLOAT3& lightAtten);
	void SetSpotLightFactorAngle(int32_t index, const XMFLOAT2& lightFactorAngle);
	//�ۉe
	void SetCircleShadowActive(int32_t index, bool active);
	void SetCircleShadowCasterPos(int32_t index, const XMFLOAT3& casterPos);
	void SetCircleShadowDir(int32_t index, const XMVECTOR& lightdir);
	void SetCircleShadowDistanceCaster(int32_t index, float distanceCasterLight);
	void SetCircleShadowAtten(int32_t index, const XMFLOAT3& lightAtten);
	void SetCircleShadowFactorAngle(int32_t index, const XMFLOAT2& lightFactorAngle);
};

