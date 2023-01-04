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
	void Delete() override;

public:

private:
	//スプライトデータ
	uint32_t catTexture;
	Sprite sprite;

	std::unique_ptr <Player> player_;
	Map* map_;
};

