#include "Player.h"
#include "Camera.h"
#include "Input.h"
#include "EnemyManager.h"
#include "SpellManager.h"
#include "UIManager.h"
#include "GameManager.h"
#include "Tutorial.h"
#include "LoadOut.h"
#include "ClearChecker.h"
#include "Setting.h"
#include "ColliderManager.h"

using namespace std;

Player* Player::GetInstance()
{
	static Player instance;

	return &instance;
}

void Player::Initialize()
{
	weapon_ = make_unique<PlayerWeapon>();
	weapon_->Initialize();

	damageTex_ = Sprite::LoadTexture("damage", "png");
	damageSp_.Initialize(damageTex_);

	BaseCol::Initialize(pos_, scale_, PLAYER);

	GameInitialize();
}

void Player::GameInitialize()
{
	pos_ = { 0,0,0 };
	rot_ = { 0,0,0 };
	scale_ = { 1,1,1 };
	cameraAngle_ = { 0,-10 };
	life_ = MAX_LIFE;
	isAttack_ = false;
	presetSpell_ = 1;
	spellAngle_ = 0;

	col_.pos = pos_;
	col_.size = { 2.5f,2.2f,2.5f };
	oldCol_.pos = col_.pos;
	oldCol_.size = col_.size;

	damageSp_.SetColor(1, 1, 1, 0);
	damageAlpha_ = 0;
	naveTimer_ = 10000;

	isInvincible_ = false;

	slow_ = 1;
}

void Player::Update()
{
	//被弾演出
	if (damageAlpha_ > 0) {
		damageAlpha_ -= SPEED_DAMAGE_UI;
		damageSp_.SetColor(1, 1, 1, damageAlpha_);
	}

	//ゲーム終了orゲームオーバーで動けなくする
	if (ClearChecker::GetInstance()->GetIsClear() || life_ <= 0) {
		weapon_->Update(false, false);
		isInvincible_ = true;
		return;
	}
	//ロードアウト変更中も動けなくする
	if (LoadOut::GetInstance()->GetIsActive()) {
		CameraMove();
		return;
	}

	//1フレーム前の情報を保存
	oldCol_.pos = pos_;

	Move();
	WorldTransUpdate();

	CameraMove();
	//攻撃
	Attack();
}

void Player::Draw()
{
	weapon_->Draw();
}

void Player::SpriteDraw()
{
	damageSp_.Draw();
}

void Player::HitChangePos()
{
	pos_ = col_.pos;
	//カメラ操作
	Camera* camera = Camera::GetInstance();
	camera->SetEye(pos_ + CAMERA_EYE);//目線にカメラを調整
	camera->SetTarget(pos_ + frontVec_ + CAMERA_EYE);//目線にカメラを調整
}

void Player::OnCollision(BaseCol* a)
{
	ColliderManager* collider = ColliderManager::GetInstance();

	if (a->GetColType() == WALL) {
		if (collider->CheckHitBox(GetCol(), a->GetCol())) {
			//X押し戻し
			if (collider->CheckHitX(GetCol(), a->GetCol())) {
				if (!collider->CheckHitX(GetOldCol(), a->GetOldCol())) {
					SetColX(GetOldCol().pos.x);
					HitChangePos();
				}
			}
			//Y押し戻し
			if (collider->CheckHitY(GetCol(), a->GetCol())) {
				if (!collider->CheckHitY(GetOldCol(), a->GetOldCol())) {
					SetColY(GetOldCol().pos.y);
					HitChangePos();
				}
			}
			//Z押し戻し
			if (collider->CheckHitZ(GetCol(), a->GetCol())) {
				if (!collider->CheckHitZ(GetOldCol(), a->GetOldCol())) {
					SetColZ(GetOldCol().pos.z);
					HitChangePos();
				}
			}
		}
	}
	if (a->GetColType() == BULLET) {
		if (collider->CheckHitBox(GetCol(), a->GetCol())) {
			if (!a->GetIsHit()) {
				SubLife(200);
				a->SetIsHit();
			}
		}
	}
}

void Player::ChargeSpell(int32_t num)
{
	SpellManager::GetInstance()->ChargeSpell(num);
}

void Player::SubLife(int32_t num)
{
	if (life_ > 0) {
		if (!Tutorial::GetInstance()->GetIsTutorial() || isInvincible_) {
			life_ -= num;
		}
		damageAlpha_ = 1.0f;
		if (Camera::GetInstance()->GetShake() <= 0) {
			Camera::GetInstance()->SetShake(SHAKE_SIZE);
		}
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


	if (!GetIsCanAction()) {
		slow_ *= SPEED_SLOW;
	}

	//移動
	if (input->PushKey(DIK_W)) {
		pos_ += moveZ * SPEED_MOVE * slow_;
	}
	if (input->PushKey(DIK_S)) {
		pos_ -= moveZ * SPEED_MOVE * slow_;
	}
	if (input->PushKey(DIK_A)) {
		pos_ -= moveX * SPEED_MOVE * slow_;
	}
	if (input->PushKey(DIK_D)) {
		pos_ += moveX * SPEED_MOVE * slow_;
	}

	float stickX = float(input->GetLStickX()) / PATCH_STICK;
	float stickY = float(input->GetLStickY()) / PATCH_STICK;

	//移動
	if (input->GetLStickY()) {
		pos_ += moveZ * SPEED_MOVE * stickY * slow_;
	}
	if (input->GetLStickX()) {
		pos_ += moveX * SPEED_MOVE * stickX * slow_;
	}

	//移動速度減速を初期化
	slow_ = 1;

	//当たり判定移動
	col_.pos = pos_;

	//navePointの重みづけ
	if (++naveTimer_ > TIME_NAVE) {
		naveTimer_ = 0;
	}

	//ポーションヒール
	HealLife();
}

void Player::CameraMove()
{
	//インスタンス取得
	Input* input = Input::GetInstance();
	Camera* camera = Camera::GetInstance();

	//カメラ操作
	if (input->PushKey(DIK_LEFT)) {
		cameraAngle_.x -= SPEED_CAMERA * Setting::GetInstance()->GetXSensi();
	}
	if (input->PushKey(DIK_RIGHT)) {
		cameraAngle_.x += SPEED_CAMERA * Setting::GetInstance()->GetXSensi();
	}
	if (input->PushKey(DIK_UP)) {
		//最大値設定
		if (cameraAngle_.y <= RAD / 2) {
			cameraAngle_.y += SPEED_CAMERA * Setting::GetInstance()->GetYSensi();
		}
	}
	if (input->PushKey(DIK_DOWN)) {
		//最小値設定
		if (cameraAngle_.y >= -RAD / 2) {
			cameraAngle_.y -= SPEED_CAMERA * Setting::GetInstance()->GetYSensi();
		}
	}

	float stickX = float(input->GetRStickX()) / 32768.0f;
	float stickY = float(input->GetRStickY()) / 32768.0f;

	Vector3 vec = camera->GetTarget() - camera->GetEye();
	float assist = 1;
	if (ColliderManager::GetInstance()->CheckHitEnemyToRay(Camera::GetInstance()->GetEye(), vec.normalize()) && (input->PushButton(XINPUT_GAMEPAD_LEFT_SHOULDER) || input->GetLTrigger())) {
		assist = 0.5f;
	}

	if (input->GetRStickX()) {
		cameraAngle_.x += SPEED_CAMERA * stickX * assist;
	}

	if (input->GetRStickY()) {
		cameraAngle_.y += SPEED_CAMERA * stickY * assist;

		//最大値設定
		if (cameraAngle_.y > RAD / 2) {
			cameraAngle_.y = RAD / 2;
		}
		//最小値設定
		if (cameraAngle_.y < -RAD / 2) {
			cameraAngle_.y = -RAD / 2;
		}
	}
	//押したボタンの呪文に変える
	if (input->TriggerButton(XINPUT_GAMEPAD_Y)) {
		presetSpell_ = 0;
		SpellManager::GetInstance()->ResetChargeTime();
	}
	else if (input->TriggerButton(XINPUT_GAMEPAD_B)) {
		presetSpell_ = 1;
		SpellManager::GetInstance()->ResetChargeTime();
	}
	else if (input->TriggerButton(XINPUT_GAMEPAD_A)) {
		presetSpell_ = 2;
		SpellManager::GetInstance()->ResetChargeTime();
	}
	else if (input->TriggerButton(XINPUT_GAMEPAD_X)) {
		presetSpell_ = 3;
		SpellManager::GetInstance()->ResetChargeTime();
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
	SpellManager* spellM = SpellManager::GetInstance();

	bool isAttackOn = false;

	if ((input->TriggerKey(DIK_SPACE) || input->TriggerButton(XINPUT_GAMEPAD_RIGHT_SHOULDER) || input->GetRTrigger()) && GetIsCanAction()) {
		//攻撃フラグを立てる
		isAttack_ = true;
		attackTime_ = TIME_ATTACK_NORMAL;
	}
	//呪文詠唱
	if ((input->PushKey(DIK_E) || input->ReleaseKey(DIK_E) || input->PushButton(XINPUT_GAMEPAD_LEFT_SHOULDER) || input->ReleaseButton(XINPUT_GAMEPAD_LEFT_SHOULDER) || input->GetLTrigger() || input->ReleaseLTrigger()) && !spellM->GetIsUseSpell()) {
		ChargeSpell(LoadOut::GetInstance()->GetSpell(presetSpell_));
		isSpell_ = true;
	}
	else if (SpellManager::GetInstance()->ChargePercent() > 0.8f && SpellManager::GetInstance()->ChargePercent() < 1) {
		ChargeSpell(LoadOut::GetInstance()->GetSpell(presetSpell_));
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

	weapon_->Update(isAttack_, isAttackOn);
}

void Player::HealLife()
{
	if (healingLife_ > 0) {
		healingLife_--;
		if (life_ < MAX_LIFE) {
			life_++;
		}
	}
}

void Player::WorldTransUpdate()
{
	worldTrans_.SetPos(pos_ + CAMERA_EYE);
	worldTrans_.SetRot(rot_);
	worldTrans_.SetScale(scale_);
}
