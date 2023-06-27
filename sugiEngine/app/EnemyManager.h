#pragma once
#include "Enemy.h"

class EnemyManager final{
private:
	EnemyManager();
	~EnemyManager();

public:
	//コピーコンストラクタ無効
	EnemyManager(const EnemyManager& obj) = delete;
	//代入演算子を無効
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