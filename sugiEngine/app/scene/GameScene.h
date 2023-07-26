#pragma once
#include "ISceneManager.h"
#include "Model.h"
#include "Object3d.h"
#include "Sprite.h"
#include "Input.h"
#include "GameManager.h"
#include "Sound.h"
#include "LightGroup.h"
#include "ImGuiManager.h"
#include "FBX.h"
#include "ParticleManager.h"
#include "ParticleEditor.h"
#include "ClearChecker.h"
#include "GameOver.h"

class GameScene : public ISceneManager
{
public:
	void Initialize() override;
	void Update() override;
	void BackSpriteDraw() override;
	void Draw() override;
	void ObjDraw() override;
	void ParticleDraw() override;
	void SpriteDraw() override;
	void Finalize() override;

public:

private:
	//ƒ‰ƒCƒgŠÖ˜A
	std::unique_ptr<LightGroup> lightGroup_ = nullptr;

	std::unique_ptr<ParticleEditor> particleE_ = nullptr;

	ClearChecker clearChecker_;
	GameOver gameOver_;
};

