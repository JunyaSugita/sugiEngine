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

	//���Z�b�g
	if (Input::GetInstance()->TriggerKey(DIK_R)) {
		Initialize();
	}
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
		cameraAngle_.y += SPEED_CAMERA;
	}
	if (input->PushKey(DIK_DOWN)) {
		cameraAngle_.y -= SPEED_CAMERA;
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

	if (input->TriggerKey(DIK_SPACE) && GetIsCanAction()) {
		//�U���t���O�𗧂Ă�
		isAttack_ = true;
		attackTime_ = TIME_ATTACK_NORMAL;
	}
	//�t�@�C�A�[�{�[����1�Ɋ��蓖��
	if ((input->PushKey(DIK_1) || input->ReleaseKey(DIK_1))) {
		spellM->ChargeFireBall();
		isSpell_ = true;
	}
	else {
		isSpell_;
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