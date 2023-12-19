#include "Fly.h"
#include "Player.h"
#include "ImGuiManager.h"
#include "EffectManager.h"
#include "ParticleManager.h"
#include "Tutorial.h"
#include "NavePointManager.h"
#include "ModelManager.h"

void Fly::Initialize(std::string name, Vector3 pos)
{
	armL_.Initialize("box");
	armL_.worldTrans.parent_ = &obj_.worldTrans;
	armL_.pos = { 0,0,-1 };
	armL_.rot = { 0,0,0 };
	armL_.scale = { 0.8f,0.3f,1 };

	armR_.Initialize("box");
	armR_.worldTrans.parent_ = &obj_.worldTrans;
	armR_.pos = { 0,0,1 };
	armR_.rot = { 0,0,0 };
	armR_.scale = { 0.8f,0.3f,1 };


	life_ = MAX_HP;
	angleSpeed_ = SPEED_ANGLE;
	height_ = HEIGHT_COL;

	BaseEnemy::Initialize(name, pos);
	WorldTransUpdate();

	obj_.pos.y = 10;
	obj_.scale.x = 2;

	col_.gap.y = 0.0f;
	col_.col.size.x = 2;
	col_.col.size.y = 1;
	col_.col.size.z = 2;

	obj_.obj->SetColor({ 0.1f,0.1f,0.1f,1 });
	armL_.obj->SetColor({ 0.1f,0.1f,0.1f,1 });
	armR_.obj->SetColor({ 0.1f,0.1f,0.1f,1 });
}

void Fly::Draw()
{
	BaseEnemy::Draw();
	armL_.Draw();
	armR_.Draw();
}

void Fly::WorldTransUpdate()
{
	armL_.Update();
	armR_.Update();

	BaseEnemy::WorldTransUpdate();
}

void Fly::Move()
{
	ColliderManager* colM = ColliderManager::GetInstance();
	NavePointManager* navePointM = NavePointManager::GetInstance();

	if (obj_.pos.y != 10) {
		isDead_ = true;
	}

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

void Fly::DontMoveUpdate()
{
	if (debuff_.isIce) {
		obj_.pos.y -= 0.5f;
	}

	if (obj_.pos.y <= 0) {
		debuff_.iceTime = 0;
	}

	col_.col.pos = obj_.pos;
}

void Fly::Attack()
{
	attackTimer_ += 0.25f;
	armL_.rot.x = EaseOut(attackTimer_, 50);
	armR_.rot.x = -EaseOut(attackTimer_, 50);
}

void Fly::Down()
{
	if (col_.col.pos.y > 1) {
		col_.col.pos.y -= 0.2f;
	}
	else {
		col_.col.pos.y = 1;
	}
	obj_.pos = col_.col.pos;

	//最後
	BaseEnemy::Down();
}