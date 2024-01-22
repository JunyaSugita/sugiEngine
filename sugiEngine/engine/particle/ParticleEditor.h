/**
 * @file ParticleEditor.h
 * @brief デバッグ時にパーティクルを自作出来るようにする
 */

#pragma once
#include "SugiMath.h"
#include <string>

struct EditFile {
	int32_t texNum;
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
	Vector3 sColor;
	Vector3 eColor;
	int32_t postEffect;
	bool add1;
	int32_t texNum1;
	int32_t num1;
	uint32_t life1;
	uint32_t lifeRand1;
	Vector3 pos1;
	Vector3 posRand1;
	bool isRevers1;
	Vector3 move1;
	Vector3 moveRand1;
	float speed1;
	Vector3 acceleration1;
	Vector2 scale1;
	float angleSpeed1;
	float angleSpeedRand1;
	Vector3 gravity1;
	Vector3 sColor1;
	Vector3 eColor1;
	int32_t postEffect1;
	bool add2;
	int32_t texNum2;
	int32_t num2;
	uint32_t life2;
	uint32_t lifeRand2;
	Vector3 pos2;
	Vector3 posRand2;
	bool isRevers2;
	Vector3 move2;
	Vector3 moveRand2;
	float speed2;
	Vector3 acceleration2;
	Vector2 scale2;
	float angleSpeed2;
	float angleSpeedRand2;
	Vector3 gravity2;
	Vector3 sColor2;
	Vector3 eColor2;
	int32_t postEffect2;
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

	void PopParticle(uint8_t num);

	bool GetIsEdit(uint8_t num) {
		return isEdit_[num];
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

public:
	static const uint8_t particleNum = 3;

private:
	char text_[16];
	bool isEdit_[particleNum];
	bool isTextureRand_[particleNum];
	int32_t texNum_[particleNum];
	int32_t texNumRand_[particleNum];
	int32_t num_[particleNum];
	int32_t life_[particleNum];
	int32_t lifeRand_[particleNum];
	float scale_[particleNum][2];
	float sColor_[particleNum][3];
	float eColor_[particleNum][3];
	bool isRevers_[particleNum];
	float pos_[particleNum][3];
	float posRand_[particleNum][3];
	float move_[particleNum][3];
	float moveRand_[particleNum][3];
	float speed_[particleNum];
	float acceleration_[particleNum][3];
	float angleSpeed_[particleNum];
	float angleSpeedRand_[particleNum];
	float gravity_[particleNum][3];
	int32_t postEffect_[particleNum];

	FILE* saveFile_;
	EditFile editData_;

	char particleName_[100][16];
};
