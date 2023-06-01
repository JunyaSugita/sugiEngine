#pragma once
#include "SugiMath.h"
#include "GrovalSetting.h"

class Camera final {
private:
	Camera();
	~Camera();

public:
	//コピーコンストラクタ無効
	Camera(const Camera& obj) = delete;
	//代入演算子を無効
	Camera& operator=(const Camera& obj) = delete;

	static Camera* GetInstance();


public:
	void Initialize();
	void Update();

	void SetEye(Vector3 eye, int num = -1) {
		if (num == -1) {
			eye_[activeNum_] = eye;
			return;
		}
		eye_[num] = eye;
	}
	Vector3 GetEye(int num = -1) {
		if (num == -1) {
			return eye_[activeNum_];
		}
		return eye_[num];
	}
	const XMFLOAT3* GetEyeXM(int num = -1) {
		XMFLOAT3 temp;
		if (num == -1) {
			temp.x = eye_[activeNum_].x;
			temp.y = eye_[activeNum_].y;
			temp.z = eye_[activeNum_].z;
		}
		else {
			temp.x = eye_[num].x;
			temp.y = eye_[num].y;
			temp.z = eye_[num].z;
		}

		return &temp;
	}

	void SetTarget(Vector3 target, int num = -1) {
		if (num == -1) {
			target_[activeNum_] = target;
			return;
		}
		target_[num] = target;
	}
	Vector3 GetTarget(int num = -1) {
		if (num == -1) {
			return target_[activeNum_];
		}
		return target_[num];
	}

	const XMFLOAT3* GetTargetXM(int num = -1) {
		XMFLOAT3 temp;
		if (num == -1) {
			temp.x = target_[activeNum_].x;
			temp.y = target_[activeNum_].y;
			temp.z = target_[activeNum_].z;
		}
		else {
			temp.x = target_[num].x;
			temp.y = target_[num].y;
			temp.z = target_[num].z;
		}

		return &temp;
	}

	void SetUp(Vector3 up, int num = -1) {
		if (num == -1) {
			up_[activeNum_] = up;
			return;
		}
		up_[num] = up;
	}
	Vector3 GetUp(int num = -1) {
		if (num == -1) {
			return up_[activeNum_];
		}
		return up_[num];
	}
	const XMFLOAT3* GetUpXM(int num = -1) {
		XMFLOAT3 temp;
		if (num == -1) {
			temp.x = up_[activeNum_].x;
			temp.y = up_[activeNum_].y;
			temp.z = up_[activeNum_].z;
		}
		else {
			temp.x = up_[num].x;
			temp.y = up_[num].y;
			temp.z = up_[num].z;
		}

		return &temp;
	}

	Matrix4 GetMatView() {
		return matView;
	}

	Matrix4 GetMatProjection() {
		return matProjection;
	}


private:
	static const int MAX_NUM = 10;

	Vector3 eye_[MAX_NUM];	//視点座標
	Vector3 target_[MAX_NUM];	//注視点座標
	Vector3 up_[MAX_NUM];		//上方向ベクトル

	//並行投影行列
	XMMATRIX ortho;
	//透視投影変換行列の計算
	XMMATRIX perspective;

	Matrix4 matProjection;
	Matrix4 matView;

	int activeNum_ = 0;
};
