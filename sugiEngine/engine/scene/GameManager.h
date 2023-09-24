#pragma once
#include "ISceneManager.h"
#include <memory>
#include "Camera.h"

class GameManager final
{
private:
	GameManager();
	~GameManager() = default;

public:
	//�R�s�[�R���X�g���N�^����
	GameManager(const GameManager& obj) = delete;
	//������Z�q�𖳌�
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
	
	void SetTitleScene();
	void SetStageSelectScene();
	void SetGameScene();
	void SetClearScene();

private:
	std::unique_ptr<ISceneManager> scene_;
};

