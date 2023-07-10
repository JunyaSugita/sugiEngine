#include "SugiFramework.h"

void SugiFramework::Initialize()
{
	winApp_ = make_unique<WinApp>();
	dxCom_ = make_unique<DXCommon>();
	matrix4_ = make_unique <Matrix4>();

#pragma region windowsAPI初期化処理
	winApp_->CreateWindowScreen();
	//コンソールへ文字入力
	OutputDebugStringA("Hello DirectX\n");
	OutputDebugStringA("This is sugiEngine\n");
#pragma endregion

#pragma region DirectX初期化処理
	// DirectX初期化処理 ここから
	dxCom_->Initialize(winApp_.get());
	//キーボード入力の初期化
	Input::GetInstance()->Initialize(winApp_.get());
	//FbxLoder初期化
	FbxLoader::GetInstance()->Initialize(dxCom_->GetDevice());

#pragma endregion

#pragma region 描画初期化処理
	Camera::GetInstance()->Initialize();
	Object3d::StaticInitialize(dxCom_->GetDevice());
	Particle::StaticInitialize(dxCom_->GetDevice());
	Fbx::SetDevice(dxCom_->GetDevice());
	Fbx::CreateGraphicsPipeline();
	Sprite::StaticInitialize(dxCom_->GetDevice());
	LightGroup::StaticInitialize(dxCom_->GetDevice());
#pragma endregion

	//PostEffect
	postEffect = make_unique <PostEffect>();
	uint32_t postNum = Sprite::LoadTexture("white1x1.png");

	postEffect->Initialize(dxCom_->GetDevice());
}

void SugiFramework::Finalize()
{
	//解放処理
	FbxLoader::GetInstance()->Finalize();

#pragma region WindowsAPI後始末
	//最後にする
	winApp_->DeleteWindow();
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
	if (winApp_->ProcMessage()) {
		return true;
	}
	return false;
}