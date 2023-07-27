#pragma once
#include "Sprite.h"

class Tutorial final{
private:
	Tutorial() = default;
	~Tutorial() = default;
public:
	//�R�s�[�R���X�g���N�^����
	Tutorial(const Tutorial& obj) = delete;
	//������Z�q�𖳌�
	Tutorial& operator=(const Tutorial& obj) = delete;

	static Tutorial* GetInstance();

public:
	void Initialize();
	void Update();
	void Draw();


private:
	uint32_t tex_;
	uint32_t tex2_;
	uint32_t tex3_;
	uint32_t tex4_;
	uint32_t tex5_;

	Sprite sprite_;

	int32_t time_;
	uint8_t number_;
	bool isNext_;
};