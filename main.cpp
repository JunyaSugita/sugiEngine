﻿#include <Windows.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <cassert>
#include <vector>
#include <string>
#include <DirectXMath.h>
using namespace DirectX;
#include <d3dcompiler.h>
#include <DirectXTex.h>
#include "Vector2.h"
#include "Vector3.h"
#include "Matrix4.h"
Matrix4* matrix4 = new Matrix4;
#include "WorldTransform.h"

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

#include"Input.h"
Input* input = new Input;
#include "Groval.h"
#include"Draw.h"

LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
	//メッセージに大路手ゲーム固有の処理を行う
	switch (msg)
	{
		//ウィンドウが破壊された
	case WM_DESTROY:
		//OSに対してアプリの終了を伝える
		PostQuitMessage(0);
		return 0;
	}

	//標準のメッセージ処理を行う
	return DefWindowProc(hwnd, msg, wparam, lparam);
}

//windowsのエントリーポイント
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

#pragma region windowsAPI初期化処理
	//コンソールへ文字入力
	OutputDebugStringA("Hello DirectX\n");
	OutputDebugStringA("This is sugiEngine\n");

	//ウィンドウクラスの設定
	WNDCLASSEX w{};
	w.cbSize = sizeof(WNDCLASSEX);
	w.lpfnWndProc = (WNDPROC)WindowProc; // ウィンドウプロシージャを設定
	w.lpszClassName = L"DirectXGame"; // ウィンドウクラス名
	w.hInstance = GetModuleHandle(nullptr); // ウィンドウハンドル
	w.hCursor = LoadCursor(NULL, IDC_ARROW); // カーソル指定
	// ウィンドウクラスをOSに登録する
	RegisterClassEx(&w);
	// ウィンドウサイズ{ X座標 Y座標 横幅 縦幅 }
	RECT wrc = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };
	// 自動でサイズを補正する
	AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false);

	// ウィンドウオブジェクトの生成
	HWND hwnd = CreateWindow(w.lpszClassName, // クラス名
		L"DirectXGame", // タイトルバーの文字
		WS_OVERLAPPEDWINDOW, // 標準的なウィンドウスタイル
		CW_USEDEFAULT, // 表示X座標(OSに任せる)
		CW_USEDEFAULT, // 表示Y座標(OSに任せる)
		wrc.right - wrc.left, // ウィンドウ横幅
		wrc.bottom - wrc.top, // ウィンドウ縦幅
		nullptr, // 親ウィンドウハンドル
		nullptr, // メニューハンドル
		w.hInstance, // 呼び出しアプリケーションハンドル
		nullptr); // オプション
	// ウィンドウを表示状態にする
	ShowWindow(hwnd, SW_SHOW);

	MSG msg{}; // メッセージ
#pragma endregion

#pragma region DirectX初期化処理
	// DirectX初期化処理 ここから

#ifdef _DEBUG
//デバッグレイヤーをオンに
	ID3D12Debug* debugController;
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)))) {
		debugController->EnableDebugLayer();
	}
#endif

	HRESULT result;
	ID3D12Device* device = nullptr;
	IDXGIFactory7* dxgiFactory = nullptr;
	IDXGISwapChain4* swapChain = nullptr;
	ID3D12CommandAllocator* commandAllocator = nullptr;		
	ID3D12GraphicsCommandList* commandList = nullptr;
	ID3D12CommandQueue* commandQueue = nullptr;
	ID3D12DescriptorHeap* rtvHeap = nullptr;

	// DXGIファクトリーの生成
	result = CreateDXGIFactory(IID_PPV_ARGS(&dxgiFactory));
	assert(SUCCEEDED(result));
	// アダプターの列挙用
	std::vector<IDXGIAdapter4*> adapters;
	// ここに特定の名前を持つアダプターオブジェクトが入る
	IDXGIAdapter4* tmpAdapter = nullptr;
	// パフォーマンスが高いものから順に、全てのアダプターを列挙する
	for (UINT i = 0;
		dxgiFactory->EnumAdapterByGpuPreference(i,
			DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE,
			IID_PPV_ARGS(&tmpAdapter)) != DXGI_ERROR_NOT_FOUND;
		i++) {
		// 動的配列に追加する
		adapters.push_back(tmpAdapter);
	}

	// 妥当なアダプタを選別する
	for (size_t i = 0; i < adapters.size(); i++) {
		DXGI_ADAPTER_DESC3 adapterDesc;
		// アダプターの情報を取得する
		adapters[i]->GetDesc3(&adapterDesc);
		// ソフトウェアデバイスを回避
		if (!(adapterDesc.Flags & DXGI_ADAPTER_FLAG3_SOFTWARE)) {
			// デバイスを採用してループを抜ける
			tmpAdapter = adapters[i];
			break;
		}
	}

	// 対応レベルの配列
	D3D_FEATURE_LEVEL levels[] = {
	D3D_FEATURE_LEVEL_12_1,
	D3D_FEATURE_LEVEL_12_0,
	D3D_FEATURE_LEVEL_11_1,
	D3D_FEATURE_LEVEL_11_0,
	};

	D3D_FEATURE_LEVEL featureLevel;
	for (size_t i = 0; i < _countof(levels); i++) {
		// 採用したアダプターでデバイスを生成
		result = D3D12CreateDevice(tmpAdapter, levels[i],
			IID_PPV_ARGS(&device));
		if (result == S_OK) {
			// デバイスを生成できた時点でループを抜ける
			featureLevel = levels[i];
			break;
		}
	}

	// コマンドアロケータを生成
	result = device->CreateCommandAllocator(
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		IID_PPV_ARGS(&commandAllocator));
	assert(SUCCEEDED(result));
	// コマンドリストを生成
	result = device->CreateCommandList(0,
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		commandAllocator, nullptr,
		IID_PPV_ARGS(&commandList));
	assert(SUCCEEDED(result));

	//コマンドキューの設定
	D3D12_COMMAND_QUEUE_DESC commandQueueDesc{};
	//コマンドキューを生成
	result = device->CreateCommandQueue(&commandQueueDesc, IID_PPV_ARGS(&commandQueue));
	assert(SUCCEEDED(result));

	// スワップチェーンの設定
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};
	swapChainDesc.Width = 1280;
	swapChainDesc.Height = 720;
	swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // 色情報の書式
	swapChainDesc.SampleDesc.Count = 1; // マルチサンプルしない
	swapChainDesc.BufferUsage = DXGI_USAGE_BACK_BUFFER; // バックバッファ用
	swapChainDesc.BufferCount = 2; // バッファ数を2つに設定
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD; // フリップ後は破棄
	swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	// スワップチェーンの生成
	result = dxgiFactory->CreateSwapChainForHwnd(
		commandQueue, hwnd, &swapChainDesc, nullptr, nullptr,
		(IDXGISwapChain1**)&swapChain);
	assert(SUCCEEDED(result));

	// デスクリプタヒープの設定
	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc{};
	rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV; // レンダーターゲットビュー
	rtvHeapDesc.NumDescriptors = swapChainDesc.BufferCount; // 裏表の2つ
	// デスクリプタヒープの生成
	device->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&rtvHeap));

	// バックバッファ
	std::vector<ID3D12Resource*> backBuffers;
	backBuffers.resize(swapChainDesc.BufferCount);

	// スワップチェーンの全てのバッファについて処理する
	for (size_t i = 0; i < backBuffers.size(); i++) {
		// スワップチェーンからバッファを取得
		swapChain->GetBuffer((UINT)i, IID_PPV_ARGS(&backBuffers[i]));
		// デスクリプタヒープのハンドルを取得
		D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = rtvHeap->GetCPUDescriptorHandleForHeapStart();
		// 裏か表かでアドレスがずれる
		rtvHandle.ptr += i * device->GetDescriptorHandleIncrementSize(rtvHeapDesc.Type);
		// レンダーターゲットビューの設定
		D3D12_RENDER_TARGET_VIEW_DESC rtvDesc{};
		// シェーダーの計算結果をSRGBに変換して書き込む
		rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
		rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
		// レンダーターゲットビューの生成
		device->CreateRenderTargetView(backBuffers[i], &rtvDesc, rtvHandle);
	}

	// フェンスの生成
	ID3D12Fence* fence = nullptr;
	UINT64 fenceVal = 0;
	result = device->CreateFence(fenceVal, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence));

	//キーボード入力の初期化
	input->Initialize(result, w, hwnd);

#pragma endregion

#pragma region 描画初期化処理
	Draw draw;
	draw.GraphInisialize(device);

#pragma endregion

#pragma region ゲームループ
	// ゲームループ
	while (true) {
		// メッセージがある?
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg); // キー入力メッセージの処理
			DispatchMessage(&msg); // プロシージャにメッセージを送る
		}
		// ✖ボタンで終了メッセージが来たらゲームループを抜ける
		if (msg.message == WM_QUIT) {
			break;
		}
		// DirectX毎フレーム処理 ここから///////////////////////////////////////////////////////////////////////////

		//キーボード入力
		input->Update();

		// バックバッファの番号を取得(2つなので0番か1番)
		UINT bbIndex = swapChain->GetCurrentBackBufferIndex();
		// 1.リソースバリアで書き込み可能に変更
		D3D12_RESOURCE_BARRIER barrierDesc{};
		barrierDesc.Transition.pResource = backBuffers[bbIndex]; // バックバッファを指定
		barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT; // 表示状態から
		barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET; // 描画状態へ
		commandList->ResourceBarrier(1, &barrierDesc);
		// 2.描画先の変更
		// レンダーターゲットビューのハンドルを取得
		D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = rtvHeap->GetCPUDescriptorHandleForHeapStart();
		rtvHandle.ptr += bbIndex * device->GetDescriptorHandleIncrementSize(rtvHeapDesc.Type);
		commandList->OMSetRenderTargets(1, &rtvHandle, false, nullptr);
		// 3.画面クリア R G B A
		if (input->PushKey(DIK_SPACE)) {
			FLOAT clearColor[] = { 0.5f,0.1f, 0.25f,0.0f }; // 赤っぽい色
			commandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
		}
		else {
			FLOAT clearColor[] = { 0.1f,0.25f, 0.5f,0.0f }; // 青っぽい色
			commandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
		}
		// 4.描画コマンド
		draw.GraphUpdate(commandList);
		// 5.リソースバリアを戻す
		barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET; // 描画状態から
		barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT; // 表示状態へ
		commandList->ResourceBarrier(1, &barrierDesc);

		// 命令のクローズ
		result = commandList->Close();
		assert(SUCCEEDED(result));
		// コマンドリストの実行
		ID3D12CommandList* commandLists[] = { commandList };
		commandQueue->ExecuteCommandLists(1, commandLists);
		// 画面に表示するバッファをフリップ(裏表の入替え)
		result = swapChain->Present(1, 0);
		assert(SUCCEEDED(result));

		// コマンドの実行完了を待つ
		commandQueue->Signal(fence, ++fenceVal);
		if (fence->GetCompletedValue() != fenceVal) {
			HANDLE event = CreateEvent(nullptr, false, false, nullptr);
			fence->SetEventOnCompletion(fenceVal, event);
			WaitForSingleObject(event, INFINITE);
			CloseHandle(event);
		}
		// キューをクリア
		result = commandAllocator->Reset();
		assert(SUCCEEDED(result));
		// 再びコマンドリストを貯める準備
		result = commandList->Reset(commandAllocator, nullptr);
		assert(SUCCEEDED(result));

		// DirectX毎フレーム処理 ここまで///////////////////////////////////////////////////////////////////////////
	}

#pragma endregion

#pragma region WindowsAPI後始末
	// ウィンドウクラスを登録解除
	UnregisterClass(w.lpszClassName, w.hInstance);

#pragma endregion

#pragma region delete処理 
	delete input;
	delete matrix4;

#pragma endregion

	return 0;
}
