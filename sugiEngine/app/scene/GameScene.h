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
	void Delete() override;

public:

private:
	//モデル
	Model* sphereModel_;
	Object3d* sphereObj_;
	WorldTransform sphereWorldTransform_;

	Model* groundModel_;
	Object3d* groundObj_;
	WorldTransform groundWorldTransform_;

	Model* boxModel_;
	Object3d* boxObj_;
	WorldTransform boxWorldTransform_;

	//スプライト
	uint32_t catTexture_;
	Sprite catSprite_;

	uint32_t dogTexture_;
	Sprite dogSprite_;

	FbxModel* model1_ = nullptr;
	Fbx* obj1_ = nullptr;

	//音
	std::unique_ptr<Sound> sound_ = std::make_unique <Sound>();

	//ライト関連
	LightGroup* lightGroup_ = nullptr;

	//丸影
	float circleShadowDir_[3] = { 0,-1,0 };
	float circleShadowAtten_[3] = { 0.5f,0.6f,0.0f };
	float circleShadowFactorAngle_[2] = { 0.0f,0.5f };

	float f_;
};

