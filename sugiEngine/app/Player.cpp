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

#pragma region ƒJƒƒ‰‘€ì
	//ƒJƒƒ‰‘€ì
	Camera* camera = Camera::GetInstance();

	camera->SetEye(pos_);
	camera->AddEyeY(CAMERA_Y);
	camera->SetTarget(pos_);
	camera->SetTargetY(pos_.y + CAMERA_Y);
	camera->SetTargetZ(pos_.z + 1);

#pragma endregion

#pragma region ˆÚ“®ˆ—
	Input* input = Input::GetInstance();

	if (input->PushKey(DIK_W)) {
		pos_.z += SPEED;
	}
	if (input->PushKey(DIK_S)) {
		pos_.z -= SPEED;
	}
	if (input->PushKey(DIK_A)) {
		pos_.x -= SPEED;
	}
	if (input->PushKey(DIK_D)) {
		pos_.x += SPEED;
	}

	if (input->TriggerKey(DIK_R)) {
		Initialize();
	}

#pragma endregion


}

void Player::Draw()
{

}
