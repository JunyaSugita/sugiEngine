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
#include "ParticleEditer.h"

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
	unique_ptr<LightGroup> lightGroup_ = nullptr;

	unique_ptr<ParticleEditer> particleE_ = nullptr;
};

