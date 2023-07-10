#pragma once
#include "ISceneManager.h"
#include <memory>
#include "Camera.h"

class GameManager
{
private:
	GameManager();
	~GameManager();

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
	void ParticleDraw();
	void SpriteDraw();
	void Finalize();
	
	void SetTitleScene();
	void SetGameScene();
	void SetClearScene();

private:
	std::unique_ptr<ISceneManager> scene_;
};

