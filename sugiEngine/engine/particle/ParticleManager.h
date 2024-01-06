/**
 * @file ParticleManager.h
 * @brief パーティクルを管理するマネージャー
 */

#pragma once
#include <Windows.h>
#include "DXCommon.h"
#include "GlobalSetting.h"
#include "WorldTransform.h"
#include "SugiMath.h"
#include <forward_list>
#include "ParticleEditor.h"

#include <DirectXMath.h>

struct Particle {
	int32_t frame = 0;
	int32_t num_frame = 0;
	Vector3 originPos = {};
	Vector3 position = {};
	bool isRevers_ = false;
	Vector3 velocity = {};
	float speed = 1.0f;
	Vector3 accel = { 1,1,1 };
	float scale = 1.0f;
	float s_scale = 1.0f;
	float e_scale = 1.0f;
	Vector3 gravity = {};
	Vector3 color = { 1,1,1 };
	Vector3 s_color = { 1,1,1 };
	Vector3 e_color = { 1,1,1 };
	int32_t postEffect = 0;
};

enum ParticleName {
	P_FIRE_BALL,
	P_FIRE_BALL_EXPLODE,
	P_ICE,
	P_MAGIC_MISSILE,
	P_LIGHTNING,
	P_WEAPON,
	P_WEAPON_FIRE,
	P_DEBUFF_FIRE,
	P_GOAL,
	P_TORCH,
	P_DAMAGE,
	P_CHARGE_FIRE,
	P_CHARGE_MAX_FIRE,
	P_BACKGROUND,
	P_ICE_BREAK,
	P_DEBUFF_ICE,

	P_END
};

class ParticleManager final
{
private:
	ParticleManager() = default;
	~ParticleManager() = default;

public:
	//コピーコンストラクタ無効
	ParticleManager(const ParticleManager& obj) = delete;
	//代入演算子を無効
	ParticleManager& operator=(const ParticleManager& obj) = delete;

	static ParticleManager* GetInstance();


public:
	//エイリアステンプレート
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	struct VertexSp {
		DirectX::XMFLOAT3 pos;
		float scale;
		DirectX::XMFLOAT4 color;
	};

	struct ConstBuffB1 {
		DirectX::XMMATRIX mat;
		DirectX::XMMATRIX billboard;
	};

public:
	static void StaticInitialize(ID3D12Device* device);

	static void PreDraw(ID3D12GraphicsCommandList* cmdList);

	static void PostDraw();

	static uint32_t LoadTexture(std::string file);

protected:
	//デバイス
	static ComPtr<ID3D12Device> sDevice;
	static ComPtr<ID3D12PipelineState> sPipelineState;
	// ルートシグネチャ
	static ComPtr<ID3D12RootSignature> sRootSignature;
	// 頂点バッファビューの作成

	static ComPtr<ID3D12GraphicsCommandList> sCmdList;

	//SRVの最大数
	static const size_t MAX_SRV_COUNT = 2056;
	static std::array<ComPtr<ID3D12Resource>, MAX_SRV_COUNT> sTextureBuffers;

	static ComPtr<ID3D12DescriptorHeap> sSrvHeap;
	static uint32_t sIncrementSize;
	static uint32_t sTextureIndex;

	static const uint32_t vertexCount = 2048 * 8;

private:
	void AdjustTextureSize();

public:
	void Initialize();
	void Update();
	void Draw();
	void Finalize();

	void SetPos(float x, float y);

	void SetRotate(float r);

	void SetColor(float x, float y, float z, float w);

	void SetSize(float x, float y);

	void SetAnchorPoint(float x, float y);

	void SetFlipX(bool isFlip);
	void SetFlipY(bool isFlip);

	void SetIsView(bool is) {
		isView_ = is;
	}

	void SetUpVertex();

	Vector2 GetTextureSize() {
		return textureSize_;
	}

	void SetTextureSize(float x, float y);

	void AddCircle(int life, Vector3 pos, bool isRevers, Vector3 velo,float speed, Vector3 accel, Vector3 gravity,float start_scale,float end_scale,Vector3 sColor, Vector3 eColor, int32_t postEffect);
	void Add(Vector3 pos, EditFile data);
	void AddEditScaleAndColor(Vector3 pos, EditFile data,float scale,Vector3 color);

	void AddFromFile(uint8_t num, Vector3 pos, bool isEdit = false);
	void AddFromFileEditScaleAndColor(uint8_t num, Vector3 pos, float scale ,Vector3 color,bool isEdit = false);

	void Clear();

	void SetParticleData(int num, EditFile file) {
		particleData_[num] = file;
	}

	bool GetIsEdit() {
		return particleE_->GetIsEdit(0);
	}
protected:
	D3D12_HEAP_PROPERTIES heapProp_{}; // ヒープ設定
	D3D12_RESOURCE_DESC resDesc_{};
	ComPtr<ID3D12Resource> vertBuff_ = nullptr;
	ID3D12Resource* constBuffTransform_ = nullptr;
	ConstBuffB1* constMapTransform_ = nullptr;
	WorldTransform worldTransform_;
	Vector3 pos_;
	float rotate_;
	DirectX::XMFLOAT4 color_ = { 1,1,1,1 };
	Vector2 size_ = { 100.0f,100.0f };
	Vector2 anchorPoint_ = { 0.5f,0.5f };
	bool isFlipX_ = false;
	bool isFlipY_ = false;
	bool isView_ = true;
	VertexSp vertices_[vertexCount];
	D3D12_VERTEX_BUFFER_VIEW vbView_;
	uint32_t textureNum_;

	Vector2 textureLeftTop_ = { 0.0f,0.0f };
	Vector2 textureSize_ = { 100.0f,100.0f };

	std::forward_list<Particle> circleParticles_;

	EditFile particleData_[100];

	std::unique_ptr<ParticleEditor> particleE_ = nullptr;
};


