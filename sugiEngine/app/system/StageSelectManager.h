#pragma once
#include "Sprite.h"

enum STAGE_ID {
	TUTORIAL,
	STAGE1,
	STAGE2,
	SET_SPELL_STAGE,
	END_STAGE_ID,
};

class StageSelectManager {
private:
	StageSelectManager() = default;
	~StageSelectManager() = default;

public:
	//コピーコンストラクタ無効
	StageSelectManager(const StageSelectManager& obj) = delete;
	//代入演算子を無効
	StageSelectManager& operator=(const StageSelectManager& obj) = delete;

	static StageSelectManager* GetInstance();

	void Initialize();
	void GameInitialize();
	void Update();
	void Draw();

	int32_t GetSelectNum() {
		return selectNum_;
	}

private:
	const int32_t DISTANCE = 200;
	const int32_t TIME_MOVE = 10;
	const int32_t SPEED_MOVE = DISTANCE / TIME_MOVE;

	Sprite stageTex_[END_STAGE_ID];
	Vector2 texSize_[END_STAGE_ID];
	int32_t selectNum_;
	Vector2 originPos_;
	Vector2 nowPos_;
	int32_t moveTimer_;
};