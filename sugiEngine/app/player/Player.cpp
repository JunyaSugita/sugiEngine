#include "Player.h"
#include "Camera.h"
#include "Input.h"
#include "EnemyManager.h"
#include "PlayerWeapon.h"
#include "SpellManager.h"
#include "UIManager.h"
#include "GameManager.h"
#include "Tutorial.h"
#include "LoadOut.h"

Player* Player::GetInstance()
{
	static Player instance;

	return &instance;
}

void Player::Initialize()
{
	pos_ = { 0,0,-40 };
	rot_ = { 0,0,0 };
	scale_ = { 1,1,1 };
	cameraAngle_ = { 0,0 };
	life_ = 10;
	isAttack_ = false;
	presetSpell_ = 0;
	spellAngle_ = 0;

	boxCol_.pos = pos_;
	boxCol_.size = {2.5f,2.2f,2.5f};
	oldBoxCol_.pos = pos_;
	oldBoxCol_.size = { 2.5f,2.2f,2.5f };

	PlayerWeapon::GetInstance()->Initialize();

	damageTex_ = Sprite::LoadTexture("damage.png");
	damageSp_.Initialize(damageTex_);
	damageSp_.SetColor(1,1,1,0);
	damageAlpha_ = 0;
}

void Player::GameInitialize()
{
	pos_ = { 0,0,-40 };
	rot_ = { 0,0,0 };
	scale_ = { 1,1,1 };
	cameraAngle_ = { 0,0 };
	life_ = 10;
	isAttack_ = false;
	presetSpell_ = 0;
	spellAngle_ = 0;

	boxCol_.pos = pos_;
	boxCol_.size = { 2.5f,2.2f,2.5f };
	oldBoxCol_.pos = pos_;
	oldBoxCol_.size = { 2.5f,2.2f,2.5f };

	damageSp_.SetColor(1, 1, 1, 0);
	damageAlpha_ = 0;
}

void Player::Update()
{
	//被弾演出
	if (damageAlpha_ > 0) {
		damageAlpha_ -= 0.01f;
		damageSp_.SetColor(1, 1, 1, damageAlpha_);
	}

	//ゲーム終了orゲームオーバーで動けなくする
	if (UIManager::GetInstance()->GetStateAlpha_() != 0 || life_ <= 0) {
		return;
	}
	//ロードアウト変更中も動けなくする
	if (LoadOut::GetInstance()->GetIsActive()) {
		return;
	}

	//1フレーム前の情報を保存
	oldBoxCol_.pos = pos_;

	Move();
	WorldTransUpdate();

	//攻撃
	Attack();

	CameraMove();
}

void Player::Draw()
{
	PlayerWeapon::GetInstance()->Draw();
}

void Player::SpDraw()
{
	damageSp_.Draw();
}

void Player::SubLife()
{
	if (life_ > 0 ) {
		if (!Tutorial::GetInstance()->GetIsTutorial()) {
			life_--;
		}
		damageAlpha_ = 1.0f;
		Camera::GetInstance()->SetShake(0.05f);
	}
}

bool Player::GetIsCanAction()
{
	if (isAttack_ || isSpell_ || SpellManager::GetInstance()->GetIsUseSpell()) {
		return false;
	}
	return true;
}

void Player::Move()
{
	//インスタンス取得
	Input* input = Input::GetInstance();

	Vector3 moveZ = { frontVec_.x,0,frontVec_.z };
	moveZ.normalize();
	Vector3 moveX = { rightVec_.x,0,rightVec_.z };
	moveX.normalize();

	//移動速度低下処理
	float slow = 1;
	if (!GetIsCanAction()) {
		slow = SPEED_SLOW;
	}

	//移動
	if (input->PushKey(DIK_W)) {
		pos_ += moveZ * SPEED_MOVE * slow;
	}
	if (input->PushKey(DIK_S)) {
		pos_ -= moveZ * SPEED_MOVE * slow;
	}
	if (input->PushKey(DIK_A)) {
		pos_ -= moveX * SPEED_MOVE * slow;
	}
	if (input->PushKey(DIK_D)) {
		pos_ += moveX * SPEED_MOVE * slow;
	}

	float stickX = float(input->GetLStickX()) / 32768.0f;
	float stickY = float(input->GetLStickY()) / 32768.0f;

	//移動
	if (input->GetLStickY()) {
		pos_ += moveZ * SPEED_MOVE * stickY * slow;
	}
	if (input->GetLStickX()) {
		pos_ += moveX * SPEED_MOVE * stickX * slow;
	}

	//当たり判定移動
	boxCol_.pos = pos_;

	//navePointの重みづけ
	ColliderManager::GetInstance()->NavePointScore();
}

void Player::CameraMove()
{
	//インスタンス取得
	Input* input = Input::GetInstance();
	Camera* camera = Camera::GetInstance();

	//カメラ操作
	if (input->PushKey(DIK_LEFT)) {
		cameraAngle_.x -= SPEED_CAMERA;
	}
	if (input->PushKey(DIK_RIGHT)) {
		cameraAngle_.x += SPEED_CAMERA;
	}
	if (input->PushKey(DIK_UP)) {
		//最大値設定
		if (cameraAngle_.y <= 90) {
			cameraAngle_.y += SPEED_CAMERA;
		}
	}
	if (input->PushKey(DIK_DOWN)) {
		//最小値設定
		if (cameraAngle_.y >= -90) {
			cameraAngle_.y -= SPEED_CAMERA;
		}
	}

	float stickX = float(input->GetRStickX()) / 32768.0f;
	float stickY = float(input->GetRStickY()) / 32768.0f;

	if (input->GetLTrigger() < 50 && !(input->TriggerKey(DIK_1) || input->TriggerKey(DIK_2) || input->TriggerKey(DIK_3) || input->TriggerKey(DIK_4) || input->TriggerKey(DIK_5))) {
		if (input->GetRStickX()) {
			cameraAngle_.x += SPEED_CAMERA * stickX;
		}

		if (input->GetRStickY()) {
			cameraAngle_.y += SPEED_CAMERA * stickY;

			//最大値設定
			if (cameraAngle_.y > RAD / 2) {
				cameraAngle_.y = RAD / 2;
			}
			//最小値設定
			if (cameraAngle_.y < -RAD / 2) {
				cameraAngle_.y = -RAD / 2;
			}
		}
	}
	else {
		Vector2 len = Vector2(input->GetRStickX(), input->GetRStickY());
		float length = len.length();
		if (len.length() > 20000 || input->TriggerKey(DIK_1) || input->TriggerKey(DIK_2) || input->TriggerKey(DIK_3) || input->TriggerKey(DIK_4) || input->TriggerKey(DIK_5)) {
			len.normalize();
			if (input->GetRStickX() != 0 && input->GetRStickY() != 0) {
				spellAngle_ = (atan2(len.cross({ 0,-1 }), -len.dot({ 0,-1 })) / PI * -RAD - (RAD / 2)) + RAD / 2 * 3;
			}

			//スティックを倒した方向の呪文に変える
			if (spellAngle_ >= RAD && spellAngle_ < 72 + RAD) {
				presetSpell_ = 0;
				SpellManager::GetInstance()->ResetChargeTime();
			}
			else if (spellAngle_ >= 72 + RAD && spellAngle_ < 144 + RAD) {
				presetSpell_ = 1;
				SpellManager::GetInstance()->ResetChargeTime();
			}
			else if (spellAngle_ >= 144 + RAD && spellAngle_ < 180 + RAD || spellAngle_ >= 0 && spellAngle_ < 36) {
				presetSpell_ = 2;
				SpellManager::GetInstance()->ResetChargeTime();
			}
			else if (spellAngle_ >= 36 && spellAngle_ < 108) {
				presetSpell_ = 3;
				SpellManager::GetInstance()->ResetChargeTime();
			}
			else if (spellAngle_ >= 108 && spellAngle_ < RAD) {
				presetSpell_ = 4;
				SpellManager::GetInstance()->ResetChargeTime();
			}

			//キー対応
			if (input->TriggerKey(DIK_1)) {
				presetSpell_ = 0;
				SpellManager::GetInstance()->ResetChargeTime();
			}
			else if (input->TriggerKey(DIK_2)) {
				presetSpell_ = 1;
				SpellManager::GetInstance()->ResetChargeTime();
			}
			else if (input->TriggerKey(DIK_3)) {
				presetSpell_ = 2;
				SpellManager::GetInstance()->ResetChargeTime();
			}
			else if (input->TriggerKey(DIK_4)) {
				presetSpell_ = 3;
				SpellManager::GetInstance()->ResetChargeTime();
			}
			else if (input->TriggerKey(DIK_5)) {
				presetSpell_ = 4;
				SpellManager::GetInstance()->ResetChargeTime();
			}

		}
	}

	frontVec_.x = float(sin(Radian(cameraAngle_.x)));
	frontVec_.y = float(sin(Radian(cameraAngle_.y)));
	frontVec_.z = float(cos(Radian(cameraAngle_.x)));
	rightVec_.x = float(sin(Radian(cameraAngle_.x + RAD / 2)));
	rightVec_.y = float(sin(Radian(cameraAngle_.y)));
	rightVec_.z = float(cos(Radian(cameraAngle_.x + RAD / 2)));

	//カメラ操作
	camera->SetEye(pos_ + CAMERA_EYE);//目線にカメラを調整
	camera->SetTarget(pos_ + frontVec_ + CAMERA_EYE);//目線にカメラを調整
}

void Player::Attack()
{
	//インスタンス取得
	Input* input = Input::GetInstance();
	PlayerWeapon* weapon = PlayerWeapon::GetInstance();
	SpellManager* spellM = SpellManager::GetInstance();

	bool isAttackOn = false;

	if ((input->TriggerKey(DIK_SPACE) || input->TriggerButton(XINPUT_GAMEPAD_RIGHT_SHOULDER)) && GetIsCanAction()) {
		//攻撃フラグを立てる
		isAttack_ = true;
		attackTime_ = TIME_ATTACK_NORMAL;
	}
	//呪文詠唱
	if ((input->PushKey(DIK_E) || input->ReleaseKey(DIK_E) || input->PushButton(XINPUT_GAMEPAD_LEFT_SHOULDER) || input->ReleaseButton(XINPUT_GAMEPAD_LEFT_SHOULDER)) && !spellM->GetIsUseSpell()) {
		switch (LoadOut::GetInstance()->GetSpell(presetSpell_))
		{
		case FIRE_BALL:
			spellM->ChargeFireBall();
			break;
		case MAGIC_MISSILE:
			spellM->ChargeMagicMissile();
			break;
		case ICE_BOLT:
			spellM->ChargeIceBolt();
			break;
		case CHAIN_LIGHTNING:
			spellM->ChargeChainLightning();
			break;
		case ENCHANT_FIRE:
			spellM->ChargeEnchantFire();
			break;
		default:
			break;
		}

		isSpell_ = true;
	}
	else {
		isSpell_ = false;
	}

	//攻撃判定チェック
	if (attackTime_ < TIME_ATTACK_NORMAL - TIME_ATTACK_START_NORMAL && attackTime_ > TIME_ATTACK_NORMAL - TIME_ATTACK_END_NORMAL) {
		isAttackOn = true;
	}
	//攻撃終了
	if (attackTime_ <= 0) {
		//攻撃フラグを消す
		isAttack_ = false;
		//敵の多段ヒット回避フラグを消す
		EnemyManager::GetInstance()->ResetIsHit();
	}
	else {
		//時間を減らす
		attackTime_--;
	}

	weapon->Update(isAttack_, isAttackOn);
}

void Player::WorldTransUpdate()
{
	worldTrans_.SetPos(pos_ + CAMERA_EYE);
	worldTrans_.SetRot(rot_);
	worldTrans_.SetScale(scale_);
}

float Radian(float r) {
	return r * (PI / RAD);
}