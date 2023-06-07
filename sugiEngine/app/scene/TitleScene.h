#pragma once
#include "ISceneManager.h"
#include "Model.h"
#include "Object3d.h"
#include "Sprite.h"
#include "Input.h"
#include "GameManager.h"

class TitleScene : public ISceneManager
{
public:
	void Initialize() override;
	void Update() override;
	void BackSpriteDraw() override;
	void Draw() override;
	void ObjDraw() override;
	void SpriteDraw() override;
	void Delete() override;

private:
	Model* sphereModel_[2];

	Object3d* sphereObj_[2];
	WorldTransform sphereWorldTransform_[2];

	LightGroup* lightGroup_ = nullptr;
};

