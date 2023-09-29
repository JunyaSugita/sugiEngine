#pragma once
#include <Windows.h>
#include "DXCommon.h"
#include "GrovalSetting.h"
#include "WorldTransform.h"
#include "SugiMath.h"
#include <forward_list>
#include "ParticleEditor.h"

#include <DirectXMath.h>

struct Particle {
	Vector3 position = {};
	float scale = 1.0f;
	float s_scale = 1.0f;
	float e_scale = 1.0f;
	Vector3 velocity = {};
	Vector3 accel = {1,1,1};
	Vector3 gravity = {};
	int32_t frame = 0;
	int32_t num_frame = 0;
	Vector4 color = {0,1,0,1};
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
	P_END,
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

	void AddCircle(int life, Vector3 pos, Vector3 velo,Vector3 accel, Vector3 gravity,float start_scale,float end_scale,Vector4 color);
	void AddIce(int life, Vector3 pos, Vector3 velo, Vector3 accel, Vector3 gravity, float start_scale, float end_scale, Vector4 color);
	void Add(Vector3 pos, EditFile data);
	
	void LoadParticleData();

	void AddFromFile(uint8_t num, Vector3 pos);

	void Clear();
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
	std::forward_list<Particle> iceParticles_;

	EditFile particleData_[P_END];
};


