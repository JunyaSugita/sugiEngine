#include "Player.h"
#include "ImGuiManager.h"
#include "EffectManager.h"
#include "ParticleManager.h"
#include "Tutorial.h"
#include "NavePointManager.h"
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


	armL_.Initialize("box");
	armL_.worldTrans.parent_ = &obj_.worldTrans;
	armL_.pos = { 1,3,-1 };
	armL_.rot = { 0,0,60 };
	armL_.scale = { 1,0.3f,0.3f };

	armR_.Initialize("box");
	armR_.worldTrans.parent_ = &obj_.worldTrans;
	armR_.pos = { 1,3,1 };
	armR_.rot = { 0,0,60 };
	armR_.scale = { 1,0.3f,0.3f };


	life_ = MAX_HP;
	angleSpeed_ = SPEED_ANGLE;
	height_ = HEIGHT_COL;
	alpha_ = 1.0f;

	BaseEnemy::Initialize(name, pos);
	WorldTransUpdate();

	obj_.obj->SetColor({ 0.1f,0.1f,0.1f,1 });
	eyeObj_.obj->SetColor({ 0.1f,0.1f,0.1f,1 });
	armL_.obj->SetColor({ 0.1f,0.1f,0.1f,1 });
	armR_.obj->SetColor({ 0.1f,0.1f,0.1f,1 });
}

void Enemy::Draw()
{
	BaseEnemy::Draw();
	eyeObj_.Draw();
	armL_.Draw();
	armR_.Draw();
}

void Enemy::WorldTransUpdate()
{
	eyeObj_.Update();
	armL_.Update();
	armR_.Update();

	BaseEnemy::WorldTransUpdate();
}

void Enemy::Move()
{
	ColliderManager* colM = ColliderManager::GetInstance();
	NavePointManager* navePointM = NavePointManager::GetInstance();

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
			int32_t point = colM->CanMoveNavePointVec(obj_.pos);
			//ナビポイントが見つからなければ移動しない
			if (point == -1) {
				return;
			}
			//isStartがfalseなら止まる
			if (!isStart_) {
				return;
			}

			temp.x = navePointM->GetNavePoint(point).pos.x;
			temp.y = navePointM->GetNavePoint(point).pos.z;
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
		attackTimer_ -= 0.025f;
		if (attackTimer_ <= 0) {
			isAttack_ = false;
		}
	}
	armL_.rot.z = 50 - EaseOut(attackTimer_, 50);
	armR_.rot.z = 50 - EaseOut(attackTimer_, 50);
}

void Enemy::Down()
{
	if (obj_.rot.z < 90) {
		obj_.rot.z += 5;
		eyeObj_.obj->SetEffectCross(false);
	}

	//最後
	BaseEnemy::Down();
}
