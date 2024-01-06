#pragma once
#include "SugiMath.h"
#include "BaseCol.h"
#include <forward_list>

class ColliderManager
{
private:
	ColliderManager() = default;
	~ColliderManager() = default;

public:
	//コピーコンストラクタ無効
	ColliderManager(const ColliderManager& obj) = delete;
	//代入演算子を無効
	ColliderManager& operator=(const ColliderManager& obj) = delete;

	static ColliderManager* GetInstance();

public:
	void Initialize();
	void Update();
	void Draw();

	/// <summary>
	/// 判定を追加
	/// </summary>
	/// <param name="baseCol">追加したい当たり判定</param>
	void AddCollider(BaseCol* baseCol);

	/// <summary>
	/// 当たり判定を削除
	/// </summary>
	/// <param name="baseCol">削除したい当たり判定</param>
	void DeleteCollider(BaseCol* baseCol);

	/// <summary>
	/// ヒットボックス表示の切り替え
	/// </summary>
	void ToggleShowHitBox() {
		showHitBox_ = (showHitBox_ + 1) % 2;
	}

	/// <summary>
	/// 敵からプレイヤーに直線で進めるか
	/// </summary>
	/// <param name="pos">敵の現在位置</param>
	/// <returns>進めるか</returns>
	bool CanMoveEnemyToPlayer(Vector3 pos);

private:
	/// <summary>
	/// プレイヤーと壁の判定
	/// </summary>
	/// <param name="a">obj1</param>
	/// <param name="b">obj2</param>
	void CheckHitPlayerToWall(BaseCol* a, BaseCol* b);

	/// <summary>
	/// 敵と壁の判定
	/// </summary>
	/// <param name="a">obj1</param>
	/// <param name="b">obj2</param>
	void CheckHitEnemyToWall(BaseCol* a, BaseCol* b);

	/// <summary>
	/// 呪文と壁の判定
	/// </summary>
	/// <param name="a">obj1</param>
	/// <param name="b">obj2</param>
	void CheckHitSpellToWall(BaseCol* a, BaseCol* b);

	/// <summary>
	/// 呪文と敵の判定
	/// </summary>
	/// <param name="a">obj1</param>
	/// <param name="b">obj2</param>
	void CheckHitSpellToEnemy(BaseCol* a, BaseCol* b);

	/// <summary>
	/// プレイヤーと敵の判定
	/// </summary>
	/// <param name="a">obj1</param>
	/// <param name="b">obj2</param>
	void CheckHitPlayerToEnemy(BaseCol* a, BaseCol* b);

	/// <summary>
	/// 敵と敵の判定
	/// </summary>
	/// <param name="a">obj1</param>
	/// <param name="b">obj2</param>
	void CheckHitEnemyToEnemy(BaseCol* a, BaseCol* b);

	/// <summary>
	/// プレイヤーとゴールの判定
	/// </summary>
	/// <param name="a">obj1</param>
	/// <param name="b">obj2</param>
	void CheckHitPlayerToGoal(BaseCol* a, BaseCol* b);

	/// <summary>
	/// プレイヤーと壁が当たった時
	/// </summary>
	/// <param name="a">obj1</param>
	/// <param name="b">obj2</param>
	void HitPlayerToWall(BaseCol* a, BaseCol* b);

	/// <summary>
	/// 敵と壁が当たった時
	/// </summary>
	/// <param name="a">obj1</param>
	/// <param name="b">obj2</param>
	void HitEnemyToWall(BaseCol* a, BaseCol* b);

	/// <summary>
	/// 呪文と壁の判定
	/// </summary>
	/// <param name="a">obj1</param>
	/// <param name="b">obj2</param>
	void HitSpellToWall(BaseCol* a, BaseCol* b);

	/// <summary>
	/// 呪文と敵の判定
	/// </summary>
	/// <param name="a">obj1</param>
	/// <param name="b">obj2</param>
	void HitSpellToEnemy(BaseCol* a, BaseCol* b);

	/// <summary>
	/// 敵と敵が当たった時
	/// </summary>
	/// <param name="a">obj1</param>
	/// <param name="b">obj2</param>
	void HitEnemyToEnemy(BaseCol* a, BaseCol* b);

	/// <summary>
	/// プレイヤーと敵の判定
	/// </summary>
	/// <param name="a">obj1</param>
	/// <param name="b">obj2</param>
	void HitPlayerToEnemy(BaseCol* a, BaseCol* b);

	/// <summary>
	/// プレイヤーとゴールの判定
	/// </summary>
	/// <param name="a">obj1</param>
	/// <param name="b">obj2</param>
	void HitPlayerToGoal(BaseCol* a, BaseCol* b);
	


private:
	bool CheckHitBox(Col a, Col b);
	bool CheckHitCircle(Col a, Col b);

	bool CheckHitX(Col a, Col b);
	bool CheckHitY(Col a, Col b);
	bool CheckHitZ(Col a, Col b);

private:
	//デバフの時間
	const int32_t DEBUFF_TIME = 3;
	const int32_t TIME_SHAKE = 10;
	//押し出し処理
	const float PUSH_LEN = 0.01f;
	//仮敵の判定
	const Vector3 TEMP_ENEMY_HITBOX = { 0.8f,0.1f,0.8f };

	std::forward_list<BaseCol*> colliders_;

	bool showHitBox_ = false;
};

