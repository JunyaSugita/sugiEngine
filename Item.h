#pragma once
#include "Model.h"
#include "Object3d.h"

class Item
{
public:
	void Initialize();
	void Update();
	void Draw();

	bool GetIsSet() {
		return isSet_;
	}

	Vector3 GetPos() {
		return worldTransform_.trans;
	}

	void SetPos(Vector3 pos) {
		worldTransform_.trans = pos;
	};

	void SetIsSet(bool is) {
		isSet_ = is;
	}

	void SetIsUse(bool is);

private:
	//モデルデータ
	Model* modelItem_;
	Object3d* objectItem_;

	bool isSet_ = false;

	WorldTransform worldTransform_;
	bool isUse_;
};

