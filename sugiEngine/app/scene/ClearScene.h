/**
 * @file ClearScene.h
 * @brief ステージをクリアした時に呼ばれる
 */

#pragma once
#include "ISceneManager.h"
#include "Object3d.h"
#include "Sprite.h"
#include "GameManager.h"
#include "LightGroup.h"
#include "Input.h"
#include "JsonLoader.h"
#include <map>


class ClearScene : public ISceneManager
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
	LevelData* levelData_;

	std::unique_ptr<Model> sphereModel_;
	std::unique_ptr<Model> playerModel_;
	std::map<std::string, Model*> models_;

	WorldTransform worldTransform_;
	std::vector<Object3d*> objects_;

	//スポーンポイント
	Vector3 spawnPoint_;
	int32_t spawnNum_;
	
	int32_t playerNum_;

	std::unique_ptr<LightGroup> lightGroup_ = nullptr;
};

