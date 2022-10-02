#include "WorldTransform.h"

void WorldTransform::SetMatScale(Matrix4& matScale, const Vector3& scale) {
	matScale.m[0][0] = scale.x;
	matScale.m[1][1] = scale.y;
	matScale.m[2][2] = scale.z;
	matScale.m[3][3] = 1.0f;
};

void WorldTransform::SetMatTrans(Matrix4& matTrans, const Vector3& trans) {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
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
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				if (i == j) {
					matRot.m[i][j] = 1;
				}
				else {
					matRot.m[i][j] = 0;
				}
			}
		}
		matRot.m[0][0] = cos(XMConvertToRadians(rot.z));
		matRot.m[0][1] = sin(XMConvertToRadians(rot.z));
		matRot.m[1][0] = -sin(XMConvertToRadians(rot.z));
		matRot.m[1][1] = cos(XMConvertToRadians(rot.z));
	}
	
	if (way == 'x') {
		//X軸回転
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				if (i == j) {
					matRot.m[i][j] = 1;
				}
				else {
					matRot.m[i][j] = 0;
				}
			}
		}
		matRot.m[1][1] = cos(XMConvertToRadians(rot.x));
		matRot.m[1][2] = sin(XMConvertToRadians(rot.x));
		matRot.m[2][1] = -sin(XMConvertToRadians(rot.x));
		matRot.m[2][2] = cos(XMConvertToRadians(rot.x));
	}
	//Y軸回転
	if (way == 'y') {
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				if (i == j) {
					matRot.m[i][j] = 1;
				}
				else {
					matRot.m[i][j] = 0;
				}
			}
		}
		matRot.m[0][0] = cos(XMConvertToRadians(rot.y));
		matRot.m[0][2] = -sin(XMConvertToRadians(rot.y));
		matRot.m[2][0] = sin(XMConvertToRadians(rot.y));
		matRot.m[2][2] = cos(XMConvertToRadians(rot.y));
	}
}

void WorldTransform::SetWorldMat() {
	//ワールドトランスフォームの初期化
	matWorld =
	{
		1,0,0,0,
		0,1,0,0,
		0,0,1,0,
		0,0,0,1
	};

	Matrix4 matScale;
	SetMatScale(matScale, scale);

	Matrix4 matTrans;
	SetMatTrans(matTrans, trans);

	Matrix4 matRotZ;
	SetMatRot(matRotZ, rotation, 'z');

	Matrix4 matRotX;
	SetMatRot(matRotX, rotation, 'x');

	Matrix4 matRotY;
	SetMatRot(matRotY, rotation, 'y');

	Matrix4 matRot = matRotZ * matRotX * matRotY;

	//単位行列を代入
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (i == j) {
				matWorld.m[i][j] = 1;
			}
			else {
				matWorld.m[i][j] = 0;
			}
		}
	}

	matWorld *= matScale * matRot * matTrans;

	if (parent != nullptr) {
		matWorld *= parent->matWorld;
	}
}