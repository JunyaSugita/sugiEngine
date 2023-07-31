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

	int32_t titleTex_;
	Sprite titleSp_;

	//本体
	WorldTransform worldTrans_;
	Vector3 pos_;
	Vector3 rot_;
	Vector3 scale_;

	//上のオーブ
	WorldTransform orbTrans_;
	Vector3 orbPos_;
	Vector3 orbRot_;
	Vector3 orbScale_;

	std::unique_ptr <Model> model_;
	std::unique_ptr <Object3d> obj_;

	std::unique_ptr <Model> orbModel_;
	std::unique_ptr <Object3d> orbObj_;
};

