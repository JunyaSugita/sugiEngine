/**
 * @file ColliderManager.h
 * @brief 当たり判定を管理するマネージャークラス
 */

#pragma once
#include "SugiMath.h"

struct BoxCol{
	Vector3 pos;
	Vector3 size;
};

class ColliderManager final{
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

	/// <summary>
	/// ヒットボックスを表示しているかを返す
	/// </summary>
	/// <returns>ヒットボックスを表示しているか</returns>
	bool GetIsShowHitBox() {
		return isShowHitBox_;
	}

	/// <summary>
	/// ヒットボックスの表示と非表示を切り替える
	/// </summary>
	void ChangeIsShowHitBox() {
		isShowHitBox_ = (isShowHitBox_ + 1) % 2;
	}

	/// <summary>
	/// チェインライトニングが当たっているか判定
	/// </summary>
	/// <returns>チェインライトニングが当たっているか</returns>
	bool CheckHitEnemyToChainLightning();

	/// <summary>
	/// 敵から自分までまっすぐ進むことができるか判定
	/// </summary>
	/// <param name="pos">敵の位置</param>
	/// <returns>敵から自分までまっすぐ進むことができるか</returns>
	bool CanMovePlayerVec(Vector3 pos);

	/// <summary>
	/// 敵からプレイヤーまでの最短でまっすぐナビポイントに行ける場所を探索
	/// </summary>
	/// <param name="pos">敵の位置</param>
	/// <returns>敵からプレイヤーまでの最短でまっすぐナビポイントに行ける場所</returns>
	int32_t CanMoveNaviPointVec(Vector3 pos);

	/// <summary>
	/// プレイヤーから各ナビポイントまでの距離を計算
	/// </summary>
	void SetNaviPointScore();

private:
	/// <summary>
	/// aとbが当たっているか判定
	/// </summary>
	/// <param name="a">判定したいもの1</param>
	/// <param name="b">判定したいもの2</param>
	/// <returns>当たっているか</returns>
	
	bool CheckWayX(BoxCol a, BoxCol b);
	bool CheckWayY(BoxCol a, BoxCol b);
	bool CheckWayZ(BoxCol a, BoxCol b);

	bool CheckHitX(BoxCol a, BoxCol b);
	bool CheckHitY(BoxCol a, BoxCol b);
	bool CheckHitZ(BoxCol a, BoxCol b);

	bool CheckHitBox(BoxCol a, BoxCol b);
	bool CheckHitCircle(BoxCol a, BoxCol b);

private:
	//デバフの時間
	const int32_t TIME_DEBUFF = 3;
	//シェイクの時間(フレーム)
	const int32_t TIME_SHAKE = 10;
	//押し出し処理
	const float PUSH_LEN = 0.01f;
	//仮高さ
	const float TEMP_Y = 0.03f;
	//仮敵の判定
	const Vector3 TEMP_ENEMY_HITBOX = {1.5f,0.01f,1.5f};

	//当たり判定表示のフラグ
	bool isShowHitBox_;
};
