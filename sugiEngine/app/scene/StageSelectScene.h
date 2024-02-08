/**
 * @file StageSelectScene.h
 * @brief ステージを選ばせる
 */

#pragma once
#include "GameManager.h"
#include "IScene.h"
#include "LightGroup.h"


class StageSelectScene : public IScene
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

private:
	//ライト関連
	std::unique_ptr<LightGroup> lightGroup_ = nullptr;
};
