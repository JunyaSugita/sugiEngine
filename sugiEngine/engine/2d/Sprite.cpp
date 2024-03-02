#include "Sprite.h"
#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")
#include <array>
#include <d3dx12.h>

using namespace Microsoft::WRL;
using namespace std;
using namespace DirectX;

ComPtr<ID3D12Device> Sprite::sDevice = nullptr;
ComPtr<ID3D12PipelineState> Sprite::sPipelineState = nullptr;
ComPtr<ID3D12RootSignature> Sprite::sRootSignature;
ComPtr<ID3D12GraphicsCommandList> Sprite::sCmdList;
std::array<ComPtr<ID3D12Resource>, Sprite::MAX_SRV_COUNT> Sprite::sTextureBuffers;
const size_t Sprite::MAX_SRV_COUNT;
ComPtr<ID3D12DescriptorHeap> Sprite::sSrvHeap;
uint32_t Sprite::sIncrementSize;
uint32_t Sprite::sTextureIndex = 0;
vector<ID3D12Resource*> Sprite::sIntermediateResource;

void Sprite::StaticInitialize(ID3D12Device* device, ID3D12GraphicsCommandList* cmdList)
{
	HRESULT result;
	sDevice = device;
	sCmdList = cmdList;

	ComPtr<ID3DBlob> vsBlob = nullptr; // 頂点シェーダオブジェクト
	ComPtr<ID3DBlob> psBlob = nullptr; // ピクセルシェーダオブジェクト
	ComPtr<ID3DBlob> errorBlob = nullptr; // エラーオブジェクト
	// 頂点シェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"Resources/Shaders/SpriteVS.hlsl", // シェーダファイル名
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
	// ピクセルシェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"Resources/Shaders/SpritePS.hlsl", // シェーダファイル名
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
		"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT, 0,
		D3D12_APPEND_ALIGNED_ELEMENT,
		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
	}
	};

	// グラフィックスパイプライン設定
	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc{};
	// シェーダーの設定
	pipelineDesc.VS.pShaderBytecode = vsBlob->GetBufferPointer();
	pipelineDesc.VS.BytecodeLength = vsBlob->GetBufferSize();
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

	//半透明合成
	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;			//加算
	blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;		//ソースのアルファ値
	blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;//1.0f-1ソースのアルファ値

	// ブレンドステートの設定
	for (int i = 0; i < MULTI_RENDAR_TARGET_NUM; i++) {
		pipelineDesc.BlendState.RenderTarget[i] = blenddesc;
	}

	// 頂点レイアウトの設定
	pipelineDesc.InputLayout.pInputElementDescs = inputLayout;
	pipelineDesc.InputLayout.NumElements = _countof(inputLayout);
	// 図形の形状設定
	pipelineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	// その他の設定
	pipelineDesc.NumRenderTargets = 2; // 描画対象は1つ
	pipelineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 0~255指定のRGBA
	pipelineDesc.RTVFormats[1] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 0~255指定のRGBA
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

void Sprite::PreDraw(ID3D12GraphicsCommandList* cmdList)
{
	Sprite::sCmdList = cmdList;
	// パイプラインステートとルートシグネチャの設定コマンド
	cmdList->SetPipelineState(sPipelineState.Get());
	cmdList->SetGraphicsRootSignature(sRootSignature.Get());

	// プリミティブ形状の設定コマンド
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP); // 三角形リスト
}

void Sprite::PostDraw()
{
	//Sprite::sCmdList = nullptr;
}

uint32_t Sprite::LoadTexture(const string& textureName, const std::string& fileExt) {
	HRESULT result;

	TexMetadata metadata{};
	ScratchImage scratchImg{};
	string fileName = "Resources/" + textureName + "." + fileExt;
	//ユニコード文字列に変換する
	wchar_t wfilepath[128];
	MultiByteToWideChar(CP_ACP, 0, fileName.c_str(), -1, wfilepath, _countof(wfilepath));

	if (fileExt == "dds") {
		//DDSテクスチャのロード
		result = LoadFromDDSFile(wfilepath, DDS_FLAGS_NONE, &metadata, scratchImg);
	}
	else {
		//WICテクスチャのロード
		result = LoadFromWICFile(wfilepath, WIC_FLAGS_NONE, &metadata, scratchImg);

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
	}

	//読み込んだディフューズテクスチャをSRGBとして扱う
	metadata.format = MakeSRGB(metadata.format);

	//ヒープ設定
	D3D12_HEAP_PROPERTIES textureHeapProp{};
	textureHeapProp.Type = D3D12_HEAP_TYPE_DEFAULT;
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
		D3D12_RESOURCE_STATE_COPY_DEST,
		nullptr,
		IID_PPV_ARGS(&sTextureBuffers[sTextureIndex])
	);

	vector<D3D12_SUBRESOURCE_DATA> subResources;
	DirectX::PrepareUpload(sDevice.Get(), scratchImg.GetImages(),scratchImg.GetImageCount(),scratchImg.GetMetadata(),subResources);
	uint64_t intermediateSize = GetRequiredIntermediateSize(sTextureBuffers[sTextureIndex].Get(), 0, UINT(subResources.size()));
	ID3D12Resource* intermediateResource = CreateBufferResource(intermediateSize);
	UpdateSubresources(sCmdList.Get(), sTextureBuffers[sTextureIndex].Get(),intermediateResource,0,0,UINT(subResources.size()),subResources.data());
	
	D3D12_RESOURCE_BARRIER barrier{};
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barrier.Transition.pResource = sTextureBuffers[sTextureIndex].Get();
	barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_GENERIC_READ;
	sCmdList->ResourceBarrier(1,&barrier);

	sIntermediateResource.push_back(intermediateResource);

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

	sTextureIndex++;

	return sTextureIndex - 1;
}

void Sprite::Initialize(uint32_t texNum)
{
	HRESULT result;
	textureNum_ = texNum;
	AdjustTextureSize();

	vertices_[0] = { {  0.0f,textureSize_.y,0.0f},{0.0f,1.0f} };	//左下
	vertices_[1] = { {  0.0f,  0.0f,0.0f},{0.0f,0.0f} };	//左上
	vertices_[2] = { {textureSize_.x,textureSize_.y,0.0f},{1.0f,1.0f} };	//右下
	vertices_[3] = { {textureSize_.x,  0.0f,0.0f},{1.0f,0.0f} };	//右上
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
	for (int32_t i = 0; i < _countof(vertices_); i++) {
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
	cbResourceDesc.Width = (sizeof(ConstBufferDataTransform) + 0xff) & ~0xff;	//256バイトアライメント
	cbResourceDesc.Height = 1;
	cbResourceDesc.DepthOrArraySize = 1;
	cbResourceDesc.MipLevels = 1;
	cbResourceDesc.SampleDesc.Count = 1;
	cbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;


	//定数バッファの生成
	result = Sprite::sDevice->CreateCommittedResource(
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
	matTransform.SetPos(Vector3(pos_.x, pos_.y, 0));
	matTransform.SetWorldMat();

	constMapTransform_->mat = matTransform.GetMatWorld() * worldTransform_.GetMatWorld();

	//定数バッファ
	result = sDevice->CreateCommittedResource(
		&cbHeapProp,	//ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&cbResourceDesc,//リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffMaterial_)
	);
	assert(SUCCEEDED(result));

	result = constBuffMaterial_->Map(0, nullptr, (void**)&constMapMaterial_);	//マッピング
	assert(SUCCEEDED(result));

	constMapMaterial_->color = color_;

	SetUpVertex();
}

void Sprite::Draw()
{
	// 頂点バッファビューの設定コマンド
	sCmdList->IASetVertexBuffers(0, 1, &vbView_);
	//定数バッファビュー(CBV)の設定コマンド
	sCmdList->SetGraphicsRootConstantBufferView(0, constBuffMaterial_->GetGPUVirtualAddress());
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
		sCmdList->DrawInstanced(4, 1, 0, 0); // 全ての頂点を使って描画
	}
}

void Sprite::SetPos(float x, float y) {
	pos_.x = x;
	pos_.y = y;

	SetUpVertex();
}

void Sprite::SetPos(const Vector2& pos)
{
	pos_ = pos;

	SetUpVertex();
}

void Sprite::SetRotate(float r) {
	rotate_ = r;

	SetUpVertex();
}

void Sprite::SetColor(float x, float y, float z, float w) {
	color_.x = x;
	color_.y = y;
	color_.z = z;
	color_.w = w;

	SetUpVertex();
}

void Sprite::SetColor(const Vector4& color) {
	color_.x = color.x;
	color_.y = color.y;
	color_.z = color.z;
	color_.w = color.w;

	SetUpVertex();
}

void Sprite::SetColor(float x, float y, float z)
{
	color_.x = x;
	color_.y = y;
	color_.z = z;

	SetUpVertex();
}

void Sprite::SetColor(const Vector3& color)
{
	color_.x = color.x;
	color_.y = color.y;
	color_.z = color.z;

	SetUpVertex();
}

void Sprite::SetAlpha(float alpha)
{
	color_.w = alpha;
}

void Sprite::SetSize(float x, float y) {
	size_.x = x;
	size_.y = y;

	SetUpVertex();
}

void Sprite::SetSize(const Vector2& vec) {
	size_.x = vec.x;
	size_.y = vec.y;

	SetUpVertex();
}

void Sprite::SetSizeX(float x)
{
	size_.x = x;

	SetUpVertex();
}

void Sprite::SetSizeY(float y)
{
	size_.y = y;

	SetUpVertex();
}

void Sprite::SetAnchorPoint(float x, float y) {
	anchorPoint_.x = x;
	anchorPoint_.y = y;

	SetUpVertex();
}

void Sprite::SetFlipX(bool isFlip) {
	isFlipX_ = isFlip;

	SetUpVertex();
}
void Sprite::SetFlipY(bool isFlip) {
	isFlipY_ = isFlip;

	SetUpVertex();
}

void Sprite::SetTextureSize(float x, float y) {
	textureSize_.x = x;
	textureSize_.y = y;

	SetUpVertex();
}

void Sprite::SetUpVertex() {

	HRESULT result;

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

	vertices_[0].pos = { left,bottom,0.0f };	//左下
	vertices_[1].pos = { left,   top,0.0f };	//左上
	vertices_[2].pos = { right,bottom,0.0f };	//右下
	vertices_[3].pos = { right,   top,0.0f };	//右上

	if (sTextureBuffers[textureNum_]) {

		D3D12_RESOURCE_DESC resDesc = sTextureBuffers[textureNum_]->GetDesc();

		float tex_left = textureLeftTop_.x / resDesc.Width;
		float tex_right = (textureLeftTop_.x + textureSize_.x) / resDesc.Width;
		float tex_top = textureLeftTop_.y / resDesc.Height;
		float tex_bottom = (textureLeftTop_.y + textureSize_.y) / resDesc.Height;

		//UV
		vertices_[0].uv = { tex_left,tex_bottom };	//左下
		vertices_[1].uv = { tex_left,tex_top };	//左上
		vertices_[2].uv = { tex_right,tex_bottom };	//右下
		vertices_[3].uv = { tex_right,tex_top };	//右上
	}

	// GPU上のバッファに対応した仮想メモリ(メインメモリ上)を取得
	VertexSp* vertMap = nullptr;
	result = vertBuff_->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(result));
	// 全頂点に対して
	for (int32_t i = 0; i < 4; i++) {
		vertMap[i] = vertices_[i]; // 座標をコピー
	}

	//ワールド変換行列
	WorldTransform matTransform;
	matTransform.GetMatWorld().Initialize();
	matTransform.SetRotZ(rotate_);
	matTransform.SetPos(Vector3(pos_.x, pos_.y, 0));
	matTransform.SetWorldMat();

	constMapTransform_->mat = matTransform.GetMatWorld() * worldTransform_.GetMatWorld();

	constMapMaterial_->color = color_;

}

void Sprite::AdjustTextureSize()
{
	D3D12_RESOURCE_DESC resDesc = sTextureBuffers[textureNum_]->GetDesc();

	textureSize_.x = static_cast<float>(resDesc.Width);
	textureSize_.y = static_cast<float>(resDesc.Height);
}

ID3D12Resource* Sprite::CreateBufferResource(uint64_t size)
{
	HRESULT result;

	D3D12_HEAP_PROPERTIES heapProperties{};
	heapProperties.Type = D3D12_HEAP_TYPE_UPLOAD;

	D3D12_RESOURCE_DESC bufferResourceDesc{};
	bufferResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	bufferResourceDesc.Width = size;
	//↓バッファの時は確定
	bufferResourceDesc.Height = 1;
	bufferResourceDesc.DepthOrArraySize = 1;
	bufferResourceDesc.MipLevels = 1;
	bufferResourceDesc.SampleDesc.Count = 1;
	bufferResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//実際に作成
	ID3D12Resource* bufferResource = nullptr;
	result = sDevice->CreateCommittedResource(
		&heapProperties,
		D3D12_HEAP_FLAG_NONE,
		&bufferResourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&bufferResource)
	);
	assert(SUCCEEDED(result));

	return bufferResource;
}
