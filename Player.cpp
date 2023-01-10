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
	trans_[LT].trans = Vector3(player_.trans.x - 0.0f, player_.trans.y + 0.0f, 0);	//LT
	trans_[LB].trans = Vector3(player_.trans.x - 0.0f, player_.trans.y - 1.5f, 0);	//LB
	trans_[RT].trans = Vector3(player_.trans.x + 2.0f, player_.trans.y + 0.0f, 0);	//RT
	trans_[RB].trans = Vector3(player_.trans.x + 2.0f, player_.trans.y - 1.5f, 0);	//RB

	//ジャンプ(押す長さで高さ調整)
	if ((input->TriggerKey(DIK_SPACE) || input->TriggerButton(XINPUT_GAMEPAD_A)) && isJump_ == false) {
		jumpPow_ += 0.6f;
		isJumpNow = true;
	}
	if ((input->PushKey(DIK_SPACE) || input->PushButton(XINPUT_GAMEPAD_A)) && isJumpNow == true) {
		if (jumpPow_ < 1.4f) {
			jumpPow_ += 0.2f;
		}
		else {
			isJumpNow = false;
		}
	}
	if ((input->ReleaseKey(DIK_SPACE) || input->ReleaseButton(XINPUT_GAMEPAD_A)) && isJumpNow == true) {
		if (jumpPow_ < 0.4f) {
			jumpPow_ += 0.5f;
		}
		isJumpNow = false;
	}

	isJump_ = true;

	//ジャンプしてたらだんだん上昇力減衰
	jumpPow_ -= 0.05f;
	if (jumpPow_ < 0) {
		jumpPow_ = 0;
	}

	//重力とジャンプ力でプレイヤーを仮移動
	if (isGrab_ == false) {
		for (int i = 0; i < 4; i++) {
			trans_[i].trans.y += GRAVITY + jumpPow_;
		}
	}

	//床の判定
	if (map->GetMap((trans_[LB].trans.x + 36) / 2, (trans_[LB].trans.y - 20) * -1 / 2) == 1) {
		if (input->PushKey(DIK_SPACE) != true) {
			isJump_ = false;
			isDash_ = false;
			isWallJumpL_ = -1;
			isWallJumpR_ = -1;
		}
		for (int i = 0; i < 5; i++) {
			map->SetItemUse(i, false);
		}
		for (int i = 0; i < 4; i++) {
			if (isGrab_ == false) {
				trans_[i].trans.y -= GRAVITY + jumpPow_;
			}
		}
	}
	else if (map->GetMap((trans_[RB].trans.x + 36) / 2, (trans_[RB].trans.y - 20) * -1 / 2) == 1) {
		if (input->PushKey(DIK_SPACE) != true) {
			isJump_ = false;
			isDash_ = false;
		}
		for (int i = 0; i < 5; i++) {
			map->SetItemUse(i, false);
		}
		for (int i = 0; i < 4; i++) {
			if (isGrab_ == false) {
				trans_[i].trans.y -= GRAVITY + jumpPow_;
			}
		}
	}
	//天井の判定
	else if(map->GetMap((trans_[LT].trans.x + 36) / 2, (trans_[LT].trans.y - 20) * -1 / 2) == 1){
		jumpPow_ = 0;
		isJump_ = true;
		if (isGrab_ == false) {
			for (int i = 0; i < 4; i++) {
				trans_[i].trans.y -= GRAVITY + jumpPow_;
			}
		}
	}
	else if (map->GetMap((trans_[RT].trans.x + 36) / 2, (trans_[RT].trans.y - 20) * -1 / 2) == 1) {
		jumpPow_ = 0;
		isJump_ = true;
		if (isGrab_ == false) {
			for (int i = 0; i < 4; i++) {
				trans_[i].trans.y -= GRAVITY + jumpPow_;
			}
		}
	}
	else {
		if (dash_ == false && isGrab_ == false) {
			player_.trans.y += GRAVITY + jumpPow_;
		}
	}

	//ダッシュ
	if ((input->TriggerKey(DIK_LSHIFT) || input->TriggerButton(XINPUT_GAMEPAD_X)) && isDash_ == false) {
		dash_ = 15;
		jumpPow_ = 0;
		isDash_ = true;
		way_ = { 0,0,0 };
		int count = 0;
		//方向
		if (input->PushKey(DIK_A)) {
			way_.x = -1;
			count++;
		}
		else if (input->PushKey(DIK_D)) {
			way_.x = 1;
			count++;
		}
		if (input->PushKey(DIK_S)) {
			way_.y = -1;
			count++;
		}
		else if (input->PushKey(DIK_W)) {
			count++;
			way_.y = 1;
		}
		if (count == 2) {
			way_ *= 0.7;
		}


		if (input->GetLSteckX() != 0 || input->GetLSteckY() != 0) {
			Vector3 angle = { 0,0,0 };
			angle.x = input->GetLSteckX();
			angle.y = input->GetLSteckY();
			way_ = angle.normalize();
		}
	}
	trans_[LT].trans = Vector3(player_.trans.x - 0.0f, player_.trans.y + 0.0f, 0);	//LT
	trans_[LB].trans = Vector3(player_.trans.x - 0.0f, player_.trans.y - 1.5f, 0);	//LB
	trans_[RT].trans = Vector3(player_.trans.x + 2.0f, player_.trans.y + 0.0f, 0);	//RT
	trans_[RB].trans = Vector3(player_.trans.x + 2.0f, player_.trans.y - 1.5f, 0);	//RB
	isGrab_ = false;
	//左右移動
	if (dash_ == false && isWallJumpL_ <= 0 && isWallJumpR_ <= 0) {
		if (input->PushKey(DIK_A) || input->GetLSteckX() < 0) {
			for (int i = 0; i < 4; i++) {
				trans_[i].trans.x -= speed_;
			}
			if (map->GetMap((trans_[LT].trans.x + 36) / 2, (trans_[LT].trans.y - 20) * -1 / 2) == 1) {
				if (input->TriggerKey(DIK_SPACE) || input->TriggerButton(XINPUT_GAMEPAD_A)) {
					if (isWallJumpL_ == -1 && isJump_ == true) {
						isWallJumpL_ = 10;
						jumpPow_ = 1.5f;
						isWallJumpR_ = -1;
					}
				}
			}
			else if (map->GetMap((trans_[LB].trans.x + 36) / 2, (trans_[LB].trans.y - 20) * -1 / 2) == 1) {
			}
			else {
				player_.trans.x -= speed_;
			}
		}
		else if (input->PushKey(DIK_D) || input->GetLSteckX() > 0) {
			for (int i = 0; i < 4; i++) {
				trans_[i].trans.x += speed_;
			}
			if (map->GetMap((trans_[RT].trans.x + 36) / 2, (trans_[RT].trans.y - 20) * -1 / 2) == 1) {
				if (input->TriggerKey(DIK_SPACE) || input->TriggerButton(XINPUT_GAMEPAD_A)) {
					if (isWallJumpR_ == -1 && isJump_ == true) {
						isWallJumpR_ = 10;
						jumpPow_ = 1.5f;
						isWallJumpL_ = -1;
					}
				}
			}
			else if (map->GetMap((trans_[RB].trans.x + 36) / 2, (trans_[RB].trans.y - 20) * -1 / 2) == 1) {
			}
			else {
				player_.trans.x += speed_;
			}
		}
	}

	//ダッシュ中の処理
	if (dash_ != 0) {
		dash_--;
		trans_[LT].trans = Vector3(player_.trans.x - 0.0f, player_.trans.y + 0.0f, 0);	//LT
		trans_[LB].trans = Vector3(player_.trans.x - 0.0f, player_.trans.y - 1.5f, 0);	//LB
		trans_[RT].trans = Vector3(player_.trans.x + 2.0f, player_.trans.y + 0.0f, 0);	//RT
		trans_[RB].trans = Vector3(player_.trans.x + 2.0f, player_.trans.y - 1.5f, 0);	//RB
		for (int i = 0; i < 4; i++) {
			trans_[i].trans += way_;
		}

		if (map->GetMap((trans_[LT].trans.x + 36) / 2, (trans_[LT].trans.y - 20) * -1 / 2) == 1) {
			dash_ = 0;
		}
		else if (map->GetMap((trans_[LB].trans.x + 36) / 2, (trans_[LB].trans.y - 20) * -1 / 2) == 1) {
			dash_ = 0;
		}
		else if (map->GetMap((trans_[RT].trans.x + 36) / 2, (trans_[RT].trans.y - 20) * -1 / 2) == 1) {
			dash_ = 0;
		}
		else if (map->GetMap((trans_[RB].trans.x + 36) / 2, (trans_[RB].trans.y - 20) * -1 / 2) == 1) {
			dash_ = 0;
		}
		else {
			player_.trans += way_;
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

	//壁キック
	if (isWallJumpL_ > 0) {
		Vector3 vec = { 0.7f,0,0 };
		isWallJumpL_--;
		for (int i = 0; i < 4; i++) {
			trans_[i].trans += vec;
		}
		if (map->GetMap((trans_[LT].trans.x + 36) / 2, (trans_[LT].trans.y - 20) * -1 / 2) == 1) {
			isWallJumpL_ = 0;
		}
		else if (map->GetMap((trans_[LB].trans.x + 36) / 2, (trans_[LB].trans.y - 20) * -1 / 2) == 1) {
			isWallJumpL_ = 0;
		}
		else if (map->GetMap((trans_[RT].trans.x + 36) / 2, (trans_[RT].trans.y - 20) * -1 / 2) == 1) {
			isWallJumpL_ = 0;
		}
		else if (map->GetMap((trans_[RB].trans.x + 36) / 2, (trans_[RB].trans.y - 20) * -1 / 2) == 1) {
			isWallJumpL_ = 0;
		}
		else {
			player_.trans += vec;
		}
	}
	if (isWallJumpR_ > 0) {
		Vector3 vec = { -0.7f,0,0 };
		isWallJumpR_--;
		for (int i = 0; i < 4; i++) {
			trans_[i].trans += vec;
		}
		if (map->GetMap((trans_[LT].trans.x + 36) / 2, (trans_[LT].trans.y - 20) * -1 / 2) == 1) {
			isWallJumpR_ = 0;
		}
		else if (map->GetMap((trans_[LB].trans.x + 36) / 2, (trans_[LB].trans.y - 20) * -1 / 2) == 1) {
			isWallJumpR_ = 0;
		}
		else if (map->GetMap((trans_[RT].trans.x + 36) / 2, (trans_[RT].trans.y - 20) * -1 / 2) == 1) {
			isWallJumpR_ = 0;
		}
		else if (map->GetMap((trans_[RB].trans.x + 36) / 2, (trans_[RB].trans.y - 20) * -1 / 2) == 1) {
			isWallJumpR_ = 0;
		}
		else {
			player_.trans += vec;
		}
	}


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

	//プレイヤーの場所をモデルにセット
	objectPlayer_->SetWorldTransform(player_);
	objectPlayer_->Update();

	//アイテムとの判定
	for (int i = 0; i < 5; i++) {
		Vector3 len = map->GetItemPos(i) - player_.trans;
		if (len.length() > -2 && len.length() < 2) {
			map->SetItemUse(i, true);
			isDash_ = false;
		}
	}
	//トラップとの判定
	for (int i = 0; i < 50; i++) {
		Vector3 len = map->GetTrapPos(i) - player_.trans;
		if (len.length() > -2 && len.length() < 2) {
			isDead_ = true;
			player_.trans = map->GetRespornPoint();
			isDead_ = false;
		}
	}
	//ゴールの判定
	Vector3 len = map->GetGoalPos() - player_.trans;
	if (len.length() > -2 && len.length() < 2) {
		isGoal_ = true;
		mapNum++;
		map->SetMap(mapNum);
		player_.trans = map->GetRespornPoint();
		isGoal_ = false;
	}

}

void Player::Draw()
{
	if (isDead_ == false) {
		objectPlayer_->Draw();
	}
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
