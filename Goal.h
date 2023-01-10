#pragma once
#include "Model.h"
#include "Object3d.h"


class Goal
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

	void SetIsSet(bool is) {
		isSet_ = is;
	}

	void SetPos(Vector3 pos) {
		worldTransform_.trans = pos;
	};

private:
	//モデルデータ
	Model* modelGoal_;
	Object3d* objectGoal_;

	bool isSet_ = false;

	WorldTransform worldTransform_;
};

