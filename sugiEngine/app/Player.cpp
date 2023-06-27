#include "Player.h"
#include "Camera.h"
#include "Input.h"

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
	cameraAngle_ = { 0,0 };
}

void Player::Update()
{
	CameraMove();
	Move();

	//���Z�b�g
	if (Input::GetInstance()->TriggerKey(DIK_R)) {
		Initialize();
	}
}

void Player::Draw()
{

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

float Radian(float r) {
	return r * (PI / 180);
}