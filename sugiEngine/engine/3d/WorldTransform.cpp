#include "WorldTransform.h"

using namespace DirectX;

void WorldTransform::SetMatScale(Matrix4& matScale, const Vector3& scale) {
	matScale.m[0][0] = scale.x;
	matScale.m[1][1] = scale.y;
	matScale.m[2][2] = scale.z;
	matScale.m[3][3] = 1.0f;
};

void WorldTransform::SetMatTrans(Matrix4& matTrans, const Vector3& trans) {
	for (int32_t i = 0; i < 4; i++) {
		for (int32_t j = 0; j < 4; j++) {
			if (i == j) {
				matTrans.m[i][j] = 1;
			}
			else {
				matTrans.m[i][j] = 0;
			}
		}
	}

	matTrans.m[3][0] = trans.x;
	matTrans.m[3][1] = trans.y;
	matTrans.m[3][2] = trans.z;
}

void WorldTransform::SetMatRot(Matrix4& matRot, const Vector3& rot, const char way) {

	if (way == 'z') {
		//Z軸回転
		for (int32_t i = 0; i < 4; i++) {
			for (int32_t j = 0; j < 4; j++) {
				if (i == j) {
					matRot.m[i][j] = 1;
				}
				else {
					matRot.m[i][j] = 0;
				}
			}
		}
		matRot.m[0][0] = static_cast<float> (cos(XMConvertToRadians(rot.z)));
		matRot.m[0][1] = static_cast<float> (sin(XMConvertToRadians(rot.z)));
		matRot.m[1][0] = static_cast<float> (-sin(XMConvertToRadians(rot.z)));
		matRot.m[1][1] = static_cast<float> (cos(XMConvertToRadians(rot.z)));
	}

	if (way == 'x') {
		//X軸回転
		for (int32_t i = 0; i < 4; i++) {
			for (int32_t j = 0; j < 4; j++) {
				if (i == j) {
					matRot.m[i][j] = 1;
				}
				else {
					matRot.m[i][j] = 0;
				}
			}
		}
		matRot.m[1][1] = static_cast<float> (cos(XMConvertToRadians(rot.x)));
		matRot.m[1][2] = static_cast<float> (sin(XMConvertToRadians(rot.x)));
		matRot.m[2][1] = static_cast<float> (-sin(XMConvertToRadians(rot.x)));
		matRot.m[2][2] = static_cast<float> (cos(XMConvertToRadians(rot.x)));
	}
	//Y軸回転
	if (way == 'y') {
		for (int32_t i = 0; i < 4; i++) {
			for (int32_t j = 0; j < 4; j++) {
				if (i == j) {
					matRot.m[i][j] = 1;
				}
				else {
					matRot.m[i][j] = 0;
				}
			}
		}
		matRot.m[0][0] = static_cast<float> (cos(XMConvertToRadians(rot.y)));
		matRot.m[0][2] = static_cast<float> (-sin(XMConvertToRadians(rot.y)));
		matRot.m[2][0] = static_cast<float> (sin(XMConvertToRadians(rot.y)));
		matRot.m[2][2] = static_cast<float> (cos(XMConvertToRadians(rot.y)));
	}
}

void WorldTransform::SetWorldMat() {
	//ワールドトランスフォームの初期化
	matWorld_ =
	{
		1,0,0,0,
		0,1,0,0,
		0,0,1,0,
		0,0,0,1
	};

	Matrix4 matScale;
	SetMatScale(matScale, scale_);

	Matrix4 matTrans;
	SetMatTrans(matTrans, pos_);

	Matrix4 matRotZ;
	SetMatRot(matRotZ, rot_, 'z');

	Matrix4 matRotX;
	SetMatRot(matRotX, rot_, 'x');

	Matrix4 matRotY;
	SetMatRot(matRotY, rot_, 'y');

	Matrix4 matRot = matRotZ * matRotX * matRotY;

	matWorld_ *= matScale * matRot * matTrans;

	if (parent_ != nullptr) {
		matWorld_ *= parent_->matWorld_;
	}
}
