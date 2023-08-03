#include "Enemy.h"
#include "Player.h"
#include "ImGuiManager.h"
#include "EffectManager.h"
#include "ParticleManager.h"
#include "Tutorial.h"
#include "NavePointManager.h"

std::unique_ptr<Model> Enemy::sEyeModel_;

void Enemy::StaticInitialize()
{
	sModel_ = move(Model::LoadFromObj("player"));	
	sEyeModel_ = move(Model::LoadFromObj("sphere", true));
}

void Enemy::Initialize(Vector3 pos)
{
	eyeObj_ = move(Object3d::Create());
	eyeObj_->SetModel(sEyeModel_.get());
	eyeObj_->SetEffectCross();

	armLObj_ = move(Object3d::Create());
	armLObj_->SetModel(sColModel_.get());

	armRObj_ = move(Object3d::Create());
	armRObj_->SetModel(sColModel_.get());

	eyeWorldTrans_.parent_ = &worldTrans_;
	eyePos_ = { 0.3f,4.1f,0 };
	eyeRot_ = { 0,0,0 };
	eyeScale_ = { 0.3f,0.3f,0.3f };

	armRWorldTrans_.parent_ = &worldTrans_;
	armLWorldTrans_.parent_ = &worldTrans_;
	armRPos_ = { 1,3,1 };
	armLPos_ = { 1,3,-1 };
	armRot_ = { 0,0,60 };
	armScale_ = { 1,0.3f,0.3f };

	life_ = MAX_HP;
	angleSpeed_ = SPEED_ANGLE;
	height_ = HEIGHT_COL;

	BaseEnemy::Initialize(pos);
	WorldTransUpdate();
}

void Enemy::Draw()
{
	BaseEnemy::Draw();
	eyeObj_->Draw();
	armLObj_->Draw();
	armRObj_->Draw();
}

void Enemy::WorldTransUpdate()
{
	eyeWorldTrans_.SetPos(eyePos_);
	eyeWorldTrans_.SetRot(eyeRot_);
	eyeWorldTrans_.SetScale(eyeScale_);

	armRWorldTrans_.SetPos(armRPos_);
	armRWorldTrans_.SetRot(armRot_);
	armRWorldTrans_.SetScale(armScale_);

	armLWorldTrans_.SetPos(armLPos_);
	armLWorldTrans_.SetRot(armRot_);
	armLWorldTrans_.SetScale(armScale_);

	//•K‚¸ÅŒã‚É‹Lq
	BaseEnemy::WorldTransUpdate();
}

void Enemy::SetWorldTrans()
{
	BaseEnemy::SetWorldTrans();

	eyeObj_->SetWorldTransform(eyeWorldTrans_);
	eyeObj_->Update();
	armLObj_->SetWorldTransform(armLWorldTrans_);
	armLObj_->Update();
	armRObj_->SetWorldTransform(armRWorldTrans_);
	armRObj_->Update();
}

void Enemy::Move()
{
	ColliderManager* colM = ColliderManager::GetInstance();
	NavePointManager* navePointM = NavePointManager::GetInstance();

	if (!isStop_) {
		Vector2 temp;
		if (colM->CanMovePlayerVec(pos_)) {
			temp.x = Player::GetInstance()->GetBoxCol().pos.x;
			temp.y = Player::GetInstance()->GetBoxCol().pos.z;
		}
		else if(colM->CanMoveNavePointVec(1,pos_)){
			temp.x = navePointM->GetNavePoint(colM->GetMoveNavePointVec()).pos.x;
			temp.y = navePointM->GetNavePoint(colM->GetMoveNavePointVec()).pos.z;
		}
		else if (colM->CanMoveNavePointVec(2, pos_)) {
			temp.x = navePointM->GetNavePoint(colM->GetMoveNavePointVec()).pos.x;
			temp.y = navePointM->GetNavePoint(colM->GetMoveNavePointVec()).pos.z;
		}
		else if (colM->CanMoveNavePointVec(3, pos_)) {
			temp.x = navePointM->GetNavePoint(colM->GetMoveNavePointVec()).pos.x;
			temp.y = navePointM->GetNavePoint(colM->GetMoveNavePointVec()).pos.z;
		}
		else if (colM->CanMoveNavePointVec(4, pos_)) {
			temp.x = navePointM->GetNavePoint(colM->GetMoveNavePointVec()).pos.x;
			temp.y = navePointM->GetNavePoint(colM->GetMoveNavePointVec()).pos.z;
		}

		toPlayer = Vector2(temp.x - pos_.x, temp.y - pos_.z);
		toPlayer.normalize();
		pos_.x += toPlayer.x * SPEED_MOVE * GetSlow();
		pos_.z += toPlayer.y * SPEED_MOVE * GetSlow();
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
	armRot_.z = 50 - EaseOut(attackTimer_,50);
}
