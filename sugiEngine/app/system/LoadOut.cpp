#include "LoadOut.h"
#include "Input.h"
#include "PostEffectSecond.h"

LoadOut* LoadOut::GetInstance()
{
	static LoadOut instance;

	return &instance;
}

void LoadOut::Initialize()
{
	isActive_ = true;
	setSpell_[0] = FIRE_BALL;
	setSpell_[1] = MAGIC_MISSILE;
	setSpell_[2] = ICE_BOLT;
	setSpell_[3] = CHAIN_LIGHTNING;
	setSpell_[4] = ENCHANT_FIRE;

	selectSpell_ = 0;
	selectNum_ = 0;
	selectMode_ = SELECT_SPELL;

	spellTexNum_[FIRE_BALL] = Sprite::LoadTexture("fireBallIcon.png");
	spellTexNum_[MAGIC_MISSILE] = Sprite::LoadTexture("magicMissileIcon.png");
	spellTexNum_[ICE_BOLT] = Sprite::LoadTexture("iceBolt.png");
	spellTexNum_[CHAIN_LIGHTNING] = Sprite::LoadTexture("chainLightning.png");
	spellTexNum_[ENCHANT_FIRE] = Sprite::LoadTexture("enchantFireIcon.png");
	for (int i = 5; i < 20; i++) {
		spellTexNum_[i] = Sprite::LoadTexture("comingSoonIcon.png");
	}

	preview_[FIRE_BALL].Initialize(spellTexNum_[FIRE_BALL]);
	preview_[FIRE_BALL].SetPos(170, 150);

	preview_[MAGIC_MISSILE].Initialize(spellTexNum_[MAGIC_MISSILE]);
	preview_[MAGIC_MISSILE].SetPos(290, 150);

	preview_[ICE_BOLT].Initialize(spellTexNum_[ICE_BOLT]);
	preview_[ICE_BOLT].SetPos(410, 150);

	preview_[CHAIN_LIGHTNING].Initialize(spellTexNum_[CHAIN_LIGHTNING]);
	preview_[CHAIN_LIGHTNING].SetPos(530, 150);

	preview_[ENCHANT_FIRE].Initialize(spellTexNum_[ENCHANT_FIRE]);
	preview_[ENCHANT_FIRE].SetPos(650, 150);

	for (int i = 0; i < 20; i++) {
		preview_[i].Initialize(spellTexNum_[i]);
	}

	for (int y = 0; y < 4; y++) {
		for (int x = 0; x < 5; x++) {
			preview_[y * 5 + x].SetPos(float(170 + x * 120),float(120 + y * 120));
		}
	}

	hiLight_.Initialize(Sprite::LoadTexture("white1x1.png"));
	hiLight_.SetSize(120,120);
	hiLight_.SetPos(preview_[selectSpell_].GetPos());

	for (int i = 0; i < 20; i++) {
		preview_[i].SetAnchorPoint(0.5f, 0.5f);
	}
	hiLight_.SetAnchorPoint(0.5f, 0.5f);

	for (int i = 0; i < 5; i++) {
		set_[i].Initialize(spellTexNum_[setSpell_[i]]);
		set_[i].SetAnchorPoint(0.5f, 0.5f);
	}
	set_[0].SetPos(170, 600);
	set_[1].SetPos(290, 600);
	set_[2].SetPos(410, 600);
	set_[3].SetPos(530, 600);
	set_[4].SetPos(650, 600);
}

void LoadOut::Update()
{
	Input* input = Input::GetInstance();

	if (isActive_) {
		PostEffectSecond::SetPos({ 820,50 });
		PostEffectSecond::SetSize({WIN_WIDTH / 3,WIN_HEIGHT / 3});

		//modeごとの異なる処理
		if (selectMode_ == SELECT_SPELL) {
			//セットしたい呪文を選択させる
			if (input->TriggerKey(DIK_D)) {
				if (selectSpell_ % 5 == 4) {
					selectSpell_ -= 5;
				}
				selectSpell_++;
			}
			if (input->TriggerKey(DIK_A)) {
				if (selectSpell_ % 5 == 0) {
					selectSpell_ += 5;
				}
				selectSpell_--;
			}
			if (input->TriggerKey(DIK_S)) {
				if (selectSpell_ >= 15) {
					selectSpell_ -= 20;
				}
				selectSpell_+= 5	;
			}
			if (input->TriggerKey(DIK_W)) {
				if (selectSpell_ <= 4) {
					selectSpell_ += 20;
				}
				selectSpell_ -= 5;
			}

			hiLight_.SetPos(preview_[selectSpell_].GetPos());

			if (input->TriggerKey(DIK_SPACE)) {
				selectMode_ = SELECT_NUM;
			}
			if (input->TriggerKey(DIK_0) || input->TriggerButton(XINPUT_GAMEPAD_B)) {
				isActive_ = false;
			}
		}
		else if (selectMode_ == SELECT_NUM) {
			//セットしたい呪文をどこに入れるか選択させる
			if (input->TriggerKey(DIK_D)) {
				if (selectNum_ % 5 == 4) {
					selectNum_ -= 5;
				}
				selectNum_++;
			}
			if (input->TriggerKey(DIK_A)) {
				if (selectNum_ % 5 == 0) {
					selectNum_ += 5;
				}
				selectNum_--;
			}

			hiLight_.SetPos(set_[selectNum_].GetPos());

			if (input->TriggerKey(DIK_SPACE)) {
				SetSpell(selectNum_,selectSpell_);
				set_[selectNum_].SetTexture(spellTexNum_[selectSpell_]);
				selectMode_ = SELECT_SPELL;
			}
			if (input->TriggerKey(DIK_0) || input->TriggerButton(XINPUT_GAMEPAD_B)) {
				selectMode_ = SELECT_SPELL;
			}
		}

	}
	else {
		PostEffectSecond::SetPos({ 0,0 });
		PostEffectSecond::SetSize({ WIN_WIDTH,WIN_HEIGHT});
	}
}

void LoadOut::Draw()
{
	if (isActive_) {
		hiLight_.Draw();
		for (int i = 0; i < 20; i++) {
			preview_[i].Draw();
		}
		for (int i = 0; i < 5; i++) {
			set_[i].Draw();
		}
	}
}

void LoadOut::SetSpell(int32_t num, int32_t spellName)
{
	//選択した呪文を選択した番号に入れる
	setSpell_[num] = spellName;
	isDirty_ = true;
}
