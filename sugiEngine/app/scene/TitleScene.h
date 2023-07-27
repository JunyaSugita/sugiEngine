#pragma once
#include "ISceneManager.h"
#include "Model.h"
#include "Object3d.h"
#include "Sprite.h"
#include "Input.h"
#include "GameManager.h"
#include "Sound.h"

class TitleScene : public ISceneManager
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

private:
	std::unique_ptr<Model> sphereModel_[2];

	std::unique_ptr<Object3d> sphereObj_[2];
	WorldTransform sphereWorldTransform_[2];

	std::unique_ptr<LightGroup> lightGroup_ = nullptr;

	Sound sound_;
};

