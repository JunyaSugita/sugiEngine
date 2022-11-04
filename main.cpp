﻿#include <Windows.h>
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

	D3D12_RESOURCE_DESC depthResourceDesc{};
	depthResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	depthResourceDesc.Width = WIN_WIDTH;
	depthResourceDesc.Height = WIN_HEIGHT;
	depthResourceDesc.DepthOrArraySize = 1;
	depthResourceDesc.Format = DXGI_FORMAT_D32_FLOAT;//深度値フォーマット
	depthResourceDesc.SampleDesc.Count = 1;
	depthResourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

	//深度値用ヒーププロパティ
	D3D12_HEAP_PROPERTIES depthHeapProp{};
	depthHeapProp.Type = D3D12_HEAP_TYPE_DEFAULT;
	//深度値のクリア設定
	D3D12_CLEAR_VALUE depthClearValue{};
	depthClearValue.DepthStencil.Depth = 1.0f;		//深度値1.0f(最大値)でクリア
	depthClearValue.Format = DXGI_FORMAT_D32_FLOAT;	//深度値フォーマット
	//リソース生成
	ComPtr<ID3D12Resource> depthBuff;
	result = dxCom->GetDevice()->CreateCommittedResource(
		&depthHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&depthResourceDesc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE,	//深度値書き込みに使用
		&depthClearValue,
		IID_PPV_ARGS(&depthBuff)
	);
	//深度ビュー用デスクリプタヒープ生成
	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc{};
	dsvHeapDesc.NumDescriptors = 1;//深度ビュー1つ
	dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;//デプスステンシルビュー
	ComPtr<ID3D12DescriptorHeap> dsvHeap;
	result = dxCom->GetDevice()->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&dsvHeap));

	//深度ビュー作成
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;	//深度値フォーマット
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	dxCom->GetDevice()->CreateDepthStencilView(
		depthBuff.Get(),
		&dsvDesc,
		dsvHeap->GetCPUDescriptorHandleForHeapStart()
	);

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

		// バックバッファの番号を取得(2つなので0番か1番)
		UINT bbIndex = dxCom->GetSwapChain()->GetCurrentBackBufferIndex();
		// 1.リソースバリアで書き込み可能に変更
		D3D12_RESOURCE_BARRIER barrierDesc{};
		barrierDesc.Transition.pResource = dxCom->GetBackBuffers(bbIndex); // バックバッファを指定
		barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT; // 表示状態から
		barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET; // 描画状態へ
		dxCom->GetCommandList()->ResourceBarrier(1, &barrierDesc);
		// 2.描画先の変更
		// レンダーターゲットビューのハンドルを取得
		D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = dxCom->GetRtvHeap()->GetCPUDescriptorHandleForHeapStart();
		rtvHandle.ptr += bbIndex * dxCom->GetDevice()->GetDescriptorHandleIncrementSize(dxCom->GetRtvHeapDesc().Type);
		D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = dsvHeap->GetCPUDescriptorHandleForHeapStart();
		dxCom->GetCommandList()->OMSetRenderTargets(1, &rtvHandle, false, &dsvHandle);
		// 3.画面クリア R G B A
		if (input->PushKey(DIK_SPACE)) {
			FLOAT clearColor[] = { 0.5f,0.1f, 0.25f,0.0f }; // 赤っぽい色
			dxCom->GetCommandList()->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
			dxCom->GetCommandList()->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
		}
		else {
			FLOAT clearColor[] = { 0.1f,0.25f, 0.5f,0.0f }; // 青っぽい色
			dxCom->GetCommandList()->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
			dxCom->GetCommandList()->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
		}
		// 4.描画コマンド 
		// ビューポート設定コマンド
		D3D12_VIEWPORT viewport{};
		viewport.Width = WIN_WIDTH;
		viewport.Height = WIN_HEIGHT;
		viewport.TopLeftX = 0;
		viewport.TopLeftY = 0;
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;
		// ビューポート設定コマンドを、コマンドリストに積む
		dxCom->GetCommandList()->RSSetViewports(1, &viewport);

		// シザー矩形
		D3D12_RECT scissorRect{};
		scissorRect.left = 0; // 切り抜き座標左
		scissorRect.right = scissorRect.left + WIN_WIDTH; // 切り抜き座標右
		scissorRect.top = 0; // 切り抜き座標上
		scissorRect.bottom = scissorRect.top + WIN_HEIGHT; // 切り抜き座標下
		// シザー矩形設定コマンドを、コマンドリストに積む
		dxCom->GetCommandList()->RSSetScissorRects(1, &scissorRect);

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
		// 5.リソースバリアを戻す
		barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET; // 描画状態から
		barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT; // 表示状態へ
		dxCom->GetCommandList()->ResourceBarrier(1, &barrierDesc);

		// 命令のクローズ
		result = dxCom->GetCommandList()->Close();
		assert(SUCCEEDED(result));
		// コマンドリストの実行
		ComPtr<ID3D12CommandList> commandLists[] = { dxCom->GetCommandList() };
		dxCom->GetCommandQueue()->ExecuteCommandLists(1, commandLists->GetAddressOf());
		// 画面に表示するバッファをフリップ(裏表の入替え)
		result = dxCom->GetSwapChain()->Present(1, 0);
		assert(SUCCEEDED(result));

		// コマンドの実行完了を待つ
		dxCom->GetCommandQueue()->Signal(dxCom->GetFence(), dxCom->AddGetFanceVal());
		if (dxCom->GetFence()->GetCompletedValue() != dxCom->GetFanceVal()) {
			HANDLE event = CreateEvent(nullptr, false, false, nullptr);
			dxCom->GetFence()->SetEventOnCompletion(dxCom->GetFanceVal(), event);
			WaitForSingleObject(event, INFINITE);
			CloseHandle(event);
		}

		//FPS固定
		dxCom->UpdateFixFPS();

		// キューをクリア
		result = dxCom->GetCommandAllocator()->Reset();
		assert(SUCCEEDED(result));
		// 再びコマンドリストを貯める準備
		result = dxCom->GetCommandList()->Reset(dxCom->GetCommandAllocator(), nullptr);
		assert(SUCCEEDED(result));

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
