//全ての敵のマネージャークラス
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
	void Draw2();

	void PopEnemy(Vector3 pos);
	void PopSlime(Vector3 pos);
	void ResetIsHit();

	std::vector<BaseEnemy*> GetEnemysList() {
		return enemysList_;
	}

private:
	std::vector<BaseEnemy*> enemysList_;
};
