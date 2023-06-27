#pragma once
#include "Enemy.h"

class EnemyManager final{
private:
	EnemyManager();
	~EnemyManager();

public:
	//�R�s�[�R���X�g���N�^����
	EnemyManager(const EnemyManager& obj) = delete;
	//������Z�q�𖳌�
	EnemyManager& operator=(const EnemyManager& obj) = delete;

	static EnemyManager* GetInstance();

public:
	void Initialize();
	void Update();
	void Draw();

	void Set(Vector3 pos = Vector3(0,0,0));

private:
	std::list<std::unique_ptr<Enemy>> enemys_;
};