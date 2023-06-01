#pragma once
#include <string>
#include <DirectXMath.h>
#include <vector>
#include <DirectXTex.h>
#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <d3dx12.h>

struct Node
{
	std::string name;

	DirectX::XMVECTOR scaling = { 1,1,1,0 };
	DirectX::XMVECTOR rotation = { 0,0,0,0 };
	DirectX::XMVECTOR  translation = { 0,0,0,1 };
	DirectX::XMMATRIX transform;
	DirectX::XMMATRIX globalTransform;

	//親ノード
	Node* parent = nullptr;
};

class FbxModel
{
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	using TexMetadata = DirectX::TexMetadata;
	using ScratchImage = DirectX::ScratchImage;
	using string = std::string;
	template <class T> using vector = std::vector<T>;

public:
	struct VertexPosNormalUv {
		XMFLOAT3 pos;
		XMFLOAT3 normal;
		XMFLOAT2 uv;
	};

public:
	friend class FbxLoader;

public:
	void CreateBuffers(ID3D12Device* device);
	void Draw(ID3D12GraphicsCommandList* cmdList);

	const XMMATRIX& GetModelTransform() {
		return meshNode->globalTransform;
	}

private:
	string name;
	//ノード配列
	vector<Node> nodes;

	Node* meshNode = nullptr;
	vector<VertexPosNormalUv> vertices;
	vector<unsigned short> indices;

	//マテリアル
	XMFLOAT3 ambient = { 1,1,1 };
	XMFLOAT3 diffuse = { 1,1,1 };
	TexMetadata metadata = {};
	ScratchImage scratchImg = {};

	ComPtr<ID3D12Resource> vertBuff;
	ComPtr<ID3D12Resource> indexBuff;
	ComPtr<ID3D12Resource> texBuff;
	D3D12_VERTEX_BUFFER_VIEW vbView = {};
	D3D12_INDEX_BUFFER_VIEW ibView = {};
	ComPtr <ID3D12DescriptorHeap> descHeapSRV;
};

