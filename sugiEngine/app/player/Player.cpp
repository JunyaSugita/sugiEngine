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

	PlayerWeapon::GetInstance()->Initialize();
}

void Player::Update()
{
	Move();
	WorldTransUpdate();
	CameraMove();
	
	//�U��
	Attack();
}

void Player::Draw()
{
	PlayerWeapon::GetInstance()->Draw();
}

bool Player::GetIsCanAction()
{
	if (isAttack_ || isSpell_) {
		return false;
	}
	return true;
}

void Player::Move()
{
	//�C���X�^���X�擾
	Input* input = Input::GetInstance();

	Vector3 moveZ= { frontVec_.x,0,frontVec_.z };
	moveZ.normalize();
	Vector3 moveX = { rightVec_.x,0,rightVec_.z };
	moveX.normalize();
	//�ړ�
	if (input->PushKey(DIK_W)) {
		pos_ += moveZ * SPEED_MOVE;
	}
	if (input->PushKey(DIK_S)) {
		pos_ -= moveZ * SPEED_MOVE;
	}
	if (input->PushKey(DIK_A)) {
		pos_ -= moveX * SPEED_MOVE;
	}
	if (input->PushKey(DIK_D)) {
		pos_ += moveX * SPEED_MOVE;
	}

	float stickX = float(input->GetLSteckX()) / 32768.0f;
	float stickY = float(input->GetLSteckY()) / 32768.0f;

	//�ړ�
	if (input->GetLSteckY()) {
		pos_ += moveZ * SPEED_MOVE * stickY;
	}
	if (input->GetLSteckX()) {
		pos_ += moveX * SPEED_MOVE * stickX;
	}
}

void Player::CameraMove()
{
	//�C���X�^���X�擾
	Input* input = Input::GetInstance();
	Camera* camera = Camera::GetInstance();

	//�J��������
	if (input->PushKey(DIK_LEFT)) {
		cameraAngle_.x -= SPEED_CAMERA;
	}
	if (input->PushKey(DIK_RIGHT)) {
		cameraAngle_.x += SPEED_CAMERA;
	}
	if (input->PushKey(DIK_UP)) {
		//�ő�l�ݒ�
		if (cameraAngle_.y <= 90) {
			cameraAngle_.y += SPEED_CAMERA;
		}
	}
	if (input->PushKey(DIK_DOWN)) {
		//�ŏ��l�ݒ�
		if (cameraAngle_.y >= -90) {
			cameraAngle_.y -= SPEED_CAMERA;
		}
	}

	float stickX = float(input->GetRSteckX()) / 32768.0f;
	float stickY = float(input->GetRSteckY()) / 32768.0f;

	if (input->GetRSteckX()) {
		cameraAngle_.x += SPEED_CAMERA * stickX;
	}

	if (input->GetRSteckY()) {
		cameraAngle_.y += SPEED_CAMERA * stickY;

		//�ő�l�ݒ�
		if (cameraAngle_.y > 90) {
			cameraAngle_.y = 90;
		}
		//�ŏ��l�ݒ�
		if (cameraAngle_.y < -90) {
			cameraAngle_.y = -90;
		}
	}

	frontVec_.x = float(sin(Radian(cameraAngle_.x)));
	frontVec_.y = float(sin(Radian(cameraAngle_.y)));
	frontVec_.z = float(cos(Radian(cameraAngle_.x)));
	rightVec_.x = float(sin(Radian(cameraAngle_.x + 90)));
	rightVec_.y = float(sin(Radian(cameraAngle_.y)));
	rightVec_.z = float(cos(Radian(cameraAngle_.x + 90)));

	//�J��������
	camera->SetEye(pos_ + CAMERA_EYE);//�ڐ��ɃJ�����𒲐�
	camera->SetTarget(pos_ + frontVec_ + CAMERA_EYE);//�ڐ��ɃJ�����𒲐�
}

void Player::Attack()
{
	//�C���X�^���X�擾
	Input* input = Input::GetInstance();
	PlayerWeapon* weapon = PlayerWeapon::GetInstance();
	SpellManager* spellM = SpellManager::GetInstance();

	bool isAttackOn = false;

	if ((input->TriggerKey(DIK_SPACE) || input->TriggerButton(XINPUT_GAMEPAD_RIGHT_SHOULDER))&& GetIsCanAction()) {
		//�U���t���O�𗧂Ă�
		isAttack_ = true;
		attackTime_ = TIME_ATTACK_NORMAL;
	}
	//�����r��
	if ((input->PushKey(DIK_E) || input->ReleaseKey(DIK_E) || input->PushButton(XINPUT_GAMEPAD_LEFT_SHOULDER) || input->ReleaseButton(XINPUT_GAMEPAD_LEFT_SHOULDER))) {
		switch (presetSpell_)
		{
		case FIRE_BALL:
			spellM->ChargeFireBall();
			break;
		case MAGIC_MISSILE:
			spellM->ChargeMagicMissile();
			break;

		default:
			break;
		}
		
		isSpell_ = true;
	}
	else {
		isSpell_ = false;
	}

	//�U������`�F�b�N
	if (attackTime_ < TIME_ATTACK_NORMAL - TIME_ATTACK_START_NORMAL && attackTime_ > TIME_ATTACK_NORMAL - TIME_ATTACK_END_NORMAL) {
		isAttackOn = true;
	}
	//�U���I��
	if (attackTime_ <= 0) {
		//�U���t���O������
		isAttack_ = false;
		//�G�̑��i�q�b�g����t���O������
		EnemyManager::GetInstance()->ResetIsHit();
	}
	else {
		//���Ԃ����炷
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
	return r * (PI / 180);
}