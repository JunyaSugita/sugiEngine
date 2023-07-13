#pragma once
#include "SugiMath.h"
#include <string>

struct EditFile {
	int32_t num;
	uint32_t life;
	Vector3 pos;
	Vector2 scale;
	Vector3 move;
	Vector3 moveRand;
	Vector3 acceleration;
	Vector3 gravity;
	Vector4 color;
	bool add1;
	int32_t num1;
	uint32_t life1;
	Vector3 pos1;
	Vector2 scale1;
	Vector3 move1;
	Vector3 moveRand1;
	Vector3 acceleration1;
	Vector3 gravity1;
	Vector4 color1;
	bool add2;
	int32_t num2;
	uint32_t life2;
	Vector3 pos2;
	Vector2 scale2;
	Vector3 move2;
	Vector3 moveRand2;
	Vector3 acceleration2;
	Vector3 gravity2;
	Vector4 color2;
};

class ParticleEditer {
public:
	void Initialize();
	void Update();

	void PopParticle(uint8_t num);

	bool GetIsEdit(uint8_t num) {
		return isEdit_[num];
	}

	void Save();
	void Write();
	void Load();
	void Read();

public:
	static const uint8_t particleNum = 3;

private:
	char text_[16];
	bool isEdit_[particleNum];
	int32_t num_[particleNum];
	int32_t life_[particleNum];
	float scale_[particleNum][2];
	float color_[particleNum][4];
	float pos_[particleNum][3];
	float move_[particleNum][3];
	float moveRand_[particleNum][3];
	float acceleration_[particleNum][3];
	float gravity_[particleNum][3];

	FILE* saveFile_;
	EditFile editData_;
};