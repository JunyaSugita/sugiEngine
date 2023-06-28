#include "Enemy.h"
#include "Player.h"
#include "ImGuiManager.h"

void Enemy::Initialize()
{
	model_ = move(Model::LoadFromObj("player"));
	obj_ = move(Object3d::Create());
	obj_->SetModel(model_.get());

	eyeModel_ = move(Model::LoadFromObj("sphere",true));
	eyeObj_ = move(Object3d::Create());
	eyeObj_->SetModel(eyeModel_.get());

	pos_ = {0,0,0};
	rot_ = {0,90,0};
	scale_ = {1,1,1};

	isDead_ = false;

	eyeWorldTransform_.parent_ = &worldTransform_;
	eyePos_ = { 0.3f,4.1f,0 };
	eyeRot_ = { 0,0,0 };
	eyeScale_ = { 0.3f,0.3f,0.3f };

	WorldTransUpdate();

	isHit_ = false;
	life_ = 2;
}

void Enemy::Update()
{
	//プレイヤー情報の取得
	GetPlayer();
	//移動
	Move();

	//プレイヤーの方へゆっくり回転
	SetAngleToPlayer();

	//移動を適応
	WorldTransUpdate();


}

void Enemy::Draw()
{
	obj_->Draw();
	eyeObj_->Draw();
}

void Enemy::SetIsHit()
{
	//既に当たっていたら当たらない
	if (isHit_) {
		return;
	}
	else {
		//当たったフラグを立てる
		isHit_ = true;
		//体力を削る
		SubLife();
	}
}

void Enemy::WorldTransUpdate()
{
	worldTransform_.SetPos(pos_);
	worldTransform_.SetRot(rot_);
	worldTransform_.SetScale(scale_);

	eyeWorldTransform_.SetPos(eyePos_);
	eyeWorldTransform_.SetRot(eyeRot_);
	eyeWorldTransform_.SetScale(eyeScale_);

	SetWorldTrans();
}

void Enemy::SetWorldTrans()
{
	obj_->SetWorldTransform(worldTransform_);
	obj_->Update();
	eyeObj_->SetWorldTransform(eyeWorldTransform_);
	eyeObj_->Update();
}

void Enemy::SetAngleToPlayer()
{
	//インスタンス取得
	Player* player = Player::GetInstance()->GetInstance();

	//プレイヤーの向きを計算して単位化
	Vector2 len = Vector2((player->GetPos() - pos_).x, (player->GetPos() - pos_).z);
	len.normalize();

	//プレイヤーの方にゆっくり向く
	float angle_ = atan2(len.cross(UP), -len.dot(UP)) / PI * -RADIAN - (RADIAN / 2);
	while (rot_.y - angle_ > RADIAN || rot_.y - angle_ < -RADIAN) {
		if (rot_.y - angle_ > RADIAN) {
			rot_.y -= RADIAN * 2;
		}
		if (rot_.y - angle_ < -RADIAN) {
			rot_.y += RADIAN * 2;
		}
	}
	if (rot_.y - angle_ < 0) {
		rot_.y += SPEED_ANGLE;
		if (rot_.y - angle_ > 0) {
			rot_.y = angle_;
		}
	}
	else {
		rot_.y -= SPEED_ANGLE;
		if (rot_.y - angle_ < 0) {
			rot_.y = angle_;
		}
	}
}

void Enemy::GetPlayer()
{
	//インスタンス取得
	Player* player = Player::GetInstance()->GetInstance();

	toPlayer = Vector2((player->GetPos() - pos_).x, (player->GetPos() - pos_).z);

	//プレイヤーに近づいたら死ぬ
	if (toPlayer.length() <= 0.5f) {
		isDead_ = true;
		player->SubLife();
	}
}

void Enemy::Move()
{
	toPlayer.normalize();

	pos_.x += toPlayer.x * SPEED_MOVE;
	pos_.z += toPlayer.y * SPEED_MOVE;
}

void Enemy::SubLife()
{
	life_--;
	if (life_ <= 0) {
		isDead_ = true;
	}
}
