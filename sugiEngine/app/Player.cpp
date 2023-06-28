#include "Player.h"
#include "Camera.h"
#include "Input.h"
#include "EnemyManager.h"

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
	life_ = 10;
	isAttack_ = false;
}

void Player::Update()
{
	CameraMove();
	Move();

	

	//リセット
	if (Input::GetInstance()->TriggerKey(DIK_R)) {
		Initialize();
	}
}

void Player::Draw()
{

}

void Player::Move()
{
	//インスタンス取得
	Input* input = Input::GetInstance();

	Vector3 moveZ= { frontVec_.x,0,frontVec_.z };
	moveZ.normalize();
	Vector3 moveX = { rightVec_.x,0,rightVec_.z };
	moveX.normalize();
	//移動
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

	//カメラ操作
	camera->SetEye(pos_ + CAMERA_EYE);//目線にカメラを調整
	camera->SetTarget(pos_ + frontVec_ + CAMERA_EYE);//目線にカメラを調整
}

void Player::Attack()
{
	//インスタンス取得
	Input* input = Input::GetInstance();

	if (input->TriggerKey(DIK_SPACE) && !isAttack_) {
		//攻撃フラグを立てる
		isAttack_ = true;
		attackTime_ = TIME_ATTACK_NORMAL;
		//攻撃判定チェック
		if (attackTime_ < TIME_ATTACK_NORMAL - TIME_ATTACK_START_NORMAL && attackTime_ > TIME_ATTACK_NORMAL - TIME_ATTACK_END_NORMAL) {

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
	}
}

float Radian(float r) {
	return r * (PI / 180);
}