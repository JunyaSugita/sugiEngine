#pragma once
#include "ISceneManager.h"
#include "Model.h"
#include "Object3d.h"
#include "Sprite.h"
#include "Input.h"
#include "GameManager.h"

#include "Player.h"
#include "Map.h"

class GameScene : public ISceneManager
{
public:
	void Initialize() override;
	void Update(Input* input) override;
	void Draw() override;
	void SpriteDraw() override;

public:

private:
	//�X�v���C�g�f�[�^
	uint32_t catTexture;
	Sprite sprite;

	std::unique_ptr <Player> player_;
	std::unique_ptr <Map> map_;
};

