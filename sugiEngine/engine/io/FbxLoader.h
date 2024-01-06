/**
 * @file FbxLoader.h
 * @brief FBXファイルの読み込み
 */

#pragma once

#include "fbxsdk.h"

#include <d3d12.h>
#include <d3dx12.h>

#include <string>
#include "FbxModel.h"

class FbxLoader final
{
public:
	/// <summary>
	/// シングルトンインスタンスの取得
	/// </summary>
	/// <returns>インスタンス</returns>
	static FbxLoader* GetInstance();

	static void ConvertMatrixFromFbx(DirectX::XMMATRIX* dst, const FbxAMatrix& src);

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

	std::unique_ptr<FbxModel> LoadModelFromFile(const std::string& modelName);

	void ParseNodeRecursive(FbxModel* model,FbxNode* fbxNode,Node* parent = nullptr);

	void ParseMesh(FbxModel* model, FbxNode* fbxNode);
	void ParseMeshVertices(FbxModel* model, FbxMesh* fbxMesh);
	void ParseMeshFaces(FbxModel* model, FbxMesh* fbxMesh);
	void ParseMaterial(FbxModel* model,FbxNode* fbxNode);
	void LoadTexture(FbxModel* model, const std::string& fullpath);
	void ParseSkin(FbxModel* model, FbxMesh* fbxMesh);

	std::string ExtractFileName(const std::string& path);

public:
	//モデル格納ルートパス

	static const std::string sBaseDirectory;
	static const std::string sDefaultTextureFileName;

private:
	ID3D12Device* device_ = nullptr;

	FbxManager* fbxManager_ = nullptr;

	FbxImporter* fbxImporter_ = nullptr;
};
