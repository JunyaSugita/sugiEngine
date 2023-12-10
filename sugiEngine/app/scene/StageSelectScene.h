#pragma once
#include "GameManager.h"
#include "ISceneManager.h"
#include "LightGroup.h"


class StageSelectScene : public ISceneManager
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
	//ライト関連
	std::unique_ptr<LightGroup> lightGroup_ = nullptr;
};
