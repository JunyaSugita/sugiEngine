#include "Particle.h"
#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")
#include <array>
#include "Camera.h"
#include "ParticleEditor.h"
#include "SpellManager.h"
#include <random>

using namespace Microsoft::WRL;
using namespace std;
using namespace DirectX;

ComPtr<ID3D12Device> Particle::sDevice = nullptr;
ComPtr<ID3D12PipelineState> Particle::sPipelineState = nullptr;
ComPtr<ID3D12RootSignature> Particle::sRootSignature;
ComPtr<ID3D12GraphicsCommandList> Particle::sCmdList;
std::array<ComPtr<ID3D12Resource>, Particle::MAX_SRV_COUNT> Particle::sTextureBuffers;
const size_t Particle::MAX_SRV_COUNT;
ComPtr<ID3D12DescriptorHeap> Particle::sSrvHeap;
uint32_t Particle::sIncrementSize;
uint32_t Particle::sTextureIndex = 0;

void Particle::StaticInitialize(ID3D12Device* device)
{
	HRESULT result;
	sDevice = device;

	ComPtr<ID3DBlob> vsBlob = nullptr; // 頂点シェーダオブジェクト
	ComPtr<ID3DBlob> gsBlob = nullptr;
	ComPtr<ID3DBlob> psBlob = nullptr; // ピクセルシェーダオブジェクト
	ComPtr<ID3DBlob> errorBlob = nullptr; // エラーオブジェクト
	// 頂点シェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"Resources/Shaders/ParticleVS.hlsl", // シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		"main", "vs_5_0", // エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0,
		&vsBlob, &errorBlob);
	// エラーなら
	if (FAILED(result)) {
		// errorBlobからエラー内容をstring型にコピー
		std::string error;
		error.resize(errorBlob->GetBufferSize());
		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			error.begin());
		error += "\n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(error.c_str());
		assert(0);
	}
	// ジオメトリシェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"Resources/Shaders/ParticleGS.hlsl", // シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		"main", "gs_5_0", // エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0,
		&gsBlob, &errorBlob);
	// エラーなら
	if (FAILED(result)) {
		// errorBlobからエラー内容をstring型にコピー
		std::string error;
		error.resize(errorBlob->GetBufferSize());
		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			error.begin());
		error += "\n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(error.c_str());
		assert(0);
	}
	// ピクセルシェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"Resources/Shaders/ParticlePS.hlsl", // シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		"main", "ps_5_0", // エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0,
		&psBlob, &errorBlob);
	// エラーなら
	if (FAILED(result)) {
		// errorBlobからエラー内容をstring型にコピー
		std::string error;
		error.resize(errorBlob->GetBufferSize());
		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			error.begin());
		error += "\n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(error.c_str());
		assert(0);
	}

	// 頂点レイアウト
	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
	{//xyz座標
		"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
		D3D12_APPEND_ALIGNED_ELEMENT,
		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
	},
	{//uv座標
		"TEXCOORD", 0, DXGI_FORMAT_R32_FLOAT, 0,
		D3D12_APPEND_ALIGNED_ELEMENT,
		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
	},
	{//uv座標
		"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,
		D3D12_APPEND_ALIGNED_ELEMENT,
		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
	}
	};

	// グラフィックスパイプライン設定
	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc{};
	// シェーダーの設定
	pipelineDesc.VS.pShaderBytecode = vsBlob->GetBufferPointer();
	pipelineDesc.VS.BytecodeLength = vsBlob->GetBufferSize();
	pipelineDesc.GS.pShaderBytecode = gsBlob->GetBufferPointer();
	pipelineDesc.GS.BytecodeLength = gsBlob->GetBufferSize();
	pipelineDesc.PS.pShaderBytecode = psBlob->GetBufferPointer();
	pipelineDesc.PS.BytecodeLength = psBlob->GetBufferSize();

	// サンプルマスクの設定
	pipelineDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // 標準設定
	// ラスタライザの設定
	pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE; // カリングしない
	pipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID; // ポリゴン内塗りつぶし
	pipelineDesc.RasterizerState.DepthClipEnable = true; // 深度クリッピングを有効に
	// ブレンドステート
	//pipelineDesc.BlendState.RenderTarget[0].RenderTargetWriteMask
	//	= D3D12_COLOR_WRITE_ENABLE_ALL; // RBGA全てのチャンネルを描画

	//レンダーターゲットのブレンド設定
	D3D12_RENDER_TARGET_BLEND_DESC& blenddesc = pipelineDesc.BlendState.RenderTarget[0];
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;	//RGBA全てのチャンネルを描画

	//共通設定(アルファ値)
	blenddesc.BlendEnable = true;					//ブレンドを有効にする
	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;	//加算
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;		//ソースの値を100%使う
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;	//デストの値を0%使う

	//加算合成
	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;			//加算
	blenddesc.SrcBlend = D3D12_BLEND_ONE;		//ソースのアルファ値
	blenddesc.DestBlend = D3D12_BLEND_ONE;//1.0f-1ソースのアルファ値

	for (int i = 0; i < MULTI_RENDAR_TARGET_NUM; i++) {
		// ブレンドステートの設定
		pipelineDesc.BlendState.RenderTarget[i] = blenddesc;
	}

	// 頂点レイアウトの設定
	pipelineDesc.InputLayout.pInputElementDescs = inputLayout;
	pipelineDesc.InputLayout.NumElements = _countof(inputLayout);
	// 図形の形状設定
	pipelineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT;

	// その他の設定
	pipelineDesc.NumRenderTargets = MULTI_RENDAR_TARGET_NUM; // 描画対象は8つ
	for (int i = 0; i < MULTI_RENDAR_TARGET_NUM; i++) {
		pipelineDesc.RTVFormats[i] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 0~255指定のRGBA
	}
	pipelineDesc.SampleDesc.Count = 1; // 1ピクセルにつき1回サンプリング

	//デスクリプタレンジの設定
	D3D12_DESCRIPTOR_RANGE descriptorRange{};
	descriptorRange.NumDescriptors = 1;
	descriptorRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descriptorRange.BaseShaderRegister = 0;//t0
	descriptorRange.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	//ルートパラメータ
	D3D12_ROOT_PARAMETER rootParams[3] = {};
	//定数バッファ0番
	rootParams[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//定数バッファビュー
	rootParams[0].Descriptor.ShaderRegister = 0;					//定数バッファ番号
	rootParams[0].Descriptor.RegisterSpace = 0;						//デフォルト値
	rootParams[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//全てのシェーダーから
	//テクスチャレジスタ
	rootParams[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;	//種類
	rootParams[1].DescriptorTable.pDescriptorRanges = &descriptorRange;			//デスクリプタレンジ
	rootParams[1].DescriptorTable.NumDescriptorRanges = 1;						//デスクリプタレンジ数
	rootParams[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;				//全てのシェーダーから
	//定数バッファ1番
	rootParams[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//定数バッファビュー
	rootParams[2].Descriptor.ShaderRegister = 1;					//定数バッファ番号
	rootParams[2].Descriptor.RegisterSpace = 0;						//デフォルト値
	rootParams[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	//テクスチャサンプラーの設定
	D3D12_STATIC_SAMPLER_DESC samplerDesc{};
	samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;					//横繰り返し(タイリング)
	samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;					//縦繰り返し(タイリング)
	samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;					//奥行繰り返し(タイリング)
	samplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;	//ボーダーの時は黒
	samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;					//全てリニア補間
	samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;									//ミップマップ最大値
	samplerDesc.MinLOD = 0.0f;												//ミップマップ最小値
	samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	samplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;			//ピクセルシェーダからのみ使用可能

	// ルートシグネチャの設定
	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	rootSignatureDesc.pParameters = rootParams;	//ルートパラメータの先頭アドレス
	rootSignatureDesc.NumParameters = _countof(rootParams);		//ルートパラメータ数
	rootSignatureDesc.pStaticSamplers = &samplerDesc;
	rootSignatureDesc.NumStaticSamplers = 1;

	// ルートシグネチャのシリアライズ
	ID3DBlob* rootSigBlob = nullptr;
	result = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0,
		&rootSigBlob, &errorBlob);
	assert(SUCCEEDED(result));
	result = device->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(),
		IID_PPV_ARGS(&sRootSignature));
	assert(SUCCEEDED(result));
	// パイプラインにルートシグネチャをセット
	pipelineDesc.pRootSignature = sRootSignature.Get();

	//デプスステンシルステートの設定
	pipelineDesc.DepthStencilState.DepthEnable = true;	//深度テストを行う
	pipelineDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;//書き込み許可
	pipelineDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;	//小さければ合格
	pipelineDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;	//深度値フォーマット

	// パイプランステートの生成
	result = device->CreateGraphicsPipelineState(&pipelineDesc, IID_PPV_ARGS(&sPipelineState));
	assert(SUCCEEDED(result));

	//ヒープ設定
	D3D12_HEAP_PROPERTIES cbHeapProp{};
	cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;	//GPUへの転送用

	//リソース設定
	D3D12_RESOURCE_DESC cbResourceDesc{};
	cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	cbResourceDesc.Width = (sizeof(ConstBufferDataMaterial) + 0xff) & ~0xff;	//256バイトアライメント
	cbResourceDesc.Height = 1;
	cbResourceDesc.DepthOrArraySize = 1;
	cbResourceDesc.MipLevels = 1;
	cbResourceDesc.SampleDesc.Count = 1;
	cbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//デスクリプタヒープの設定
	D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
	srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	srvHeapDesc.NumDescriptors = MAX_SRV_COUNT;
	//設定を元にSRV用のデスクリプタヒープを生成
	result = device->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&sSrvHeap));
	assert(SUCCEEDED(result));
}

void Particle::PreDraw(ID3D12GraphicsCommandList* cmdList)
{
	Particle::sCmdList = cmdList;
	// パイプラインステートとルートシグネチャの設定コマンド
	cmdList->SetPipelineState(sPipelineState.Get());
	cmdList->SetGraphicsRootSignature(sRootSignature.Get());

	// プリミティブ形状の設定コマンド
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_POINTLIST); // 三角形リスト
}

void Particle::PostDraw()
{
	Particle::sCmdList = nullptr;
}

uint32_t Particle::LoadTexture(string file) {
	HRESULT result;

	TexMetadata metadata{};
	ScratchImage scratchImg{};
	string fileName = "Resources/" + file;
	//ユニコード文字列に変換する
	wchar_t wfilepath[128];
	MultiByteToWideChar(CP_ACP, 0, fileName.c_str(), -1, wfilepath, _countof(wfilepath));

	//WICテクスチャのロード
	result = LoadFromWICFile(
		wfilepath,
		WIC_FLAGS_NONE,
		&metadata,
		scratchImg
	);

	ScratchImage mipChain{};
	//ミップマップ生成
	result = GenerateMipMaps(
		scratchImg.GetImages(),
		scratchImg.GetImageCount(),
		scratchImg.GetMetadata(),
		TEX_FILTER_DEFAULT,
		0,
		mipChain
	);
	if (SUCCEEDED(result)) {
		scratchImg = std::move(mipChain);
		metadata = scratchImg.GetMetadata();
	}
	//読み込んだディフューズテクスチャをSRGBとして扱う
	metadata.format = MakeSRGB(metadata.format);

	//ヒープ設定
	D3D12_HEAP_PROPERTIES textureHeapProp{};
	textureHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
	textureHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	textureHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;
	//リソース設定
	D3D12_RESOURCE_DESC textureResourceDesc{};
	textureResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	textureResourceDesc.Format = metadata.format;
	textureResourceDesc.Width = metadata.width;
	textureResourceDesc.Height = (uint32_t)metadata.height;
	textureResourceDesc.DepthOrArraySize = (UINT16)metadata.arraySize;
	textureResourceDesc.MipLevels = (UINT16)metadata.mipLevels;
	textureResourceDesc.SampleDesc.Count = 1;

	//テクスチャバッファの生成
	result = sDevice->CreateCommittedResource(
		&textureHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&textureResourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&sTextureBuffers[sTextureIndex])
	);

	//全ミップマップについて
	for (size_t i = 0; i < metadata.mipLevels; i++) {
		//ミップマップレベルを指定してイメージを取得
		const Image* img = scratchImg.GetImage(i, 0, 0);
		// テクスチャバッファにデータ転送
		result = sTextureBuffers[sTextureIndex]->WriteToSubresource(
			(uint32_t)i,
			nullptr,
			img->pixels,
			(uint32_t)img->rowPitch,
			(uint32_t)img->slicePitch
		);
		assert(SUCCEEDED(result));
	}

	//シェーダーリソースビュー設定
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};//設定構造体
	srvDesc.Format = textureResourceDesc.Format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;	//2Dテクスチャ
	srvDesc.Texture2D.MipLevels = textureResourceDesc.MipLevels;

	//SRVヒープの先頭ハンドルを取得
	D3D12_CPU_DESCRIPTOR_HANDLE srvHandle = sSrvHeap->GetCPUDescriptorHandleForHeapStart();
	//テクスチャ切り替え
	sIncrementSize = sDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	srvHandle.ptr += sIncrementSize * sTextureIndex;

	//ハンドルの指す位置にシェーダーリソースビュー作成
	sDevice->CreateShaderResourceView(sTextureBuffers[sTextureIndex].Get(), &srvDesc, srvHandle);

	textureNum_ = sTextureIndex;
	sTextureIndex++;

	return sTextureIndex - 1;
}

void Particle::Initialize(string textureName)
{
	HRESULT result;
	LoadTexture(textureName);
	AdjustTextureSize();

	size_ = textureSize_;

	uint32_t sizeVB = static_cast<uint32_t>(sizeof(vertices_[0]) * _countof(vertices_));

	// 頂点バッファの設定
	heapProp_.Type = D3D12_HEAP_TYPE_UPLOAD; // GPUへの転送用
	// リソース設定
	resDesc_.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc_.Width = sizeVB; // 頂点データ全体のサイズ
	resDesc_.Height = 1;
	resDesc_.DepthOrArraySize = 1;
	resDesc_.MipLevels = 1;
	resDesc_.SampleDesc.Count = 1;
	resDesc_.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	// 頂点バッファの生成
	result = sDevice->CreateCommittedResource(
		&heapProp_, // ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&resDesc_, // リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff_));
	assert(SUCCEEDED(result));

	// GPU上のバッファに対応した仮想メモリ(メインメモリ上)を取得
	VertexSp* vertMap = nullptr;
	result = vertBuff_->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(result));
	// 全頂点に対して
	for (int i = 0; i < vertexCount; i++) {
		vertMap[i] = vertices_[i]; // 座標をコピー
	}
	// 繋がりを解除
	vertBuff_->Unmap(0, nullptr);

	// GPU仮想アドレス
	vbView_.BufferLocation = vertBuff_->GetGPUVirtualAddress();
	// 頂点バッファのサイズ
	vbView_.SizeInBytes = sizeVB;
	// 頂点1つ分のデータサイズ
	vbView_.StrideInBytes = sizeof(vertices_[0]);


	//ヒープ設定
	D3D12_HEAP_PROPERTIES cbHeapProp{};
	cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;		//GPUへの転送用
	//リソース設定
	D3D12_RESOURCE_DESC cbResourceDesc{};
	cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	cbResourceDesc.Width = (sizeof(ConstBuffB1) + 0xff) & ~0xff;	//256バイトアライメント
	cbResourceDesc.Height = 1;
	cbResourceDesc.DepthOrArraySize = 1;
	cbResourceDesc.MipLevels = 1;
	cbResourceDesc.SampleDesc.Count = 1;
	cbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;


	//定数バッファの生成
	result = Particle::sDevice->CreateCommittedResource(
		&cbHeapProp,		//ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&cbResourceDesc,	//リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffTransform_)
	);
	assert(SUCCEEDED(result));

	//定数バッファのマッピング
	result = constBuffTransform_->Map(0, nullptr, (void**)&constMapTransform_);	//マッピング
	assert(SUCCEEDED(result));

	//2Dの行列

	worldTransform_.SetMatWorld(
		Matrix4(1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1)
	);
	worldTransform_.SetMatWorld(0, 0, 2.0f / WIN_WIDTH);
	worldTransform_.SetMatWorld(1, 1, -2.0f / WIN_HEIGHT);
	worldTransform_.SetMatWorld(3, 0, -1.0f);
	worldTransform_.SetMatWorld(3, 1, 1.0f);

	//ワールド変換行列
	WorldTransform matTransform;
	matTransform.SetMatWorld(
		Matrix4(1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1)
	);


	matTransform.SetRotZ(rotate_);
	matTransform.SetPos(Vector3(pos_.x, pos_.y, pos_.z));
	matTransform.SetWorldMat();

	//constMapTransform_->mat = matTransform.GetMatWorld() * worldTransform_.GetMatWorld();
	Camera* camera = Camera::GetInstance();
	constMapTransform_->mat = ConvertToXMMATRIX(camera->GetMatView() * camera->GetMatProjection() * matTransform.GetMatWorld());

	SetUpVertex();

	Clear();
}

void Particle::Update()
{
	HRESULT result;

	circleParticles_.remove_if([](ParticleState& x) {return x.frame >= x.num_frame; });

	for (std::forward_list<ParticleState>::iterator it = circleParticles_.begin(); it != circleParticles_.end(); it++) {
		it->frame++;
		float f = (float)it->frame / it->num_frame;
		//スケールの線形補間
		it->scale = (it->e_scale - it->s_scale) * f;
		it->scale += it->s_scale;

		//カラーの線形補間
		it->color = (it->e_color - it->s_color) * f;
		it->color += it->s_color;

		it->velocity = it->velocity + it->gravity;
		it->speed *= it->accel.x;
		it->position = it->position + it->velocity * it->speed;
	}

	// GPU上のバッファに対応した仮想メモリ(メインメモリ上)を取得
	VertexSp* vertMap = nullptr;
	result = vertBuff_->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(result));
	// 全頂点に対して
	for (std::forward_list<ParticleState>::iterator it = circleParticles_.begin(); it != circleParticles_.end(); it++) {
		vertMap->pos.x = it->position.x;
		vertMap->pos.y = it->position.y;
		vertMap->pos.z = it->position.z;
		vertMap->scale = it->scale;
		vertMap->color.x = it->color.x;
		vertMap->color.y = it->color.y;
		vertMap->color.z = it->color.z;
		vertMap->color.w = 1;
		vertMap++;
	}

	// 繋がりを解除
	//vertBuff_->Unmap(0, nullptr);

	SetUpVertex();
}

void Particle::Draw()
{
	// 頂点バッファビューの設定コマンド
	sCmdList->IASetVertexBuffers(0, 1, &vbView_);
	//デスクリプタヒープの配列をセットするコマンド
	ID3D12DescriptorHeap* ppHeaps[] = { sSrvHeap.Get() };
	sCmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
	//SRVヒープの先頭ハンドルを取得(SRVを指すはず)
	D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = sSrvHeap->GetGPUDescriptorHandleForHeapStart();
	//描画するテクスチャの指定
	srvGpuHandle.ptr += sIncrementSize * textureNum_;
	//SRVヒープの先頭にあるSRVをルートパラメータ1番に設定
	sCmdList->SetGraphicsRootDescriptorTable(1, srvGpuHandle);
	//定数バッファビュー(CBV)の設定コマンド
	sCmdList->SetGraphicsRootConstantBufferView(2, constBuffTransform_->GetGPUVirtualAddress());

	if (isView_ == true) {
		// 描画コマンド
		sCmdList->DrawInstanced((UINT)std::distance(circleParticles_.begin(), circleParticles_.end()), 1, 0, 0); // 全ての頂点を使って描画
	}
}

void Particle::SetPos(float x, float y) {
	pos_.x = x;
	pos_.y = y;

	SetUpVertex();
}

void Particle::SetRotate(float r) {
	rotate_ = r;

	SetUpVertex();
}

void Particle::SetColor(float x, float y, float z, float w) {
	color_.x = x;
	color_.y = y;
	color_.z = z;
	color_.w = w;

	SetUpVertex();
}

void Particle::SetSize(float x, float y) {
	size_.x = x;
	size_.y = y;

	SetUpVertex();
}

void Particle::SetAnchorPoint(float x, float y) {
	anchorPoint_.x = x;
	anchorPoint_.y = y;

	SetUpVertex();
}

void Particle::SetFlipX(bool isFlip) {
	isFlipX_ = isFlip;

	SetUpVertex();
}
void Particle::SetFlipY(bool isFlip) {
	isFlipY_ = isFlip;

	SetUpVertex();
}

void Particle::SetTextureSize(float x, float y) {
	textureSize_.x = x;
	textureSize_.y = y;

	SetUpVertex();
}

void Particle::AddCircle(int life, Vector3 pos, bool isRevers, Vector3 velo, float speed, Vector3 accel, Vector3 gravity, float start_scale, float end_scale, Vector3 sColor, Vector3 eColor, int32_t postEffect)
{
	circleParticles_.emplace_front();
	ParticleState& p = circleParticles_.front();
	p.originPos = pos;
	p.scale = start_scale;
	p.s_scale = start_scale;
	p.e_scale = end_scale;
	p.velocity = velo.normalize();
	p.speed = speed / 1000;
	if (isRevers) {
		p.position = pos + p.velocity * p.speed * (float)life;
		p.velocity *= -1;
	}
	else {
		p.position = pos;
	}
	p.accel = accel;
	p.gravity = gravity;
	p.num_frame = life;
	p.color = sColor;
	p.s_color = sColor;
	p.e_color = eColor;
	p.postEffect = postEffect;
}

void Particle::Add(Vector3 pos, EditFile data)
{
	//ランダム
	std::random_device seed_gen;
	std::mt19937_64 engine(seed_gen());

	for (int i = 0; i < data.num; i++) {
		std::uniform_real_distribution<float> xp(-data.posRand.x, data.posRand.x);
		std::uniform_real_distribution<float> yp(-data.posRand.y, data.posRand.y);
		std::uniform_real_distribution<float> zp(-data.posRand.z, data.posRand.z);

		std::uniform_real_distribution<float> xv(-data.moveRand.x, data.moveRand.x);
		std::uniform_real_distribution<float> yv(-data.moveRand.y, data.moveRand.y);
		std::uniform_real_distribution<float> zv(-data.moveRand.z, data.moveRand.z);

		std::uniform_real_distribution<float> l(-float(data.lifeRand + 0.99f), float(data.lifeRand + 0.99f));

		AddCircle(
			data.life + (int32_t)l(engine),
			{ pos.x + data.pos.x + xp(engine),pos.y + data.pos.y + yp(engine),pos.z + data.pos.z + zp(engine) },
			data.isRevers,
			{ data.move.x + xv(engine),data.move.y + yv(engine), data.move.z + zv(engine) },
			data.speed,
			data.acceleration,
			data.gravity,
			data.scale.x,
			data.scale.y,
			data.sColor,
			data.eColor,
			data.postEffect
		);
	}
	if (data.add1) {
		for (int i = 0; i < data.num1; i++) {
			std::uniform_real_distribution<float> xp(-data.posRand1.x, data.posRand1.x);
			std::uniform_real_distribution<float> yp(-data.posRand1.y, data.posRand1.y);
			std::uniform_real_distribution<float> zp(-data.posRand1.z, data.posRand1.z);

			std::uniform_real_distribution<float> xv(-data.moveRand1.x, data.moveRand1.x);
			std::uniform_real_distribution<float> yv(-data.moveRand1.y, data.moveRand1.y);
			std::uniform_real_distribution<float> zv(-data.moveRand1.z, data.moveRand1.z);

			std::uniform_real_distribution<float> l(-float(data.lifeRand1 + 0.99f), float(data.lifeRand1 + 0.99f));

			AddCircle(
				data.life1 + (int32_t)l(engine),
				{ pos.x + data.pos1.x + xp(engine),pos.y + data.pos1.y + yp(engine),pos.z + data.pos1.z + zp(engine) },
				data.isRevers1,
				{ data.move1.x + xv(engine),data.move.y + yv(engine), data.move1.z + zv(engine) },
				data.speed1,
				data.acceleration1,
				data.gravity1,
				data.scale1.x,
				data.scale1.y,
				data.sColor1,
				data.eColor1,
				data.postEffect1
			);
		}
		if (data.add2) {
			for (int i = 0; i < data.num2; i++) {
				std::uniform_real_distribution<float> xp(-data.posRand2.x, data.posRand2.x);
				std::uniform_real_distribution<float> yp(-data.posRand2.y, data.posRand2.y);
				std::uniform_real_distribution<float> zp(-data.posRand2.z, data.posRand2.z);

				std::uniform_real_distribution<float> xv(-data.moveRand2.x, data.moveRand2.x);
				std::uniform_real_distribution<float> yv(-data.moveRand2.y, data.moveRand2.y);
				std::uniform_real_distribution<float> zv(-data.moveRand2.z, data.moveRand2.z);

				std::uniform_real_distribution<float> l(-float(data.lifeRand2 + 0.99f), float(data.lifeRand2 + 0.99f));

				AddCircle(
					data.life2 + (int32_t)l(engine),
					{ pos.x + data.pos2.x + xp(engine),pos.y + data.pos2.y + yp(engine),pos.z + data.pos2.z + zp(engine) },
					data.isRevers2,
					{ data.move2.x + xv(engine),data.move2.y + yv(engine), data.move2.z + zv(engine) },
					data.speed2,
					data.acceleration2,
					data.gravity2,
					data.scale2.x,
					data.scale2.y,
					data.sColor2,
					data.eColor2,
					data.postEffect2
				);
			}
		}
	}
}

void Particle::AddEditScaleAndColor(Vector3 pos, EditFile data, float scale, Vector3 color)
{
	//ランダム
	std::random_device seed_gen;
	std::mt19937_64 engine(seed_gen());

	for (int i = 0; i < data.num; i++) {
		std::uniform_real_distribution<float> xp(-data.posRand.x, data.posRand.x);
		std::uniform_real_distribution<float> yp(-data.posRand.y, data.posRand.y);
		std::uniform_real_distribution<float> zp(-data.posRand.z, data.posRand.z);

		std::uniform_real_distribution<float> xv(-data.moveRand.x, data.moveRand.x);
		std::uniform_real_distribution<float> yv(-data.moveRand.y, data.moveRand.y);
		std::uniform_real_distribution<float> zv(-data.moveRand.z, data.moveRand.z);

		std::uniform_real_distribution<float> l(-float(data.lifeRand + 0.99f), float(data.lifeRand + 0.99f));

		AddCircle(
			data.life + (int32_t)l(engine),
			{ pos.x + data.pos.x + xp(engine),pos.y + data.pos.y + yp(engine),pos.z + data.pos.z + zp(engine) },
			data.isRevers,
			{ data.move.x + xv(engine),data.move.y + yv(engine), data.move.z + zv(engine) },
			data.speed,
			data.acceleration,
			data.gravity,
			data.scale.x * scale,
			data.scale.y * scale,
			color,
			color,
			data.postEffect
		);
	}
	if (data.add1) {
		for (int i = 0; i < data.num1; i++) {
			std::uniform_real_distribution<float> xp(-data.posRand1.x, data.posRand1.x);
			std::uniform_real_distribution<float> yp(-data.posRand1.y, data.posRand1.y);
			std::uniform_real_distribution<float> zp(-data.posRand1.z, data.posRand1.z);

			std::uniform_real_distribution<float> xv(-data.moveRand1.x, data.moveRand1.x);
			std::uniform_real_distribution<float> yv(-data.moveRand1.y, data.moveRand1.y);
			std::uniform_real_distribution<float> zv(-data.moveRand1.z, data.moveRand1.z);

			std::uniform_real_distribution<float> l(-float(data.lifeRand1 + 0.99f), float(data.lifeRand1 + 0.99f));

			AddCircle(
				data.life1 + (int32_t)l(engine),
				{ pos.x + data.pos1.x + xp(engine),pos.y + data.pos1.y + yp(engine),pos.z + data.pos1.z + zp(engine) },
				data.isRevers1,
				{ data.move1.x + xv(engine),data.move.y + yv(engine), data.move1.z + zv(engine) },
				data.speed1,
				data.acceleration1,
				data.gravity1,
				data.scale1.x * scale,
				data.scale1.y * scale,
				data.sColor1,
				data.eColor1,
				data.postEffect1
			);
		}
		if (data.add2) {
			for (int i = 0; i < data.num2; i++) {
				std::uniform_real_distribution<float> xp(-data.posRand2.x, data.posRand2.x);
				std::uniform_real_distribution<float> yp(-data.posRand2.y, data.posRand2.y);
				std::uniform_real_distribution<float> zp(-data.posRand2.z, data.posRand2.z);

				std::uniform_real_distribution<float> xv(-data.moveRand2.x, data.moveRand2.x);
				std::uniform_real_distribution<float> yv(-data.moveRand2.y, data.moveRand2.y);
				std::uniform_real_distribution<float> zv(-data.moveRand2.z, data.moveRand2.z);

				std::uniform_real_distribution<float> l(-float(data.lifeRand2 + 0.99f), float(data.lifeRand2 + 0.99f));

				AddCircle(
					data.life2 + (int32_t)l(engine),
					{ pos.x + data.pos2.x + xp(engine),pos.y + data.pos2.y + yp(engine),pos.z + data.pos2.z + zp(engine) },
					data.isRevers2,
					{ data.move2.x + xv(engine),data.move2.y + yv(engine), data.move2.z + zv(engine) },
					data.speed2,
					data.acceleration2,
					data.gravity2,
					data.scale2.x * scale,
					data.scale2.y * scale,
					data.sColor2,
					data.eColor2,
					data.postEffect2
				);
			}
		}
	}
}

void Particle::Clear()
{
	circleParticles_.clear();
}

void Particle::SetUpVertex() {
	float left = (0.0f - anchorPoint_.x) * size_.x;
	float right = (1.0f - anchorPoint_.x) * size_.x;
	float top = (0.0f - anchorPoint_.y) * size_.y;
	float bottom = (1.0f - anchorPoint_.y) * size_.y;

	if (isFlipX_ == true) {
		left *= -1;
		right *= -1;
	}
	if (isFlipY_ == true) {
		top *= -1;
		bottom *= -1;
	}

	//ID3D12Resource* textureBuffer = textureBuffers_[textureIndex].Get();
	if (sTextureBuffers[textureNum_]) {

		D3D12_RESOURCE_DESC resDesc = sTextureBuffers[textureNum_]->GetDesc();

		//float tex_left = textureLeftTop_.x / resDesc.Width;
		//float tex_right = (textureLeftTop_.x + textureSize_.x) / resDesc.Width;
		//float tex_top = textureLeftTop_.y / resDesc.Height;
		//float tex_bottom = (textureLeftTop_.y + textureSize_.y) / resDesc.Height;
	}

	//ワールド変換行列
	WorldTransform matTransform;
	matTransform.GetMatWorld().Initialize();
	matTransform.SetScale({ 10,10,10 });
	matTransform.SetRotZ(rotate_);
	matTransform.SetPos(Vector3(pos_.x, pos_.y, 0));
	matTransform.SetWorldMat();

	Camera* camera = Camera::GetInstance();
	XMMATRIX matBillboard;

	matBillboard = XMMatrixIdentity();
	XMVECTOR cameraTarget = { camera->GetTarget().x,camera->GetTarget().y,camera->GetTarget().z,0 };
	XMVECTOR cameraPos = { camera->GetEye().x,camera->GetEye().y,camera->GetEye().z,0 };

	XMVECTOR cameraAxisZ = XMVectorSubtract(cameraTarget, cameraPos);
	cameraAxisZ = XMVector3Normalize(cameraAxisZ);
	XMVECTOR cameraAxisX = XMVector3Cross({ 0,1,0 }, cameraAxisZ);
	cameraAxisX = XMVector3Normalize(cameraAxisX);
	XMVECTOR cameraAxisY = XMVector3Cross(cameraAxisZ, cameraAxisX);

	matBillboard.r[0] = cameraAxisX;
	matBillboard.r[1] = cameraAxisY;
	matBillboard.r[2] = cameraAxisZ;
	matBillboard.r[3] = XMVectorSet(0, 0, 0, 1);

	//ビルボード

	constMapTransform_->mat = ConvertToXMMATRIX(camera->GetMatView() * camera->GetMatProjection());
	constMapTransform_->billboard = matBillboard;
}

void Particle::AdjustTextureSize()
{
	D3D12_RESOURCE_DESC resDesc = sTextureBuffers[textureNum_]->GetDesc();

	textureSize_.x = static_cast<float>(resDesc.Width);
	textureSize_.y = static_cast<float>(resDesc.Height);
}
