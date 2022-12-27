#include "Player.h"

void Player::Initialize()
{
	//オブジェクト
	modelPlayer_ = Model::LoadFromObj("box");
	objectPlayer_ = Object3d::Create();
	objectPlayer_->SetModel(modelPlayer_);

	//プレイヤーの情報
	player_.trans = { 0,0,0 };
	player_.scale = { 10,10,10 };

	speed_ = 0.5f;
	jumpPow_ = 0.0f;
	isJump_ = false;
}

void Player::Update(Input* input)
{
	way_ = { 0,0,0 };
	if (input->PushKey(DIK_A)) {
		player_.trans.x -= speed_;
		way_.x = -1;
	}
	if (input->PushKey(DIK_D)) {
		player_.trans.x += speed_;
		way_.x = 1;
	}
	if (input->PushKey(DIK_S)) {
		way_.y = -1;
	}
	if (input->PushKey(DIK_W)) {
		way_.y = 2;
	}


	if (input->PushKey(DIK_SPACE) && isJump_ == false) {
		if (jumpPow_ < 1.4f) {
			jumpPow_ += 0.2f;
		}
		else {
			isJump_ = true;
		}
	}
	if (input->ReleaseKey(DIK_SPACE) && isJump_ == false) {
		isJump_ = true;
		if (jumpPow_ < 0.4f) {
			jumpPow_ += 0.5f;
		}
	}

	if (input->TriggerKey(DIK_LSHIFT) && isDash_ == false) {
		dash_ = 15;
		jumpPow_ = 0;
		isDash_ = true;
	}

	jumpPow_ -= 0.05f;
	if (jumpPow_ < 0) {
		jumpPow_ = 0;
	}

	if (dash_ != 0) {
		dash_--;
		if (way_.x == 0 || way_.y == 0) {
			player_.trans += way_ * 0.5f;
		}
		else {
			player_.trans += way_ * 0.5f * 0.7f;
		}
		if (dash_ == 0) {
			jumpPow_ = 1.0f;
		}
	}
	else
	{
		player_.trans.y += GRAVITY + jumpPow_;
	}

	if (player_.trans.y < -18) {
		player_.trans.y = -18;
		if (input->PushKey(DIK_SPACE) != true) {
			isJump_ = false;
			isDash_ = false;
		}
	}

	if (isDash_ == false) {
		objectPlayer_->SetColor(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
	}
	else {
		objectPlayer_->SetColor(Vector4(1.0f, 0.0f, 0.0f, 1.0f));
	}

	objectPlayer_->SetWorldTransform(player_);
	objectPlayer_->Update();
}

void Player::Draw()
{
	objectPlayer_->Draw();
}
