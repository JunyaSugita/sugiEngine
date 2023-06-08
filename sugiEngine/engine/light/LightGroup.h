#pragma once
#include <DirectXMath.h>
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "CircleShadow.h"

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
	static ID3D12Device* device_;

public:
	//���s��
	static const uint32_t DirLightNum = 3;
	//�_����
	static const uint32_t PointLightNum = 3;
	//�X�|�b�g���C�g
	static const uint32_t SpotLightNum = 3;
	//�ۉe
	static const uint32_t CircleShadowNum = 1;

public:
	struct ConstBufferData
	{
		//�����̐F
		XMFLOAT3 ambientColor;
		float pad1;
		//���s�����p
		DirectionalLight::ConstBufferData dirLights[DirLightNum];
		//�_����
		PointLight::ConstBufferData pointLights[PointLightNum];
		//�X�|�b�g���C�g
		SpotLight::ConstBufferData spotLights[SpotLightNum];
		//�ۉe
		CircleShadow::ConstBufferData circleShadows[CircleShadowNum];
	};

	static void StaticInitialize(ID3D12Device* device);

	static LightGroup* Create();

private:
	//�萔�o�b�t�@
	ComPtr<ID3D12Resource>constBuff_;
	//�����̐F
	XMFLOAT3 ambientColor_ = { 1,1,1 };
	//���s�����̔z��
	DirectionalLight dirLights_[DirLightNum];
	//�_�����̔z��
	PointLight pointLights_[PointLightNum];
	//�_�����̔z��
	SpotLight spotLights_[SpotLightNum];
	//�ۉe�̔z��
	CircleShadow circleShadows_[CircleShadowNum];
	//�_�[�e�B�t���O
	bool dirty_ = false;

public:
	void Initialize();

	void TransferConstBuffer();

	void Update();

	void DefaultLightSetting();

	void Draw(ID3D12GraphicsCommandList* cmdList, UINT rootParameterIndex);

	void SetAmbientColor(const XMFLOAT3& color);
	//���s��
	void SetDirLightActive(uint32_t index, bool active);
	void SetDirLightDir(uint32_t index,const XMVECTOR& lightdir);
	void SetDirLightColor(uint32_t index, const XMFLOAT3& lightcolor);
	//�_����
	void SetPointLightActive(uint32_t index, bool active);
	void SetPointLightPos(uint32_t index,const XMFLOAT3& lightpos);
	void SetPointLightColor(uint32_t index, const XMFLOAT3& lightcolor);
	void SetPointLightAtten(uint32_t index, const XMFLOAT3& lightAtten);
	//�X�|�b�g���C�g
	void SetSpotLightActive(uint32_t index, bool active);
	void SetSpotLightDir(uint32_t index, const XMVECTOR& lightdir);
	void SetSpotLightPos(uint32_t index, const XMFLOAT3& lightpos);
	void SetSpotLightColor(uint32_t index, const XMFLOAT3& lightcolor);
	void SetSpotLightAtten(uint32_t index, const XMFLOAT3& lightAtten);
	void SetSpotLightFactorAngle(uint32_t index, const XMFLOAT2& lightFactorAngle);
	//�ۉe
	void SetCircleShadowActive(uint32_t index, bool active);
	void SetCircleShadowCasterPos(uint32_t index, const XMFLOAT3& casterPos);
	void SetCircleShadowDir(uint32_t index, const XMVECTOR& lightdir);
	void SetCircleShadowDistanceCaster(uint32_t index, float distanceCasterLight);
	void SetCircleShadowAtten(uint32_t index, const XMFLOAT3& lightAtten);
	void SetCircleShadowFactorAngle(uint32_t index, const XMFLOAT2& lightFactorAngle);
};

