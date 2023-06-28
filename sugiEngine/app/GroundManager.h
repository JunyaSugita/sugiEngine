#pragma once
#include "SugiMath.h"
#include "Graund.h"

#include <memory>

class GroundManager {
private:
	GroundManager();
	~GroundManager();

public:
	//�R�s�[�R���X�g���N�^����
	GroundManager(const GroundManager& obj) = delete;
	//������Z�q�𖳌�
	GroundManager& operator=(const GroundManager& obj) = delete;

	static GroundManager* GetInstance();

public:
	void Initialize();
	void Update();
	void Draw();

public:
	std::unique_ptr<Graund> grounds_[25];

};