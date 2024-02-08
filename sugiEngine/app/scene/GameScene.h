/**
 * @file GameScene.h
 * @brief ゲーム中に呼ばれる
 */

#pragma once
#include "IScene.h"
#include "Model.h"
#include "Sprite.h"
#include "Input.h"
#include "GameManager.h"
#include "Sound.h"
#include "LightGroup.h"
#include "ImGuiManager.h"
#include "FBX.h"
#include "ParticleEditor.h"
#include "GameOver.h"
#include "BaseObj.h"

class GameScene : public IScene
{
public:
	void Initialize() override;
	void Update() override;
	void BackSpriteDraw() override;
	void Draw() override;
	void ObjDraw() override;
	void DrawTransparent() override;
	void ParticleDraw() override;
	void SpriteDraw() override;
	void Finalize() override;

	//次のステージへ
	void AddStage() {
		stageNum_++;
	}

public:
	//ゲームリセット時
	void GameInitialize();

private:
	//スタート時ゲーム音量
	const float START_BOV = 0.1f;
	//imGuiサイズ
	const ImVec2 SIZE_IMGUI = {150,30};

	//ライト関連
	std::unique_ptr<LightGroup> lightGroup_ = nullptr;

	GameOver gameOver_;

	Sound sound_;

	int32_t stageNum_;
};

