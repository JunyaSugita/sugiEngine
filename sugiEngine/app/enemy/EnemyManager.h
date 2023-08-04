#pragma once
#include "Enemy.h"

class EnemyManager final{
private:
	EnemyManager() = default;
	~EnemyManager() = default;

public:
	//コピーコンストラクタ無効
	EnemyManager(const EnemyManager& obj) = delete;
	//代入演算子を無効
	EnemyManager& operator=(const EnemyManager& obj) = delete;

	static EnemyManager* GetInstance();

public:
	void Initialize();
	void GameInitialize();
	void Update();
	void Draw();

	void PopEnemy(Vector3 pos);
	void ResetIsHit();

	std::vector<BaseEnemy*> GetEnemysList();

	uint32_t GetEnemyCount() {
		return enemyCount_;
	}

private:
	std::list<std::unique_ptr<BaseEnemy>> enemys_;
	std::vector<BaseEnemy*> enemysList_;

	uint32_t enemyCount_;
};