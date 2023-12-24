/**
 * @file TitleScene.h
 * @brief タイトルで呼ばれる
 */

#pragma once
#include "ISceneManager.h"
#include "Sprite.h"
#include "Input.h"
#include "GameManager.h"
#include "Sound.h"
#include "BaseObj.h"
#include "ParticleEditor.h"

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
	std::unique_ptr<Object3d> sphereObj_[2];
	WorldTransform sphereWorldTransform_[2];

	std::unique_ptr<LightGroup> lightGroup_ = nullptr;

	Sound sound_;

	int32_t titleTex_;
	Sprite titleSp_;

	//本体
	BaseObj obj_;

	//上のオーブ
	BaseObj orbObj_;

	//パーティクルエディタ用
	float angle_ = 0;
	float length_ = 5;
};

