#include "Player.h"

void Player::Initialize(int num)
{
	//オブジェクト
	modelPlayer_ = Model::LoadFromObj("player");
	objectPlayer_ = Object3d::Create();
	objectPlayer_->SetModel(modelPlayer_);

	modelPlayer2_ = Model::LoadFromObj("player2");
	objectPlayer2_ = Object3d::Create();
	objectPlayer2_->SetModel(modelPlayer2_);

	//プレイヤーの情報
	player_.trans = { -34,-18,0 };
	player_.rotation = { 0,180,0 };
	player_.scale = { 0.9f,0.9f,0.9f };

	speed_ = 0.3f;
	jumpPow_ = 0.0f;

	isJump_ = false;

	//残像
	for (int i = 0; i < CONST_SHADOW; i++) {
		shadow_[i].trans = { float(i * 2),0,0 };
		shadow_[i].scale = { 1,1,1 };
		shadowAlpha_[i] = 0.0f;

		modelShadow_[i] = Model::LoadFromObj("player");
		objectShadow_[i] = Object3d::Create();
		objectShadow_[i]->SetModel(modelShadow_[i]);
		objectShadow_[i]->SetColor(Vector4(0, 0, 1, shadowAlpha_[i]));
	}

	mapNum = num;

	effectM_ = new EffectManager();
	effectM_->Initialize();
}

int Player::Update(Input* input, Map* map)
{
	if (isDead_ == 0 && isGoal_ == 0) {
		trans_[LT].trans = Vector3(player_.trans.x - 0.0f, player_.trans.y + 0.0f, 0);	//LT
		trans_[LB].trans = Vector3(player_.trans.x - 0.0f, player_.trans.y - 1.5f, 0);	//LB
		trans_[RT].trans = Vector3(player_.trans.x + 2.0f, player_.trans.y + 0.0f, 0);	//RT
		trans_[RB].trans = Vector3(player_.trans.x + 2.0f, player_.trans.y - 1.5f, 0);	//RB

		//ジャンプ(押す長さで高さ調整)
		if ((input->TriggerKey(DIK_Z) || input->TriggerButton(XINPUT_GAMEPAD_A)) && isJump_ == false) {
			jumpPow_ += 0.6f;
			isJumpNow = true;
		}
		if ((input->PushKey(DIK_Z) || input->PushButton(XINPUT_GAMEPAD_A)) && isJumpNow == true) {
			if (jumpPow_ < 1.4f) {
				jumpPow_ += 0.2f;
			}
			else {
				isJumpNow = false;
			}
		}
		if ((input->ReleaseKey(DIK_Z) || input->ReleaseButton(XINPUT_GAMEPAD_A)) && isJumpNow == true) {
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
			if (input->PushKey(DIK_Z) != true) {
				isJump_ = false;
				isDash_ = false;
				isWallJumpL_ = 0;
				isWallJumpR_ = 0;
			}
			for (int i = 0; i < 10; i++) {
				map->SetItemUse(i, false);
			}
			for (int i = 0; i < 4; i++) {
				if (isGrab_ == false) {
					trans_[i].trans.y -= GRAVITY + jumpPow_;
				}
			}
		}
		else if (map->GetMap((trans_[RB].trans.x + 36) / 2, (trans_[RB].trans.y - 20) * -1 / 2) == 1) {
			if (input->PushKey(DIK_Z) != true) {
				isJump_ = false;
				isDash_ = false;
				isWallJumpL_ = 0;
				isWallJumpR_ = 0;
			}
			for (int i = 0; i < 10; i++) {
				map->SetItemUse(i, false);
			}
			for (int i = 0; i < 4; i++) {
				if (isGrab_ == false) {
					trans_[i].trans.y -= GRAVITY + jumpPow_;
				}
			}

		}
		//天井の判定
		else if (map->GetMap((trans_[LT].trans.x + 36) / 2, (trans_[LT].trans.y - 20) * -1 / 2) == 1) {
			jumpPow_ = 0.5f;
			isJump_ = true;
			if (isGrab_ == false) {
				for (int i = 0; i < 4; i++) {
					trans_[i].trans.y -= GRAVITY + jumpPow_;
				}
			}
		}
		else if (map->GetMap((trans_[RT].trans.x + 36) / 2, (trans_[RT].trans.y - 20) * -1 / 2) == 1) {
			jumpPow_ = 0.5f;
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
		if ((input->TriggerKey(DIK_X) || input->TriggerButton(XINPUT_GAMEPAD_X)) && isDash_ == false) {
			dash_ = 15;
			jumpPow_ = 0;
			isDash_ = true;
			way_ = { 0,0,0 };
			int count = 0;
			//方向
			if (input->PushKey(DIK_LEFT)) {
				way_.x = -1;
				count++;
			}
			else if (input->PushKey(DIK_RIGHT)) {
				way_.x = 1;
				count++;
			}
			if (input->PushKey(DIK_DOWN)) {
				way_.y = -1;
				count++;
			}
			else if (input->PushKey(DIK_UP)) {
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
		trans_[LB].trans = Vector3(player_.trans.x - 0.0f, player_.trans.y - 1.3f, 0);	//LB
		trans_[RT].trans = Vector3(player_.trans.x + 2.0f, player_.trans.y + 0.0f, 0);	//RT
		trans_[RB].trans = Vector3(player_.trans.x + 2.0f, player_.trans.y - 1.3f, 0);	//RB
		isGrab_ = false;
		//左右移動
		if (dash_ == false && isWallJumpL_ <= 0 && isWallJumpR_ <= 0) {
			if (input->PushKey(DIK_LEFT) || input->GetLSteckX() < 0) {
				for (int i = 0; i < 4; i++) {
					trans_[i].trans.x -= speed_;
				}
				if (map->GetMap((trans_[LT].trans.x + 36) / 2, (trans_[LT].trans.y - 20) * -1 / 2) == 1) {
					if (input->TriggerKey(DIK_Z) || input->TriggerButton(XINPUT_GAMEPAD_A)) {
						if (isJumpNow == false) {
							isWallJumpL_ = 10;
							jumpPow_ = 1.6f;
						}
					}
				}
				else if (map->GetMap((trans_[LB].trans.x + 36) / 2, (trans_[LB].trans.y - 20) * -1 / 2) == 1) {
				}
				else {
					player_.trans.x -= speed_;
				}
				player_.rotation = { 0,0,0 };
			}
			else if (input->PushKey(DIK_RIGHT) || input->GetLSteckX() > 0) {
				for (int i = 0; i < 4; i++) {
					trans_[i].trans.x += speed_;
				}
				if (map->GetMap((trans_[RT].trans.x + 36) / 2, (trans_[RT].trans.y - 20) * -1 / 2) == 1) {
					if (input->TriggerKey(DIK_Z) || input->TriggerButton(XINPUT_GAMEPAD_A)) {
						if (isJumpNow == false) {
							isWallJumpR_ = 10;
							jumpPow_ = 1.6f;
						}
					}
				}
				else if (map->GetMap((trans_[RB].trans.x + 36) / 2, (trans_[RB].trans.y - 20) * -1 / 2) == 1) {
				}
				else {
					player_.trans.x += speed_;
				}
				player_.rotation = { 0,180,0 };
			}
		}

		//ダッシュ中の処理
		if (dash_ != 0) {
			dash_--;
			trans_[LT].trans = Vector3(player_.trans.x - 0.0f, player_.trans.y + 0.0f, 0);	//LT
			trans_[LB].trans = Vector3(player_.trans.x - 0.0f, player_.trans.y - 1.3f, 0);	//LB
			trans_[RT].trans = Vector3(player_.trans.x + 2.0f, player_.trans.y + 0.0f, 0);	//RT
			trans_[RB].trans = Vector3(player_.trans.x + 2.0f, player_.trans.y - 1.3f, 0);	//RB
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
					jumpPow_ = 0.7f;
				}
			}
			if (dash_ % 2 == 0) {
				SetShadow(player_.trans);
			}
			//player_.trans.y -= 0.01f;
		}

		//壁キック
		if (isWallJumpL_ > 0) {
			Vector3 vec = { 0.8f,0,0 };
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
			Vector3 vec = { -0.8f,0,0 };
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

		//プレイヤーの場所をモデルにセット
		objectPlayer_->SetWorldTransform(player_);
		objectPlayer_->Update();
		objectPlayer2_->SetWorldTransform(player_);
		objectPlayer2_->Update();

		//アイテムとの判定
		for (int i = 0; i < 10; i++) {
			if (map->GetItemUse(i) == false) {
				Vector3 len = map->GetItemPos(i) - player_.trans;
				if (len.length() > -2 && len.length() < 2) {
					map->SetItemUse(i, true);
					isDash_ = false;
					effectM_->BurstGenerate(player_.trans, 10, { 0,0.8f,0.8f,1 });
				}
			}
		}
		//トラップとの判定
		for (int i = 0; i < 50; i++) {
			Vector3 len = map->GetTrapPos(i) - player_.trans;
			if (len.length() > -1.8f && len.length() < 1.8f) {
				isDead_ = 80;
				dash_ = 0;
				effectM_->BurstGenerate(player_.trans, 10, { 0.8f,0,0,1 });
			}
		}
		//ゴールの判定
		Vector3 len = map->GetGoalPos() - player_.trans;
		if (len.length() > -2 && len.length() < 2) {
			isGoal_ = 120;
			dash_ = 0;
		}
	}
	else if (isGoal_ != 0) {
		isGoal_--;
		
		if (isGoal_ == 100) {
			effectM_->BurstGenerate(map->GetGoalPos(), 10, {1.1f,1.1f,1.1f,1});
		}
		else {
			map->AddGoalPosY();
		}
		if (isGoal_ == 0) {
			mapNum++;
			if (mapNum % 5 == 0) {
				return mapNum / 5;
			}
			else {
				map->SetMap(mapNum);
				player_.trans = map->GetRespornPoint();
			}
		}
	}
	else {
		isDead_--;
		if (isDead_ == 0) {
			player_.trans = map->GetRespornPoint();
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

	effectM_->Update();

	return 0;
}

void Player::Draw()
{
	if (isDead_ % 4 != 1) {
		if (isDash_ == false) {
			objectPlayer_->Draw();
		}
		else {
			objectPlayer2_->Draw();
		}
	}
	for (int i = 0; i < CONST_SHADOW; i++) {
		objectShadow_[i]->Draw();
	}
	effectM_->Draw();
}

void Player::Delete()
{
	delete modelPlayer_;
	delete objectPlayer_;
	delete modelPlayer2_;
	delete objectPlayer2_;
	for (int i = 0; i < CONST_SHADOW; i++) {
		delete modelShadow_[i];
		delete objectShadow_[i];
	}
	delete effectM_;
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
