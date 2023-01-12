#pragma once
#include "WorldTransform.h"
#include "Vector3.h"
#include "Model.h"
#include "Object3d.h"

class Burst {
public:
	void Initialize(Vector3 pos,Vector4 color, float range, float pow);
	void Update();
	void Draw();

	bool IsDead() const {
		return isDead_;
	}
private:
	//���W
	WorldTransform worldTransform_;
	//�ړ�
	Vector3 move_;
	//�F
	Vector4 color_;
	//��]
	Vector3 rotate_;
	//���f��
	Model* model_;
	//�e�N�X�`��
	Object3d* obj_;
	//���Ńt���O
	bool isDead_;
};
