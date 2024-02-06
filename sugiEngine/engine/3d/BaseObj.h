/**
 * @file BaseObj.h
 * @brief オブジェクトの汎用クラス
 */

#pragma once
#include "GlobalSetting.h"
#include <string>
#include "Object3d.h"
#include "WorldTransform.h"

class BaseObj {
public:
	virtual void Initialize(const std::string& str);
	virtual void Update();
	virtual void Draw();

public:
	std::unique_ptr<Object3d> obj;
	std::unique_ptr<Model> model;
	WorldTransform worldTrans;
	Vector3 pos;
	Vector3 rot;
	Vector3 scale;
};
