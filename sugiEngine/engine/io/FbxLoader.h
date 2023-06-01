#pragma once

#include "fbxsdk.h"

#include <d3d12.h>
#include <d3dx12.h>

#include <string>
#include "FbxModel.h"

class FbxLoader
{
public:
	/// <summary>
	/// シングルトンインスタンスの取得
	/// </summary>
	/// <returns>インスタンス</returns>
	static FbxLoader* GetInstance();

private:
	// privateなコンストラクタ（シングルトンパターン）
	FbxLoader() = default;
	// privateなデストラクタ（シングルトンパターン）
	~FbxLoader() = default;
	// コピーコンストラクタを禁止（シングルトンパターン）
	FbxLoader(const FbxLoader& obj) = delete;
	// コピー代入演算子を禁止（シングルトンパターン）
	void operator=(const FbxLoader& obj) = delete;

public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="device"></param>
	void Initialize(ID3D12Device* device);

	/// <summary>
	/// 後始末
	/// </summary>
	void Finalize();

	void LoadModelFromFile(const std::string& modelName);

	void ParseNodeRecursive(FbxModel* model,FbxNode* fbxNode,Node* parent = nullptr);

	void ParseMesh(FbxModel* model, FbxNode* fbxNode);
	void ParseMeshVertices(FbxModel* model, FbxMesh* fbxMesh);
	void ParseMeshFaces(FbxModel* model, FbxMesh* fbxMesh);
	void ParseMaterial(FbxModel* model,FbxNode* fbxNode);
	void LoadTexture(FbxModel* model, const std::string& fullpath);

	std::string ExtractFileName(const std::string& path);

public:
	//モデル格納ルートパス

	static const std::string baseDirectory;
	static const std::string defaultTextureFileName;

private:
	ID3D12Device* device = nullptr;

	FbxManager* fbxManager = nullptr;

	FbxImporter* fbxImporter = nullptr;
};