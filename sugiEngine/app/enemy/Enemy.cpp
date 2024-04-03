#include "Player.h"
#include "ImGuiManager.h"
#include "EffectManager.h"
#include "Particle.h"
#include "Tutorial.h"
#include "NaviPointManager.h"
#include "ModelManager.h"
#include "Enemy.h"
#include "ColliderManager.h"

std::unique_ptr<Model> Enemy::sEyeModel_;

void Enemy::Initialize(const std::string& name, const Vector3& pos)
{
	//目のモデル
	eyeObj_.Initialize("eye");
	eyeObj_.obj->SetEffectCross(true);
	eyeObj_.worldTrans.parent_ = &obj_.worldTrans;
	eyeObj_.pos = { 0.5f,4.1f,0 };
	eyeObj_.scale = { 0.3f,0.3f,0.3f };

	//左腕
	armL_.Initialize("box");
	armL_.worldTrans.parent_ = &obj_.worldTrans;
	armL_.pos = { 1,3,-1 };
	armL_.rot = { 0,0,60 };
	armL_.scale = { 1,0.3f,0.3f };

	//右腕
	armR_.Initialize("box");
	armR_.worldTrans.parent_ = &obj_.worldTrans;
	armR_.pos = { 1,3,1 };
	armR_.rot = { 0,0,60 };
	armR_.scale = { 1,0.3f,0.3f };

	//体力を初期化
	life_ = MAX_HP;
	//振り向きスピード
	angleSpeed_ = SPEED_ANGLE;
	//縦の当たり判定の大きさ
	col_.size.y = SIZE_HEIGHT_COL;
	//縦の当たり判定のズレ
	gap_ = GAP_HEIGHT_COL;
	//自身の初期化
	BaseEnemy::Initialize(name, pos);
	//更新
	WorldTransUpdate();

	//色指定
	obj_.obj->SetColor({ 0.1f,0.1f,0.1f,1 });
	eyeObj_.obj->SetColor({ 0.1f,0.1f,0.1f,1 });
	armL_.obj->SetColor({ 0.1f,0.1f,0.1f,1 });
	armR_.obj->SetColor({ 0.1f,0.1f,0.1f,1 });
}

void Enemy::Draw()
{
	//モデル描画
	BaseEnemy::Draw();
	eyeObj_.Draw();
	armL_.Draw();
	armR_.Draw();
}

void Enemy::WorldTransUpdate()
{
	//モデル更新
	eyeObj_.Update();
	armL_.Update();
	armR_.Update();

	BaseEnemy::WorldTransUpdate();
}

void Enemy::Move()
{
	//足止めされているか
	if (!isStop_) {
		Vector2 temp;
		//プレイヤー方向に壁が無ければプレイヤー方向に移動
		if (ColliderManager::GetInstance()->CanMoveToPlayer(col_.pos)) {
			//プレイヤー位置を設定
			temp.x = Player::GetInstance()->GetBoxCol().pos.x;
			temp.y = Player::GetInstance()->GetBoxCol().pos.z;

			//起動してなければ起動
			isStart_ = true;
		}
		else {
			//起動しているかチェック
			if (isStart_) {
				//次に進むナビポイントを取得
				Vector3 tempPos = ColliderManager::GetInstance()->CanMoveEnemyToNaviPoint(col_.pos);
				//次に進むナビポイントへの位置を設定
				temp.x = tempPos.x;
				temp.y = tempPos.z;
			}
			else {
				//起動していない時は動かす処理をスキップ
				return;
			}
		}
		//設定した位置
		toPlayer = Vector2(temp.x - obj_.pos.x, temp.y - obj_.pos.z);
		//ベクトルの単位化
		toPlayer.normalize();

		//移動スピードとデバフを加味して実際に動かす
		obj_.pos.x += toPlayer.x * SPEED_MOVE * slow_;
		obj_.pos.z += toPlayer.y * SPEED_MOVE * slow_;
	}
	else {
		//足止め解除
		isStop_ = false;
	}

	//最後にスピード減少を初期化
	slow_ = 1.0f;
}

void Enemy::Attack()
{
	//攻撃中かどうか
	if (isAttack_) {
		//攻撃のクールタイムを進める
		attackTimer_ -= SUB_TIME_ATTACK;
		if (attackTimer_ <= 0) {
			//クールタイム0で攻撃中状態を解除
			isAttack_ = false;
		}
	}

	//腕を動かす
	armL_.rot.z = ROT_ARM - EaseOut(attackTimer_, ROT_ARM);
	armR_.rot.z = ROT_ARM - EaseOut(attackTimer_, ROT_ARM);
}

void Enemy::Down()
{
	//体が倒れ切っていなければ倒れる
	if (obj_.rot.z < RAD / 2) {
		//体を倒す
		obj_.rot.z += SPEED_DOWN;
		//目の光を消す
		eyeObj_.obj->SetEffectCross(false);
	}

	//最後
	BaseEnemy::Down();
}
