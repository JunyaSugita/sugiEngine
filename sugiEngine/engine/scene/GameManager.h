/**
 * @file GameManager.h
 * @brief ゲーム全体を管理するマネージャー
 */

#pragma once
#include "IScene.h"
#include <memory>
#include "Camera.h"
#include "TitleScene.h"
#include "StageSelectScene.h"
#include "GameScene.h"
#include "ClearScene.h"

class GameManager final
{
private:
	GameManager();
	~GameManager() = default;

public:
	//コピーコンストラクタ無効
	GameManager(const GameManager& obj) = delete;
	//代入演算子を無効
	GameManager& operator=(const GameManager& obj) = delete;

	static GameManager* GetInstance();

public:
	void Update();
	void BackSpriteDraw();
	void Draw();
	void ObjDraw();
	void ObjDraw2();
	void ParticleDraw();
	void SpriteDraw();
	void Finalize();
	
	void ChangeScene(std::unique_ptr <IScene> scene);

private:
	std::unique_ptr<IScene> scene_;
};

