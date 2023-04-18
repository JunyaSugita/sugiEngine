#include "SugiFramework.h"

void SugiFramework::Initialize()
{
	winApp = make_unique<WinApp>();
	dxCom = make_unique<DXCommon>();
	matrix4 = make_unique <Matrix4>();

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
	Input::GetInstance()->Initialize(winApp.get());
#pragma endregion

#pragma region 描画初期化処理
	Object3d::StaticInitialize(dxCom->GetDevice());
	Sprite::StaticInitialize(dxCom->GetDevice());
	LightGroup::StaticInitialize(dxCom->GetDevice());
#pragma endregion
}

void SugiFramework::Finalize()
{
#pragma region WindowsAPI後始末
	winApp->DeleteWindow();
#pragma endregion
}

void SugiFramework::Update()
{
	//キーボード入力
	Input::GetInstance()->Update();
}

void SugiFramework::Run()
{
	//ゲーム初期化
	Initialize();

	while (true)
	{
		//毎フレーム更新
		Update();
		//終了リクエストを取得
		if (GetIsEnd()) {
			break;
		}
		//描画
		Draw();
	}

	//ゲームの終了
	Finalize();
}

bool SugiFramework::GetIsEnd()
{
	if (winApp->ProcMessage()) {
		return true;
	}
	return false;
}