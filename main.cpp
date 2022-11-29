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
#include "Sprite.h"

//エイリアステンプレート
template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

//windowsのエントリーポイント
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	std::unique_ptr<WinApp> winApp = std::make_unique<WinApp>();
	std::unique_ptr<DXCommon> dxCom = std::make_unique<DXCommon>();
	std::unique_ptr<Input> input = std::make_unique<Input>();
	std::unique_ptr <Matrix4> matrix4 = std::make_unique <Matrix4>();

	Object3d box;
	Object3d box2;

	Sprite sprite;
	Sprite sprite2;

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

	Object3d::StaticInitialize(dxCom->GetDevice());

	box.Initialize();
	box2.Initialize();

	Sprite::StaticInitialize(dxCom->GetDevice());

	sprite.Initialize();
	sprite2.Initialize();

#pragma endregion

#pragma region ゲームループ

	// ゲームループ
	while (true) {
		if (winApp->ProcMessage()) {
			break;
		}
		// DirectX毎フレーム処理 ここから///////////////////////////////////////////////////////////////////////////

		//キーボード入力
		input->Update();

		dxCom->PreDraw();

#pragma region ゲームシーン

		box.Trans(20.0f, -5.0f, 30.0f);
		box.Update();

		box2.Trans(-20.0f, 5.0f, -10.0f);
		box2.Update();

		Object3d::PreDraw(dxCom->GetCommandList());

		box.Draw(0);
		box2.Draw(1);

		Object3d::PostDraw();

		Sprite::PreDraw(dxCom->GetCommandList());

		sprite.Draw();
		sprite2.Draw();

		Sprite::PostDraw();

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
