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

class GameScene : public ISceneManager
{
public:
	void Initialize() override;
	void Update() override;
	void BackSpriteDraw() override;
	void Draw() override;
	void ObjDraw() override;
	void SpriteDraw() override;
	void Finalize() override;

public:

private:
	//モデル
	std::unique_ptr<Model> groundModel_;
	std::unique_ptr<Object3d> groundObj_;
	WorldTransform groundWorldTransform_;

	//ライト関連
	unique_ptr<LightGroup> lightGroup_ = nullptr;
};

