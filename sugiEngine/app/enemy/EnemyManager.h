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

	void PopEnemy(Vector3 pos = Vector3(0,-1,0));
	void ResetIsHit();

	std::vector<Enemy*> GetEnemysList() {
		enemysList_.clear();
		for (std::unique_ptr<Enemy>& enemy : enemys_) {
			enemysList_.push_back(enemy.get());
		}
		return enemysList_;
	}

	uint32_t GetEnemyCount() {
		return enemyCount_;
	}

private:
	std::list<std::unique_ptr<Enemy>> enemys_;
	std::vector<Enemy*> enemysList_;

	uint32_t enemyCount_;
};