#include "Player.h"

void Player::Initialize()
{
	//オブジェクト
	modelPlayer_ = Model::LoadFromObj("box");
	objectPlayer_ = Object3d::Create();
	objectPlayer_->SetModel(modelPlayer_);

	//プレイヤーの情報
	player_.trans = { -34,-18,0 };
	player_.scale = { 10,10,10 };

	speed_ = 0.5f;
	jumpPow_ = 0.0f;
	isJump_ = false;

	//残像
	for (int i = 0; i < CONST_SHADOW; i++) {
		shadow_[i].trans = { float(i * 2),0,0 };
		shadow_[i].scale = { 9,9,9 };
		shadowAlpha_[i] = 0.0f;

		modelShadow_[i] = Model::LoadFromObj("box");
		objectShadow_[i] = Object3d::Create();
		objectShadow_[i]->SetModel(modelShadow_[i]);
		objectShadow_[i]->SetColor(Vector4(0, 0, 1, shadowAlpha_[i]));
	}
}

void Player::Update(Input* input, Map* map)
{
	//ジャンプ(押す長さで高さ調整)
	if (input->TriggerKey(DIK_SPACE) && isJump_ == false) {
		jumpPow_ += 0.4f;
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

	//ダッシュ
	if (input->TriggerKey(DIK_LSHIFT) && isDash_ == false) {
		dash_ = 15;
		jumpPow_ = 0;
		isDash_ = true;
		way_ = { 0,0,0 };

		//方向
		if (input->PushKey(DIK_A)) {
			way_.x = -1;
		}
		if (input->PushKey(DIK_D)) {
			way_.x = 1;
		}
		if (input->PushKey(DIK_S)) {
			way_.y = -1;
		}
		if (input->PushKey(DIK_W)) {
			way_.y = 1;
		}
	}
	//ジャンプしてたらだんだん上昇力減衰
	jumpPow_ -= 0.05f;
	if (jumpPow_ < 0) {
		jumpPow_ = 0;
	}

	//ダッシュ中の処理
	if (dash_ != 0) {
		dash_--;
		if (way_.x == 0 || way_.y == 0) {
			player_.trans += way_;
		}
		else {
			player_.trans += way_ * 0.7f;
		}
		if (dash_ == 0) {
			if (way_.y > 0) {
				jumpPow_ = 1.0f;
			}
			else {
				jumpPow_ = 0.5f;
			}
		}
		if (dash_ % 3 == 0) {
			SetShadow(player_.trans);
		}
		player_.trans.y -= 0.01f;
	}
	//ダッシュして無かったら
	else
	{
		if (input->PushKey(DIK_A)) {
			player_.trans.x -= speed_;
			LT_ = Vector2(player_.trans.x - 1.0f, player_.trans.y + 2.0f);
			LB_ = Vector2(player_.trans.x - 1.0f, player_.trans.y - 2.0f);
			if (map->GetMap((LT_.x + 37) / 2, 10 - LT_.y / 2) == 1) {
				player_.trans.x += speed_;
				isJump_ = false;
				if (input->PushKey(DIK_LCONTROL)) {
					isGrab_ = true;
					if (input->PushKey(DIK_W)) {
						player_.trans.y += 0.2f;
					}
					else if(input->PushKey(DIK_S)){
						player_.trans.y -= 0.2f;
					}
				}
			}
			else if (map->GetMap((LB_.x + 37) / 2, (10 - LB_.y / 2) - 1) == 1) {
				player_.trans.x += speed_;
				isJump_ = false;
			}
		}
		if (input->PushKey(DIK_D)) {
			player_.trans.x += speed_;
			RT_ = Vector2(player_.trans.x + 1.0f, player_.trans.y + 2.0f);
			RB_ = Vector2(player_.trans.x + 1.0f, player_.trans.y - 2.0f);
			if (map->GetMap((RT_.x + 37) / 2, 10 - RT_.y / 2) == 1) {
				player_.trans.x -= speed_;
				isJump_ = false;
				if (input->PushKey(DIK_LCONTROL)) {
					isGrab_ = true;
					if (input->PushKey(DIK_W)) {
						player_.trans.y += 0.2f;
					}
					else if (input->PushKey(DIK_S)) {
						player_.trans.y -= 0.2f;
					}
				}
			}
			else if (map->GetMap((RB_.x + 37) / 2, (10 - RB_.y / 2) - 1) == 1) {
				player_.trans.x -= speed_;
				isJump_ = false;
			}
		}

		if (isGrab_ == false) {
			player_.trans.y += GRAVITY + jumpPow_;
		}
		isGrab_ = false;
	}

	//床(仮)
	if (player_.trans.y < -20) {
		player_.trans.y = -20;
		if (input->PushKey(DIK_SPACE) != true) {
			isJump_ = false;
			isDash_ = false;
		}
	}

	LB_ = Vector2(player_.trans.x - 1.0f, player_.trans.y - 2.0f);
	RB_ = Vector2(player_.trans.x + 1.0f, player_.trans.y - 2.0f);

	if (map->GetMap((LB_.x + 37) / 2, 10 - LB_.y / 2) == 1) {
		player_.trans.y = LB_.y + 2.5f;
		if (input->PushKey(DIK_SPACE) != true) {
			isJump_ = false;
			isDash_ = false;
		}
	}
	else if (map->GetMap((RB_.x + 37) / 2, 10 - RB_.y / 2) == 1) {
		player_.trans.y = LB_.y + 2.5f;
		if (input->PushKey(DIK_SPACE) != true) {
			isJump_ = false;
			isDash_ = false;
		}
	}

	//プレイヤーの色
	if (isDash_ == false && isJump_ == false) {
		objectPlayer_->SetColor(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
	}
	else if (isJump_ && isDash_) {
		objectPlayer_->SetColor(Vector4(1.0f, 0.0f, 0.0f, 1.0f));
	}
	else {
		objectPlayer_->SetColor(Vector4(1.0f, 1.0f, 0.0f, 1.0f));
	}

	////壁(仮)
	//if (player_.trans.x < -34) {
	//	player_.trans.x = -34;
	//}
	//if (player_.trans.x > 34) {
	//	player_.trans.x = 34;
	//}

	//プレイヤーの場所
	objectPlayer_->SetWorldTransform(player_);
	objectPlayer_->Update();
	//残像
	for (int i = 0; i < CONST_SHADOW; i++) {
		shadowAlpha_[i] -= 0.05f;
		if (shadowAlpha_[i] < 0.0f) {
			shadowAlpha_[i] = 0.0f;
		}
		objectShadow_[i]->SetColor(Vector4(0.0f, 0.0f, 1.0f, shadowAlpha_[i]));
		objectShadow_[i]->SetWorldTransform(shadow_[i]);
		objectShadow_[i]->Update();
	}
}

void Player::Draw()
{
	objectPlayer_->Draw();
	for (int i = 0; i < CONST_SHADOW; i++) {
		objectShadow_[i]->Draw();
	}
}

void Player::Delete()
{
	delete modelPlayer_;
	delete objectPlayer_;
	for (int i = 0; i < CONST_SHADOW; i++) {
		delete modelShadow_[i];
		delete objectShadow_[i];
	}

}

void Player::SetShadow(Vector3 pos)
{
	for (int i = 0; i < CONST_SHADOW; i++) {
		if (shadowAlpha_[i] == 0.0f) {
			shadow_[i].trans = pos;
			shadowAlpha_[i] = 1.0f;
			break;
		}
	}
}
