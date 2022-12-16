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
#include "Model.h"

//エイリアステンプレート
template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

//windowsのエントリーポイント
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	std::unique_ptr<WinApp> winApp = std::make_unique<WinApp>();
	std::unique_ptr<DXCommon> dxCom = std::make_unique<DXCommon>();
	std::unique_ptr<Input> input = std::make_unique<Input>();
	std::unique_ptr <Matrix4> matrix4 = std::make_unique <Matrix4>();

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

	Model* model = Model::LoadFromObj("box");
	Model* model2 = Model::LoadFromObj("skydome");

	Object3d* object3d = Object3d::Create();
	Object3d* object3d2 = Object3d::Create();

	object3d->SetModel(model);
	object3d2->SetModel(model2);

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

		if (input->PushKey(DIK_W)) {
			Object3d::AddCameraPos(Vector3(0, 1, 0));
		}
		if (input->PushKey(DIK_S)) {
			Object3d::AddCameraPos(Vector3(0, -1, 0));
		}
		if (input->PushKey(DIK_D)) {
			Object3d::AddCameraPos(Vector3(1, 0, 0));
		}
		if (input->PushKey(DIK_A)) {
			Object3d::AddCameraPos(Vector3(-1, 0, 0));
		}
		if (input->PushKey(DIK_R)) {
			Object3d::AddCameraPos(Vector3(0, 0, 1));
		}
		if (input->PushKey(DIK_F)) {
			Object3d::AddCameraPos(Vector3(0, 0, -1));
		}

		if (input->PushKey(DIK_UP)) {
			Object3d::AddCameraTarget(Vector3(0, 0, 1));
		}
		if (input->PushKey(DIK_DOWN)) {
			Object3d::AddCameraTarget(Vector3(0, 0, -1));
		}
		if (input->PushKey(DIK_RIGHT)) {
			Object3d::AddCameraTarget(Vector3(1, 0, 0));
		}
		if (input->PushKey(DIK_LEFT)) {
			Object3d::AddCameraTarget(Vector3(-1, 0, 0));
		}

		object3d->Scale(30, 30, 30);
		object3d->Trans(10, 10, 10);
		object3d->Rotate(0,-45,45);
		object3d->Update();

		object3d2->Scale(5,5,5);
		object3d2->Update();

		Object3d::PreDraw(dxCom->GetCommandList());

		object3d->Draw();
		object3d2->Draw();

		Object3d::PostDraw();

		sprite.Pos(100, 100);

		sprite2.Pos(300, 300);
		sprite2.Color(1, 0, 0, 1);
		sprite2.Size(200.0f, 200.0f);
		sprite2.SetAnchorPoint(0.5f, 0.5f);
		sprite2.Rotate(45);
		sprite2.FlipX(true);

		//スプライト
		Sprite::PreDraw(dxCom->GetCommandList());

		//sprite.Draw(0);
		//sprite2.Draw(1);

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
	delete object3d;
	delete object3d2;

	delete model;
	delete model2;

#pragma endregion

	return 0;
}
