#include <Windows.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <cassert>
#include <vector>
#include <string>
#include <memory>
#include <DirectXMath.h>
using namespace DirectX;
#include <d3dcompiler.h>
#include <DirectXTex.h>
#include "Vector2.h"
#include "Vector3.h"
#include "Matrix4.h"
#include "WorldTransform.h"
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

#include "GrovalSetting.h"
#include "WinApp.h"
#include "DXCommon.h"
#include"Input.h"
#include "Object3d.h"

//エイリアステンプレート
template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

//定数バッファ用データ構造体(マテリアル)
struct ConstBufferDataMaterial {
	XMFLOAT4 color;	//色(RGBA)
};



//windowsのエントリーポイント
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	std::unique_ptr<WinApp> winApp = std::make_unique<WinApp>();
	std::unique_ptr<DXCommon> dxCom = std::make_unique<DXCommon>();
	std::unique_ptr<Input> input = std::make_unique<Input>();
	std::unique_ptr <Matrix4> matrix4 = std::make_unique <Matrix4>();
	Object3d box;
	Object3d box2;

#pragma region windowsAPI初期化処理

	winApp->CreateWindowScreen();
	//コンソールへ文字入力
	OutputDebugStringA("Hello DirectX\n");
	OutputDebugStringA("This is sugiEngine\n");

#pragma endregion

#pragma region DirectX初期化処理
	// DirectX初期化処理 ここから
	dxCom->Initialize(winApp.get());
	//キーボード入力の初期化
	input->Initialize(winApp.get());

#pragma endregion

#pragma region 描画初期化処理
	HRESULT result;
	
	

	
	// 頂点バッファビューの作成
	D3D12_VERTEX_BUFFER_VIEW vbView{};
	// GPU仮想アドレス
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	// 頂点バッファのサイズ
	vbView.SizeInBytes = sizeVB;
	// 頂点1つ分のデータサイズ
	vbView.StrideInBytes = sizeof(vertices[0]);

	//ヒープ設定
	D3D12_HEAP_PROPERTIES cbHeapProp{};
	cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;		//GPUへの転送用
	//リソース設定
	D3D12_RESOURCE_DESC cbResourceDesc{};
	cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	cbResourceDesc.Width = (sizeof(ConstBufferDataMaterial) + 0xff) & ~0xff;	//256バイトアライメント
	cbResourceDesc.Height = 1;
	cbResourceDesc.DepthOrArraySize = 1;
	cbResourceDesc.MipLevels = 1;
	cbResourceDesc.SampleDesc.Count = 1;
	cbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;



	ComPtr<ID3D12Resource> constBuffMaterial = nullptr;
	//定数バッファの生成
	result = dxCom->GetDevice()->CreateCommittedResource(
		&cbHeapProp,		//ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&cbResourceDesc,	//リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffMaterial)
	);
	assert(SUCCEEDED(result));

	//定数バッファのマッピング
	ConstBufferDataMaterial* constMapMaterial = nullptr;
	result = constBuffMaterial->Map(0, nullptr, (void**)&constMapMaterial);	//マッピング
	assert(SUCCEEDED(result));

	//値を書き込むと自動的に転送される
	constMapMaterial->color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);	//RGBAで半透明の赤


	box.Initialize(dxCom.get());
	box2.Initialize(dxCom.get());

	

	

	

	


	


#pragma endregion

#pragma region ゲームループ

	int angleX = 0;
	int angleY = 0;

	// ゲームループ
	while (true) {
		if (winApp->ProcMessage()) {
			break;
		}
		// DirectX毎フレーム処理 ここから///////////////////////////////////////////////////////////////////////////

		//キーボード入力
		input->Update();

		dxCom->PreDraw();

		// パイプラインステートとルートシグネチャの設定コマンド
		dxCom->GetCommandList()->SetPipelineState(pipelineState.Get());
		dxCom->GetCommandList()->SetGraphicsRootSignature(rootSignature.Get());

		// プリミティブ形状の設定コマンド
		dxCom->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // 三角形リスト

		// 頂点バッファビューの設定コマンド
		dxCom->GetCommandList()->IASetVertexBuffers(0, 1, &vbView);

		//定数バッファビュー(CBV)の設定コマンド
		dxCom->GetCommandList()->SetGraphicsRootConstantBufferView(0, constBuffMaterial->GetGPUVirtualAddress());
		//SRVヒープの設定コマンド
		dxCom->GetCommandList()->SetDescriptorHeaps(1, &srvHeap);

		//SRVヒープの先頭ハンドルを取得(SRVを指しているはず)
		D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = srvHeap->GetGPUDescriptorHandleForHeapStart();

		//2枚目
		//srvGpuHandle.ptr += incrementSize;

		//SRVヒープの先頭にあるSRVをルートパラメータ1番に設定
		dxCom->GetCommandList()->SetGraphicsRootDescriptorTable(1, srvGpuHandle);

		//インデックスバッファビューのセットコマンド
		dxCom->GetCommandList()->IASetIndexBuffer(&ibView);

#pragma region ゲームシーン
		if (input->PushKey(DIK_UP)) {
			angleX++;
		}
		if (input->PushKey(DIK_DOWN)) {
			angleX--;
		}
		if (input->PushKey(DIK_LEFT)) {
			angleY++;
		}
		if (input->PushKey(DIK_RIGHT)) {
			angleY--;
		}

		box.Trans(10.0f, 0.0f, 0.0f);
		box.Update();

		box2.Update();

		box.Draw(_countof(indices));
		box2.Draw(_countof(indices));

#pragma endregion
		dxCom->PostDraw();

		// DirectX毎フレーム処理 ここまで///////////////////////////////////////////////////////////////////////////
	}

#pragma endregion

#pragma region WindowsAPI後始末
	winApp->DeleteWindow();

#pragma endregion

#pragma region delete処理 

#pragma endregion

	return 0;
}
