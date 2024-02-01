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
	/// プレイヤー方向に直線で進めるか
	/// </summary>
	/// <param name="pos">現在位置</param>
	/// <param name="col">当たり判定</param>
	/// <returns>進めるか</returns>
	bool CanMoveToPlayer(Vector3 pos);

	/// <summary>
	/// ナビポイント同士が繋がるか
	/// </summary>
	/// <param name="pos1">ナビポイント1</param>
	/// <param name="pos2">ナビポイント2</param>
	/// <param name="col">当たり判定</param>
	/// <returns>繋がるか</returns>
	bool CanMoveToNaviPoint(Vector3 pos1, Vector3 pos2);

	/// <summary>
	/// 敵とナビポイントが繋がるか
	/// </summary>
	/// <param name="pos1">敵の位置</param>
	/// <param name="col">当たり判定</param>
	/// <returns>繋げるナビポイント</returns>
	Vector3 CanMoveEnemyToNaviPoint(Vector3 pos1);
	
	/// <summary>
	/// 向いている方向に敵がいるか(壁を考慮)
	/// </summary>
	/// <param name="pos">レイの視点</param>
	/// <param name="ray">レイの方向ベクトル</param>
	/// <returns>敵がいるか</returns>
	bool CheckHitEnemyToRay(Vector3 pos,Vector3 ray);

	/// <summary>
	/// 向いている方向の敵の位置
	/// </summary>
	/// <param name="pos">レイの視点</param>
	/// <param name="ray">レイの方向ベクトル</param>
	/// <returns>敵の位置</returns>
	Vector3 CheckHitPosEnemyToRay(Vector3 pos, Vector3 ray);

	/// <summary>
	/// 特定の敵が近くにいるか
	/// </summary>
	/// <param name="num">調べたい特定の敵の番号</param>
	bool CheckNearEnemy(int32_t num);

	/// <summary>
	/// ライトニングに当たった敵のまわりに伝播
	/// </summary>
	/// <param name="pos"></param>
	void LightningEnemyToEnemy(Vector3 pos);
	
public:
	bool CheckHitBox(Col a, Col b);
	bool CheckHitCircle(Col a, Col b);

	bool CheckHitX(Col a, Col b);
	bool CheckHitY(Col a, Col b);
	bool CheckHitZ(Col a, Col b);

	bool CheckHitLineToBox(Vector3 posS,Vector3 posE,Col a);
	Vector3 CheckHitPosLineToBox(Vector3 posS, Vector3 posE, Col a);

private:
	//仮敵の判定
	const Vector3 TEMP_ENEMY_HITBOX = { 0.8f,0.1f,0.8f };

	std::forward_list<BaseCol*> colliders_;

	bool showHitBox_ = false;
};

