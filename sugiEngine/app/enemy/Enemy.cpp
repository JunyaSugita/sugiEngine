#include "Player.h"
#include "ImGuiManager.h"
#include "EffectManager.h"
#include "ParticleManager.h"
#include "Tutorial.h"
#include "NaviPointManager.h"
#include "ModelManager.h"
#include "Enemy.h"

std::unique_ptr<Model> Enemy::sEyeModel_;

void Enemy::Initialize(std::string name, Vector3 pos)
{
	eyeObj_.Initialize("eye");
	eyeObj_.obj->SetEffectCross(true);
	eyeObj_.worldTrans.parent_ = &obj_.worldTrans;
	eyeObj_.pos = { 0.3f,4.1f,0 };
	eyeObj_.scale = { 0.3f,0.3f,0.3f };


	wingL_.Initialize("box");
	wingL_.worldTrans.parent_ = &obj_.worldTrans;
	wingL_.pos = { 1,3,-1 };
	wingL_.rot = { 0,0,60 };
	wingL_.scale = { 1,0.3f,0.3f };

	wingR_.Initialize("box");
	wingR_.worldTrans.parent_ = &obj_.worldTrans;
	wingR_.pos = { 1,3,1 };
	wingR_.rot = { 0,0,60 };
	wingR_.scale = { 1,0.3f,0.3f };


	life_ = MAX_HP;
	angleSpeed_ = SPEED_ANGLE;
	height_ = HEIGHT_COL;

	BaseEnemy::Initialize(name, pos);
	WorldTransUpdate();

	obj_.obj->SetColor({ 0.1f,0.1f,0.1f,1 });
	eyeObj_.obj->SetColor({ 0.1f,0.1f,0.1f,1 });
	wingL_.obj->SetColor({ 0.1f,0.1f,0.1f,1 });
	wingR_.obj->SetColor({ 0.1f,0.1f,0.1f,1 });
}

void Enemy::Draw()
{
	BaseEnemy::Draw();
	eyeObj_.Draw();
	wingL_.Draw();
	wingR_.Draw();
}

void Enemy::WorldTransUpdate()
{
	eyeObj_.Update();
	wingL_.Update();
	wingR_.Update();

	BaseEnemy::WorldTransUpdate();
}

void Enemy::Move()
{
	ColliderManager* colM = ColliderManager::GetInstance();
	NaviPointManager* navePointM = NaviPointManager::GetInstance();

	if (!isStop_) {
		Vector2 temp;
		//プレイヤー方向に壁が無ければプレイヤー方向に移動
		if (colM->CanMovePlayerVec(obj_.pos)) {
			temp.x = Player::GetInstance()->GetBoxCol().pos.x;
			temp.y = Player::GetInstance()->GetBoxCol().pos.z;

			//起動してなければ起動
			isStart_ = true;
		}
		else {
			int32_t point = colM->CanMoveNaviPointVec(obj_.pos);
			//ナビポイントが見つからなければ移動しない
			if (point == -1) {
				return;
			}
			//isStartがfalseなら止まる
			if (!isStart_) {
				return;
			}

			temp.x = navePointM->GetNaviPoint(point).pos.x;
			temp.y = navePointM->GetNaviPoint(point).pos.z;
		}

		toPlayer = Vector2(temp.x - obj_.pos.x, temp.y - obj_.pos.z);
		toPlayer.normalize();

		obj_.pos.x += toPlayer.x * SPEED_MOVE * slow_;
		obj_.pos.z += toPlayer.y * SPEED_MOVE * slow_;
	}
	else {
		isStop_ = false;
	}

	//最後にスピード減少を初期化
	slow_ = 1.0f;
}

void Enemy::Attack()
{
	if (isAttack_) {
		attackTimer_ -= SUB_TIME_ATTACK;
		if (attackTimer_ <= 0) {
			isAttack_ = false;
		}
	}
	wingL_.rot.z = ROT_ARM - EaseOut(attackTimer_, ROT_ARM);
	wingR_.rot.z = ROT_ARM - EaseOut(attackTimer_, ROT_ARM);
}

void Enemy::Down()
{
	if (obj_.rot.z < RAD / 2) {
		obj_.rot.z += SPEED_DOWN;
		eyeObj_.obj->SetEffectCross(false);
	}

	//最後
	BaseEnemy::Down();
}
