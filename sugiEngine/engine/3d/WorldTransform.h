#pragma once
#include "Matrix4.h"
#include "Vector3.h"
#include <d3d12.h>
#include <dxgi1_6.h>
#include <cassert>
#include <vector>
#include <string>
#include <DirectXMath.h>
#include <DirectXTex.h>

class WorldTransform {
private:
	void SetMatScale(Matrix4& matScale, const Vector3& scale);

	void SetMatTrans(Matrix4& matTrans, const Vector3& trans);

	void SetMatRot(Matrix4& matRot, const Vector3& rot, const char way);

public:
	void SetWorldMat();

	//scale_-----------------------
	//set
	void SetScale(const Vector3& scale) {
		scale_ = scale;
	}
	void SetScaleX(float scale) {
		scale_.x = scale;
	}
	void SetScaleY(float scale) {
		scale_.y = scale;
	}
	void SetScaleZ(float scale) {
		scale_.z = scale;
	}
	//add
	void AddScale(const Vector3& add) {
		scale_ += add;
	}
	void AddScaleX(float add) {
		scale_.x += add;
	}
	void AddScaleY(float add) {
		scale_.y += add;
	}
	void AddScaleZ(float add) {
		scale_.z += add;
	}
	//get
	Vector3 GetScale() {
		return scale_;
	}

	//rot_---------------------
	//set
	void SetRot(const Vector3& rot) {
		rot_ = rot;
	}
	void SetRotX(float rot) {
		rot_.x = rot;
	}
	void SetRotY(float rot) {
		rot_.y = rot;
	}
	void SetRotZ(float rot) {
		rot_.z = rot;
	}
	//add
	void AddRot(const Vector3& add) {
		rot_ += add;
	}
	void AddRotX(float add) {
		rot_.x += add;
	}
	void AddRotY(float add) {
		rot_.y += add;
	}
	void AddRotZ(float add) {
		rot_.z += add;
	}
	//get
	Vector3 GetRot() {
		return rot_;
	}

	//pos_--------------------
	//set
	void SetPos(Vector3 pos) {
		pos_ = pos;
	}
	void SetPosX(float pos) {
		pos_.x = pos;
	}
	void SetPosY(float pos) {
		pos_.y = pos;
	}
	void SetPosZ(float pos) {
		pos_.z = pos;
	}
	//add
	void AddPos(const Vector3& add) {
		pos_ += add;
	}
	void AddPosX(float add) {
		pos_.x += add;
	}
	void AddPosY(float add) {
		pos_.y += add;
	}
	void AddPosZ(float add) {
		pos_.z += add;
	}
	//get
	Vector3 GetPos() {
		return pos_;
	}
	float* GetPosPointerX() {
		return &pos_.x;
	}

	//matWorld_
	void SetMatWorld(const Matrix4& mat) {
		matWorld_ = mat;
	}
	void SetMatWorld(const uint32_t i,const uint32_t& j,const float& num) {
		matWorld_.m[i][j] = num;
	}
	Matrix4 GetMatWorld() {
		return matWorld_;
	}

public:
	//アフィン変換情報
	Vector3 scale_ = { 1,1,1 };
	Vector3 rot_ = { 0,0,0 };
	Vector3 pos_ = { 0,0,0 };

	//ワールド変換行列
	Matrix4 matWorld_;

	//親のオブジェクトへのポインタ
	WorldTransform* parent_ = nullptr;
};

