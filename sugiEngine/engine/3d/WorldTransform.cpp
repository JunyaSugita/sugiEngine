#include "WorldTransform.h"
#include "Camera.h"

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

	Camera* camera = Camera::GetInstance();
	matBillboard = XMMatrixIdentity();
	XMVECTOR cameraTarget = { camera->GetTarget().x,camera->GetTarget().y,camera->GetTarget().z,0 };
	XMVECTOR cameraPos = { camera->GetEye().x,camera->GetEye().y,camera->GetEye().z,0 };

	XMVECTOR cameraAxisZ = XMVectorSubtract(cameraTarget, cameraPos);
	cameraAxisZ = XMVector3Normalize(cameraAxisZ);
	XMVECTOR cameraAxisX = XMVector3Cross({ 0,1,0 }, cameraAxisZ);
	cameraAxisX = XMVector3Normalize(cameraAxisX);
	XMVECTOR cameraAxisY = XMVector3Cross(cameraAxisZ, cameraAxisX);

	matBillboard.r[0] = cameraAxisX;
	matBillboard.r[1] = cameraAxisY;
	matBillboard.r[2] = cameraAxisZ;
	matBillboard.r[3] = XMVectorSet(0, 0, 0, 1);

	Matrix4 matRot = matRotZ * matRotX * matRotY;

	//単位行列を代入
	for (int32_t i = 0; i < 4; i++) {
		for (int32_t j = 0; j < 4; j++) {
			if (i == j) {
				matWorld_.m[i][j] = 1;
			}
			else {
				matWorld_.m[i][j] = 0;
			}
		}
	}
	if (isBillboard) {
		matWorld_ *= matScale * matRot * ConvertToMatrix4(matBillboard) * matTrans;
	}
	else {
		matWorld_ *= matScale * matRot * matTrans;
	}

	if (parent_ != nullptr) {
		matWorld_ *= parent_->matWorld_;
	}
}
