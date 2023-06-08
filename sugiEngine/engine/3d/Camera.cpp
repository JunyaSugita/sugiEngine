#include "Camera.h"

Camera::Camera()
{
}


Camera::~Camera()
{
}

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
	for (uint32_t i = 0; i < MAX_NUM; i++) {
		eye_[i] = Vector3(0, 50, -50);	//���_���W
		target_[i] = Vector3(0, 0, 0);	//�����_���W
		up_[i] = Vector3(0, 1, 0);		//������x�N�g��
	}
	xmmatView = XMMatrixLookAtLH(XMLoadFloat3(GetEyeXM()), XMLoadFloat3(GetTargetXM()), XMLoadFloat3(GetUpXM()));
	//matrix4�ɕϊ�
	matView_ = ConvertToMatrix4(xmmatView);
}

void Camera::Update()
{
	XMMATRIX xmmatView;
	xmmatView = XMMatrixLookAtLH(XMLoadFloat3(GetEyeXM()), XMLoadFloat3(GetTargetXM()), XMLoadFloat3(GetUpXM()));
	//matrix4�ɕϊ�
	matView_ = ConvertToMatrix4(xmmatView);
}
