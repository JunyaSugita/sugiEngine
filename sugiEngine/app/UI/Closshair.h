#pragma once
#include "Sprite.h"
#include "GrovalSetting.h"

class Closshair {
private:
	Closshair();
	~Closshair();
public:
	//�R�s�[�R���X�g���N�^����
	Closshair(const Closshair& obj) = delete;
	//������Z�q�𖳌�
	Closshair& operator=(const Closshair& obj) = delete;

	static Closshair* GetInstance();

public:
	void Initialize();
	void Update();
	void Draw();

private:
	uint32_t clossTex_;
	Sprite clossSprite_;
};