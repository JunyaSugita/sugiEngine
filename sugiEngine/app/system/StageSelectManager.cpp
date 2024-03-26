#include "StageSelectManager.h"
#include "Input.h"
#include "SceneChange.h"
#include "FieldManager.h"
#include "EnemyManager.h"
#include "Player.h"

StageSelectManager* StageSelectManager::GetInstance()
{
	static StageSelectManager instance;

	return &instance;
}

void StageSelectManager::Initialize()
{
	stageTex_[0].Initialize(Sprite::LoadTexture("stageSelect_Tutorial", "png"));
	stageTex_[1].Initialize(Sprite::LoadTexture("stageSelect_stage1", "png"));
	stageTex_[2].Initialize(Sprite::LoadTexture("stageSelect_stage2", "png"));
	for (int i = 0; i < END_STAGE_ID; i++) {
		stageTex_[i].SetAnchorPoint(0, 0.5f);
		texSize_[i] = { 500,100 };
	}
	
	
	selectNum_ = 0;

	originPos_ = { 100,300 };
	nowPos_ = originPos_;

	GameInitialize();
}

void StageSelectManager::GameInitialize()
{
	moveTimer_ = TIME_MOVE;
	Player::GetInstance()->Initialize();
	EnemyManager::GetInstance()->Update();
}

void StageSelectManager::Update()
{
	Input* input = Input::GetInstance();

	moveTimer_--;
	if (!SceneChange::GetInstance()->GetIsSceneChange()) {
		if ((input->PushKey(DIK_S) || input->GetLStickY() < -DEAD_ZONE_STICK || input->PushButton(XINPUT_GAMEPAD_DPAD_DOWN)) && selectNum_ < END_STAGE_ID - 1 && moveTimer_ <= 0) {
			selectNum_++;
			moveTimer_ = TIME_MOVE;
			EnemyManager::GetInstance()->GameInitialize();
			FieldManager::GetInstance()->Initialize(selectNum_);
		}
		else if ((input->PushKey(DIK_W) || input->GetLStickY() > DEAD_ZONE_STICK || input->PushButton(XINPUT_GAMEPAD_DPAD_UP)) && selectNum_ > 0 && moveTimer_ <= 0) {
			selectNum_--;
			moveTimer_ = TIME_MOVE;
			EnemyManager::GetInstance()->GameInitialize();
			FieldManager::GetInstance()->Initialize(selectNum_);
		}
	}

	//表示を動かす
	Vector2 tempVec2 = originPos_;
	tempVec2.y = originPos_.y - selectNum_ * DISTANCE;

	//上下に移動
	if (tempVec2.y != nowPos_.y) {
		if (tempVec2.y > nowPos_.y) {
			nowPos_.y += SPEED_MOVE;
		}
		else if (tempVec2.y < nowPos_.y) {
			nowPos_.y -= SPEED_MOVE;
		}
	}
	//サイズを変更
	for (int i = 0; i < END_STAGE_ID; i++) {
		if (i != selectNum_) {
			if (texSize_[i].x > MAX_TEXT_SIZE) {
				texSize_[i].x -= SPEED_CHANGE_TEXT_SIZE;
				texSize_[i].y--;
			}
		}
		else {
			if (moveTimer_ > 0) {
				texSize_[i].x += SPEED_CHANGE_TEXT_SIZE;
				texSize_[i].y++;
			}
		}
	}

	//ポジション確定
	for (int i = 0; i < END_STAGE_ID; i++) {
		stageTex_[i].SetPos(nowPos_.x, nowPos_.y + i * DISTANCE);
	}
	//サイズ確定
	for (int i = 0; i < END_STAGE_ID; i++) {
		stageTex_[i].SetSize(texSize_[i].x, texSize_[i].y);
	}
}

void StageSelectManager::Draw()
{
	for (int i = 0; i < END_STAGE_ID; i++) {
		stageTex_[i].Draw();
	}
}
