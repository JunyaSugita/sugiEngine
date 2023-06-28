#pragma once
#include "SugiMath.h"
#include "Graund.h"

#include <memory>

class GroundManager {
private:
	GroundManager();
	~GroundManager();

public:
	//コピーコンストラクタ無効
	GroundManager(const GroundManager& obj) = delete;
	//代入演算子を無効
	GroundManager& operator=(const GroundManager& obj) = delete;

	static GroundManager* GetInstance();

public:
	void Initialize();
	void Update();
	void Draw();

public:
	std::unique_ptr<Graund> grounds_[25];

};