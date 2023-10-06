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
	//並行投影行列
	ortho_ = XMMatrixOrthographicOffCenterLH(
		0.0f, WIN_WIDTH,
		WIN_HEIGHT, 0.0f,
		0.0f, 1.0f
	);

	//透視投影変換行列の計算
	perspective_ = XMMatrixPerspectiveFovLH(
		XMConvertToRadians(45.0f),			//上下画角45度
		(float)WIN_WIDTH / WIN_HEIGHT,//アスペクト比
		0.1f, 1000.0f						//前端,奥端
	);
	//matrix4に変換
	matProjection_ = ConvertToMatrix4(perspective_);

	//ビュー変換行列
	//カメラ操作
	XMMATRIX xmmatView;
	activeNum_ = 0;
	for (int32_t i = 0; i < MAX_NUM; i++) {
		eye_[i] = Vector3(0, 50, -50);	//視点座標
		target_[i] = Vector3(0, 0, 0);	//注視点座標
		up_[i] = Vector3(0, 1, 0);		//上方向ベクトル
	}
	xmmatView = XMMatrixLookAtLH(XMLoadFloat3(GetEyeXM()), XMLoadFloat3(GetTargetXM()), XMLoadFloat3(GetUpXM()));
	//matrix4に変換
	matView_ = ConvertToMatrix4(xmmatView);

	shake_ = 0;
}

void Camera::Update()
{
	//カメラシェイク処理
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
	//matrix4に変換
	matView_ = ConvertToMatrix4(xmmatView);
}
