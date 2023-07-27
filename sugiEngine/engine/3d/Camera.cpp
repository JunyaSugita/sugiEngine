#include "Camera.h"
#include <random>

using namespace DirectX;

Camera* Camera::GetInstance()
{
	static Camera instance;

	return &instance;
}

void Camera::Initialize()
{
	//���s���e�s��
	ortho_ = XMMatrixOrthographicOffCenterLH(
		0.0f, WIN_WIDTH,
		WIN_HEIGHT, 0.0f,
		0.0f, 1.0f
	);

	//�������e�ϊ��s��̌v�Z
	perspective_ = XMMatrixPerspectiveFovLH(
		XMConvertToRadians(45.0f),			//�㉺��p45�x
		(float)WIN_WIDTH / WIN_HEIGHT,//�A�X�y�N�g��
		0.1f, 1000.0f						//�O�[,���[
	);
	//matrix4�ɕϊ�
	matProjection_ = ConvertToMatrix4(perspective_);

	//�r���[�ϊ��s��
	//�J��������
	XMMATRIX xmmatView;
	activeNum_ = 0;
	for (int32_t i = 0; i < MAX_NUM; i++) {
		eye_[i] = Vector3(0, 50, -50);	//���_���W
		target_[i] = Vector3(0, 0, 0);	//�����_���W
		up_[i] = Vector3(0, 1, 0);		//������x�N�g��
	}
	xmmatView = XMMatrixLookAtLH(XMLoadFloat3(GetEyeXM()), XMLoadFloat3(GetTargetXM()), XMLoadFloat3(GetUpXM()));
	//matrix4�ɕϊ�
	matView_ = ConvertToMatrix4(xmmatView);

	shake_ = 0;
}

void Camera::Update()
{
	//�J�����V�F�C�N����
	if (shake_ > 0) {
		std::random_device seed_gen;
		std::mt19937_64 engine(seed_gen());

		std::uniform_real_distribution<float> x(-shake_, shake_);
		std::uniform_real_distribution<float> y(-shake_, shake_);
		std::uniform_real_distribution<float> z(-shake_, shake_);

		eye_[activeNum_].x += x(engine);
		eye_[activeNum_].y += y(engine);
		eye_[activeNum_].z += z(engine);

		shake_ -= 0.005f;
	}

	XMMATRIX xmmatView;
	xmmatView = XMMatrixLookAtLH(XMLoadFloat3(GetEyeXM()), XMLoadFloat3(GetTargetXM()), XMLoadFloat3(GetUpXM()));
	//matrix4�ɕϊ�
	matView_ = ConvertToMatrix4(xmmatView);
}

void Camera::SetShake(float num)
{
	shake_ = num;
}
