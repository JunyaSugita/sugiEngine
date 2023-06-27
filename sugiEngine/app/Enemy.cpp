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
}

void Enemy::Update()
{
	//インスタンス取得
	Player* player = Player::GetInstance()->GetInstance();
	
	Vector2 len = Vector2((player->GetPos() - pos_).x, (player->GetPos() - pos_).z);
	len.normalize();

	//プレイヤーに近づいたら死ぬ
	if (len.length() <= 0.5f) {
		isDead_ = true;
	}
	pos_.x += len.x * SPEED_MOVE;
	pos_.z += len.y * SPEED_MOVE;
	
	//プレイヤーの方を向く
	float angle_ = atan2(len.cross({ 0,-1 }), -len.dot({ 0,-1 })) / PI * -180 - 90;
	while (rot_.y - angle_ > 180 || rot_.y - angle_ < -180) {
		if (rot_.y - angle_ > 180) {
			rot_.y -= 360;
		}
		if (rot_.y - angle_ < -180) {
			rot_.y += 360;
		}
	}
	if (rot_.y - angle_ < 0) {
		rot_.y += 1;
		if (rot_.y - angle_ > 0) {
			rot_.y = angle_;
		}
	}
	else {
		rot_.y -= 1;
		if (rot_.y - angle_ < 0) {
			rot_.y = angle_;
		}
	}

	//移動を適応
	WorldTransUpdate();
}

void Enemy::Draw()
{
	obj_->Draw();
	eyeObj_->Draw();
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
