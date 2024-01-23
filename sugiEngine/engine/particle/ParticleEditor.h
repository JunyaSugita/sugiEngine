/**
 * @file ParticleEditor.h
 * @brief デバッグ時にパーティクルを自作出来るようにする
 */

#pragma once
#include "SugiMath.h"
#include <string>

struct EditFile {
	int32_t version;
	bool isRandTex;
	int32_t texNum;
	int32_t randTexNum;
	int32_t num;
	uint32_t life;
	uint32_t lifeRand;
	Vector3 pos;
	Vector3 posRand;
	bool isRevers;
	Vector3 move;
	Vector3 moveRand;
	float speed;
	Vector3 acceleration;
	Vector2 scale;
	float angleSpeed;
	float angleSpeedRand;
	Vector3 gravity;
	Vector4 sColor;
	float check1;
	Vector4 check1Color;
	float check2;
	Vector4 check2Color;
	Vector4 eColor;
	int32_t postEffect;
};

enum P_POSTEFFECT {
	P_NONE,
	P_BLOOM,
	P_CROSS,
};

class ParticleEditor{
public:
	void Initialize();
	void Update();

	void PopParticle();

	bool GetIsEdit() {
		return isEdit_;
	}

	const char* GetParticleName(int num) {
		return particleName_[num];
	}

	void Save();
	void Write();
	void Load();
	void Read();
	void Delete();

	void SetParticleData();

private:
	char text_[16];
	bool isEdit_;
	int32_t version_;
	bool isTextureRand_;
	int32_t texNum_;
	int32_t texNumRand_;     
	int32_t num_;
	int32_t life_;
	int32_t lifeRand_;
	float pos_[3];
	float posRand_[3];
	bool isRevers_;
	float move_[3];
	float moveRand_[3];
	float speed_;
	float acceleration_[3];
	float scale_[2];
	float angleSpeed_;
	float angleSpeedRand_;
	float gravity_[3];
	float sColor_[4];
	float check1_;
	float check1Color_[4];
	float check2_;
	float check2Color_[4];
	float eColor_[4];
	int32_t postEffect_;

	FILE* saveFile_;
	EditFile editData_;

	char particleName_[100][16];
	bool isPop_[100];
	bool isP_;
};