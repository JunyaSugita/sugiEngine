/**
 * @file EnemyManager.h
 * @brief 全ての敵のマネージャークラス
 */
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
	//ゲーム初期化
	void GameInitialize();
	void Update();
	void Draw();
	void DrawTransparent();
	
	//人型の敵を出す
	void PopEnemy(const Vector3& pos);
	//スライムを出す
	void PopSlime(const Vector3& pos);
	//虫を出す
	void PopFly(const Vector3& pos);
	//当たった判定のリセット
	void ResetIsHit();
	//フィールドにいる敵のリスト
	std::vector<BaseEnemy*> GetEnemysList() {
		return enemysList_;
	}

private:
	std::vector<BaseEnemy*> enemysList_;
};
