/**
 * @file Camera.h
 * @brief カメラ
 */

#pragma once
#include "SugiMath.h"
#include "GlobalSetting.h"

class Camera final {
private:
	Camera() = default;
	~Camera() = default;

public:
	//コピーコンストラクタ無効
	Camera(const Camera& obj) = delete;
	//代入演算子を無効
	Camera& operator=(const Camera& obj) = delete;

	static Camera* GetInstance();


public:
	void Initialize();
	void Update();

	//eye
	void SetEye(const Vector3& eye, int32_t num = -1) {
		if (num == -1) {
			eye_[0] = eye;
			return;
		}
		eye_[num] = eye;
	}
	void SetEyeX(float eye, int32_t num = -1) {
		if (num == -1) {
			eye_[0].x = eye;
			return;
		}
		eye_[num].x = eye;
	}
	void SetEyeY(float eye, int32_t num = -1) {
		if (num == -1) {
			eye_[0].y = eye;
			return;
		}
		eye_[num].y = eye;
	}
	void SetEyeZ(float eye, int32_t num = -1) {
		if (num == -1) {
			eye_[0].z = eye;
			return;
		}
		eye_[num].z = eye;
	}
	Vector3 GetEye(int32_t num = -1) {
		if (num == -1) {
			return eye_[0];
		}
		return eye_[num];
	}
	const DirectX::XMFLOAT3* GetEyeXM(int32_t num = -1) {
		if (num == -1) {
			eyeXM_.x = eye_[activeNum_].x;
			eyeXM_.y = eye_[activeNum_].y;
			eyeXM_.z = eye_[activeNum_].z;
		}
		else {
			eyeXM_.x = eye_[num].x;
			eyeXM_.y = eye_[num].y;
			eyeXM_.z = eye_[num].z;
		}

		return &eyeXM_;
	}

	void AddEyeX(float add,int32_t num = -1) {
		if (num == -1) {
			eye_[0].x += add;
			return;
		}
		eye_[num].x += add;
	}
	void AddEyeY(float add, int32_t num = -1) {
		if (num == -1) {
			eye_[0].y += add;
			return;
		}
		eye_[num].y += add;
	}
	void AddEyeZ(float add, int32_t num = -1) {
		if (num == -1) {
			eye_[0].z += add;
			return;
		}
		eye_[num].z += add;
	}

	//target
	void SetTarget(const Vector3& target,int32_t num = -1) {
		if (num == -1) {
			target_[0] = target;
			return;
		}
		target_[num] = target;
	}
	void SetTargetX(float target, int32_t num = -1) {
		if (num == -1) {
			target_[0].x = target;
			return;
		}
		target_[num].x = target;
	}
	void SetTargetY(float target, int32_t num = -1) {
		if (num == -1) {
			target_[0].y = target;
			return;
		}
		target_[num].y = target;
	}
	void SetTargetZ(float target, int32_t num = -1) {
		if (num == -1) {
			target_[0].z = target;
			return;
		}
		target_[num].z = target;
	}
	
	Vector3 GetTarget(int32_t num = -1) {
		if (num == -1) {
			return target_[0];
		}
		return target_[num];
	}

	const DirectX::XMFLOAT3* GetTargetXM(int32_t num = -1) {
		if (num == -1) {
			targetXM_.x = target_[activeNum_].x;
			targetXM_.y = target_[activeNum_].y;
			targetXM_.z = target_[activeNum_].z;
		}
		else {
			targetXM_.x = target_[num].x;
			targetXM_.y = target_[num].y;
			targetXM_.z = target_[num].z;
		}

		return &targetXM_;
	}

	//up
	void SetUp(const Vector3& up,int32_t num = -1) {
		if (num == -1) {
			up_[0] = up;
			return;
		}
		up_[num] = up;
	}
	Vector3 GetUp(int32_t num = -1) {
		if (num == -1) {
			return up_[0];
		}
		return up_[num];
	}
	const DirectX::XMFLOAT3* GetUpXM(int32_t num = -1) {
		if (num == -1) {
			upXM_.x = up_[activeNum_].x;
			upXM_.y = up_[activeNum_].y;
			upXM_.z = up_[activeNum_].z;
		}
		else {
			upXM_.x = up_[num].x;
			upXM_.y = up_[num].y;
			upXM_.z = up_[num].z;
		}

		return &upXM_;
	}

	Matrix4 GetMatView() {
		return matView_;
	}

	Matrix4 GetMatProjection() {
		return matProjection_;
	}

	void SetActive(int32_t num) {
		activeNum_ = num;
	}

	//カメラシェイク
	void SetShake(float num) {
		shake_ = num;
	};
	float GetShake() {
		return shake_;
	}


private:
	static const int32_t MAX_NUM = 10;

	Vector3 eye_[MAX_NUM];	//視点座標
	Vector3 target_[MAX_NUM];	//注視点座標
	Vector3 up_[MAX_NUM];		//上方向ベクトル
	DirectX::XMFLOAT3 eyeXM_;
	DirectX::XMFLOAT3 targetXM_;
	DirectX::XMFLOAT3 upXM_;

	//並行投影行列
	DirectX::XMMATRIX ortho_;
	//透視投影変換行列の計算
	DirectX::XMMATRIX perspective_;

	Matrix4 matProjection_;
	Matrix4 matView_;

	int32_t activeNum_ = 0;

	//カメラシェイク
	float shake_;
};
