#include "LightGroup.h"
#include <assert.h>

using namespace DirectX;

ID3D12Device* LightGroup::device = nullptr;

void LightGroup::StaticInitialize(ID3D12Device* device)
{
	assert(!LightGroup::device);
	assert(device);

	LightGroup::device = device;
}

LightGroup* LightGroup::Create()
{
	LightGroup* instance = new LightGroup();

	instance->Initialize();

	return instance;
}

void LightGroup::Initialize()
{
	HRESULT result;

	DefaultLightSetting();

	//ヒープ
	D3D12_HEAP_PROPERTIES cbHeapProp{};
	cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;		//GPUへの転送用
	//リソース
	D3D12_RESOURCE_DESC cbResourceDesc{};
	cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	cbResourceDesc.Width = (sizeof(ConstBufferData) + 0xff) & ~0xff;	//256バイトアライメント
	cbResourceDesc.Height = 1;
	cbResourceDesc.DepthOrArraySize = 1;
	cbResourceDesc.MipLevels = 1;
	cbResourceDesc.SampleDesc.Count = 1;
	cbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	//定数バッファ
	result = device->CreateCommittedResource(
		&cbHeapProp,		//ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&cbResourceDesc,	//リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuff_)
	);
	assert(SUCCEEDED(result));

	TransferConstBuffer();
}

void LightGroup::TransferConstBuffer()
{
	HRESULT result;

	ConstBufferData* constMap = nullptr;
	result = constBuff_->Map(0, nullptr, (void**)&constMap);
	if (SUCCEEDED(result)) {
		constMap->ambientColor = ambientColor_;
		//平行光
		for (int i = 0; i < DirLightNum; i++) {
			if (dirLights_[i].GetIsActive()) {
				constMap->dirLights[i].active = 1;
				constMap->dirLights[i].lightv = -dirLights_[i].GetLightDir();
				constMap->dirLights[i].lightcolor = dirLights_[i].GetLightColor();
			}
			else {
				constMap->dirLights[i].active = 0;
			}
		}
		//点光源
		for (int i = 0; i < PointLightNum; i++) {
			if (pointLights_[i].GetIsActive()) {
				constMap->pointLights[i].active = 1;
				constMap->pointLights[i].lightpos = pointLights_[i].GetLightPos();
				constMap->pointLights[i].lightcolor = pointLights_[i].GetLightColor();
				constMap->pointLights[i].lightatten = pointLights_[i].GetLightAtten();
			}
			else {
				constMap->pointLights[i].active = 0;
			}
		}
		//スポットライト
		for (int i = 0; i < SpotLightNum; i++) {
			if (spotLights_[i].GetIsActive()) {
				constMap->spotLights[i].active = 1;
				constMap->spotLights[i].lightv = -spotLights_[i].GetLightDir();
				constMap->spotLights[i].lightpos = spotLights_[i].GetLightPos();
				constMap->spotLights[i].lightcolor = spotLights_[i].GetLightColor();
				constMap->spotLights[i].lightatten = spotLights_[i].GetLightAtten();
				constMap->spotLights[i].lightfactoranglecos = spotLights_[i].GetLightFactorAngleCos();
			}
			else {
				constMap->spotLights[i].active = 0;
			}
		}
		//丸影
		for (int i = 0; i < CircleShadowNum; i++) {
			if (circleShadows_[i].GetIsActive()) {
				constMap->circleShadows[i].active = 1;
				constMap->circleShadows[i].dir = -circleShadows_[i].GetDir();
				constMap->circleShadows[i].casterPos = circleShadows_[i].GetCasterPos();
				constMap->circleShadows[i].distanceCasterLight = circleShadows_[i].GetDistanceCasterLight();
				constMap->circleShadows[i].atten = circleShadows_[i].GetAtten();
				constMap->circleShadows[i].factorAngleCos = circleShadows_[i].GetFactorAngleCos();
			}
			else {
				constMap->circleShadows[i].active = 0;
			}
		}

		constBuff_->Unmap(0, nullptr);
	}
}

void LightGroup::Update()
{
	if (dirty_) {
		TransferConstBuffer();
		dirty_ = false;
	}
}

void LightGroup::DefaultLightSetting()
{
	dirLights_[0].SetActive(true);
	dirLights_[0].SetLightColor({ 1.0f, 1.0f, 1.0f });
	dirLights_[0].SetLightDir({ 0.0f,-1.0f,0.0f,0 });

	dirLights_[1].SetActive(true);
	dirLights_[1].SetLightColor({ 1.0f,1.0f, 1.0f });
	dirLights_[1].SetLightDir({ +0.5f, +0.1f,+0.2f,0 });

	dirLights_[2].SetActive(true);
	dirLights_[2].SetLightColor({ 1.0f,1.0f, 1.0f });
	dirLights_[2].SetLightDir({ -0.5f,+0.1f,-0.2f,0 });
}

void LightGroup::Draw(ID3D12GraphicsCommandList* cmdList, UINT rootParameterIndex)
{
	cmdList->SetGraphicsRootConstantBufferView(rootParameterIndex, constBuff_->GetGPUVirtualAddress());
}

void LightGroup::SetAmbientColor(const XMFLOAT3& color)
{
	ambientColor_ = color;
	dirty_ = true;
}

void LightGroup::SetDirLightActive(int index, bool active)
{
	assert(0 <= index && index < DirLightNum);
	dirLights_[index].SetActive(active);
}

void LightGroup::SetDirLightDir(int index, const XMVECTOR& lightdir)
{
	assert(0 <= index && index < DirLightNum);
	dirLights_[index].SetLightDir(lightdir);
	dirty_ = true;
}

void LightGroup::SetDirLightColor(int index, const XMFLOAT3& lightcolor)
{
	assert(0 <= index && index < DirLightNum);
	dirLights_[index].SetLightColor(lightcolor);
	dirty_ = true;
}

void LightGroup::SetPointLightActive(int index, bool active)
{
	assert(0 <= index && index < PointLightNum);
	pointLights_[index].SetActive(active);
}

void LightGroup::SetPointLightPos(int index, const XMFLOAT3& lightpos)
{
	assert(0 <= index && index < PointLightNum);
	pointLights_[index].SetLightPos(lightpos);
	dirty_ = true;
}

void LightGroup::SetPointLightColor(int index, const XMFLOAT3& lightcolor)
{
	assert(0 <= index && index < PointLightNum);
	pointLights_[index].SetLightColor(lightcolor);
	dirty_ = true;
}

void LightGroup::SetPointLightAtten(int index, const XMFLOAT3& lightAtten)
{
	assert(0 <= index && index < PointLightNum);
	pointLights_[index].SetLightAtten(lightAtten);
	dirty_ = true;
}

void LightGroup::SetSpotLightActive(int index, bool active)
{
	assert(0 <= index && index < SpotLightNum);
	spotLights_[index].SetActive(active);
}

void LightGroup::SetSpotLightDir(int index, const XMVECTOR& lightdir)
{
	assert(0 <= index && index < SpotLightNum);
	spotLights_[index].SetLightDir(lightdir);
	dirty_ = true;
}

void LightGroup::SetSpotLightPos(int index, const XMFLOAT3& lightpos)
{
	assert(0 <= index && index < SpotLightNum);
	spotLights_[index].SetLightPos(lightpos);
	dirty_ = true;
}

void LightGroup::SetSpotLightColor(int index, const XMFLOAT3& lightcolor)
{
	assert(0 <= index && index < SpotLightNum);
	spotLights_[index].SetLightColor(lightcolor);
	dirty_ = true;
}

void LightGroup::SetSpotLightAtten(int index, const XMFLOAT3& lightAtten)
{
	assert(0 <= index && index < SpotLightNum);
	spotLights_[index].SetLightAtten(lightAtten);
	dirty_ = true;
}

void LightGroup::SetSpotLightFactorAngle(int index, const XMFLOAT2& lightFactorAngle)
{
	assert(0 <= index && index < SpotLightNum);
	spotLights_[index].SetLightFactorAngle(lightFactorAngle);
	dirty_ = true;
}

void LightGroup::SetCircleShadowActive(int index, bool active)
{
	assert(0 <= index && index < CircleShadowNum);
	circleShadows_[index].SetActive(active);
}

void LightGroup::SetCircleShadowCasterPos(int index, const XMFLOAT3& casterPos)
{
	assert(0 <= index && index < CircleShadowNum);
	circleShadows_[index].SetCasterPos(casterPos);
	dirty_ = true;
}

void LightGroup::SetCircleShadowDir(int index, const XMVECTOR& lightdir)
{
	assert(0 <= index && index < CircleShadowNum);
	circleShadows_[index].SetDir(lightdir);
	dirty_ = true;
}

void LightGroup::SetCircleShadowDistanceCaster(int index, float distanceCasterLight)
{
	assert(0 <= index && index < CircleShadowNum);
	circleShadows_[index].SetDistanceCasterLight(distanceCasterLight);
	dirty_ = true;
}

void LightGroup::SetCircleShadowAtten(int index, const XMFLOAT3& lightAtten)
{
	assert(0 <= index && index < CircleShadowNum);
	circleShadows_[index].SetAtten(lightAtten);
	dirty_ = true;
}

void LightGroup::SetCircleShadowFactorAngle(int index, const XMFLOAT2& lightFactorAngle)
{
	assert(0 <= index && index < CircleShadowNum);
	circleShadows_[index].SetFactorAngle(lightFactorAngle);
	dirty_ = true;
}


