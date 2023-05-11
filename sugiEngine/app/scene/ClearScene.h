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
	void SpriteDraw() override;
	void Delete() override;

private:
	LevelData* levelData_;

	Model* sphereModel_;
	std::map<std::string, Model*> models;

	WorldTransform sphereWorldTransform_;
	std::vector<Object3d*> objects;

	LightGroup* lightGroup_ = nullptr;
};

