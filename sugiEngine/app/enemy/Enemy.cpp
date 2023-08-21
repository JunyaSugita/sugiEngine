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
	eyeObj_.obj->SetEffectCross();
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
		if (colM->CanMovePlayerVec(obj_.pos)) {
			temp.x = Player::GetInstance()->GetBoxCol().pos.x;
			temp.y = Player::GetInstance()->GetBoxCol().pos.z;
		}
		else {
			temp.x = navePointM->GetNavePoint(colM->CanMoveNavePointVec(obj_.pos)).pos.x;
			temp.y = navePointM->GetNavePoint(colM->CanMoveNavePointVec(obj_.pos)).pos.z;
		}

		toPlayer = Vector2(temp.x - obj_.pos.x, temp.y - obj_.pos.z);
		toPlayer.normalize();
		obj_.pos.x += toPlayer.x * SPEED_MOVE * GetSlow();
		obj_.pos.z += toPlayer.y * SPEED_MOVE * GetSlow();
	}
	else {
		isStop_ = false;
	}
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
	}
	alpha_ -= 0.005f;
	obj_.obj->SetColor({1,1,1,alpha_});
	eyeObj_.obj->SetColor({ 1,1,1,alpha_ });
	armL_.obj->SetColor({ 1,1,1,alpha_ });
	armR_.obj->SetColor({ 1,1,1,alpha_ });
	//ç≈å„
	BaseEnemy::Down();
}
