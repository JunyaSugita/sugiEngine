#include "Player.h"
#include "Camera.h"
#include "Input.h"
#include "EnemyManager.h"
#include "PlayerWeapon.h"
#include "SpellManager.h"

Player::Player()
{
}

Player::~Player()
{
}

Player* Player::GetInstance()
{
	static Player instance;

	return &instance;
}

void Player::Initialize()
{
	pos_ = { 0,0,-50 };
	rot_ = { 0,0,0 };
	scale_ = { 1,1,1 };
	cameraAngle_ = { 0,0 };
	life_ = 10;
	isAttack_ = false;
	presetSpell_ = FIRE_BALL;
	spellAngle_ = 0;

	boxCol_.pos = pos_;
	boxCol_.size = {2.0f,2.2f,2.0f};

	PlayerWeapon::GetInstance()->Initialize();
}

void Player::Update()
{
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

	float stickX = float(input->GetLSteckX()) / 32768.0f;
	float stickY = float(input->GetLSteckY()) / 32768.0f;

	//移動
	if (input->GetLSteckY()) {
		pos_ += moveZ * SPEED_MOVE * stickY * slow;
	}
	if (input->GetLSteckX()) {
		pos_ += moveX * SPEED_MOVE * stickX * slow;
	}

	//当たり判定移動
	boxCol_.pos = pos_;
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

	float stickX = float(input->GetRSteckX()) / 32768.0f;
	float stickY = float(input->GetRSteckY()) / 32768.0f;

	if (input->GetLTrigger() < 50) {
		if (input->GetRSteckX()) {
			cameraAngle_.x += SPEED_CAMERA * stickX;
		}

		if (input->GetRSteckY()) {
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
		Vector2 len = Vector2(input->GetRSteckX(), input->GetRSteckY());
		float length = len.length();
		if (len.length() > 20000) {
			len.normalize();
			if (input->GetRSteckX() != 0 && input->GetRSteckY() != 0) {
				spellAngle_ = (atan2(len.cross({ 0,-1 }), -len.dot({ 0,-1 })) / PI * -RAD - (RAD / 2)) + RAD / 2 * 3;
			}

			//スティックを倒した方向の呪文に変える
			if (spellAngle_ >= RAD && spellAngle_ < 72 + RAD) {
				presetSpell_ = FIRE_BALL;
				SpellManager::GetInstance()->ResetChargeTime();
			}
			else if (spellAngle_ >= 72 + RAD && spellAngle_ < 144 + RAD) {
				presetSpell_ = MAGIC_MISSILE;
				SpellManager::GetInstance()->ResetChargeTime();
			}
			else if (spellAngle_ >= 144 + RAD && spellAngle_ < 180 + RAD || spellAngle_ >= 0 && spellAngle_ < 36) {
				presetSpell_ = ICE_BOLT;
				SpellManager::GetInstance()->ResetChargeTime();
			}
			else if (spellAngle_ >= 36 && spellAngle_ < 108) {
				presetSpell_ = CHAIN_LIGHTNING;
				SpellManager::GetInstance()->ResetChargeTime();
			}
			else if (spellAngle_ >= 108 && spellAngle_ < RAD) {
				presetSpell_ = FIRE_BALL;
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
		switch (presetSpell_)
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