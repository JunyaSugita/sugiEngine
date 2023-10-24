#pragma once
#include "ISceneManager.h"
#include "Model.h"
#include "Sprite.h"
#include "Input.h"
#include "GameManager.h"
#include "Sound.h"
#include "LightGroup.h"
#include "ImGuiManager.h"
#include "FBX.h"
#include "ParticleManager.h"
#include "ParticleEditor.h"
#include "GameOver.h"
#include "BaseObj.h"

class GameScene : public ISceneManager
{
public:
	void Initialize() override;
	void Update() override;
	void BackSpriteDraw() override;
	void Draw() override;
	void ObjDraw() override;
	void ObjDraw2() override;
	void ParticleDraw() override;
	void SpriteDraw() override;
	void Finalize() override;

	void AddStage() {
		stageNum_++;
	}

public:
	void GameInitialize();

private:
	//ライト関連
	std::unique_ptr<LightGroup> lightGroup_ = nullptr;

	GameOver gameOver_;

	Sound sound_;

	int32_t stageNum_;

	//シェーディング確認用
	BaseObj orb_;
	float dir_[3];
	float color_[3];
};

