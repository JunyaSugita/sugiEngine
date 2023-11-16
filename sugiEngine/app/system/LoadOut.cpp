#include "LoadOut.h"
#include "Input.h"
#include "PostEffectSecond.h"
#include "EnemyManager.h"
#include "FieldManager.h"
#include "Player.h"
#include "PlayerWeapon.h"
#include "ParticleManager.h"
#include "StageSelectManager.h"

LoadOut* LoadOut::GetInstance()
{
	static LoadOut instance;

	return &instance;
}

void LoadOut::Initialize()
{
	isActive_ = false;

	//初回のみの処理
	if (!isFirst_) {
		setSpell_[0] = FIRE_BALL;
		setSpell_[1] = MAGIC_MISSILE;
		setSpell_[2] = ICE_BOLT;
		setSpell_[3] = ENCHANT_FIRE;
	}
	isFirst_ = true;

	selectSpell_ = 0;
	selectNum_ = 0;
	selectMode_ = SELECT_SPELL;

	spellTexNum_[FIRE_BALL] = Sprite::LoadTexture("fireBallIcon.png");
	spellTexNum_[MAGIC_MISSILE] = Sprite::LoadTexture("magicMissileIcon.png");
	spellTexNum_[ICE_BOLT] = Sprite::LoadTexture("iceBolt.png");
	spellTexNum_[CHAIN_LIGHTNING] = Sprite::LoadTexture("chainLightning.png");
	spellTexNum_[ENCHANT_FIRE] = Sprite::LoadTexture("enchantFireIcon.png");
	spellTexNum_[FLAME] = Sprite::LoadTexture("flame.png");
	for (int i = 6; i < SPELL_ALL; i++) {
		spellTexNum_[i] = Sprite::LoadTexture("comingSoonIcon.png");
	}

	for (int i = 0; i < SPELL_ALL; i++) {
		preview_[i].Initialize(spellTexNum_[i]);
	}

	for (int y = 0; y < 4; y++) {
		for (int x = 0; x < SPELL_SET; x++) {
			preview_[y * SPELL_SET + x].SetPos(float(170 + x * 120),float(120 + y * 120));
		}
	}

	hiLight_.Initialize(Sprite::LoadTexture("white1x1.png"));
	hiLight_.SetSize(120,120);
	hiLight_.SetPos(preview_[selectSpell_].GetPos());
	
	back_.Initialize(Sprite::LoadTexture("LoadOut.png"));

	int a = Sprite::LoadTexture("Explan.png");
	for (int i = 0; i < SPELL_ALL; i++) {
		spellExplanTex_[i] = a;
	}
	spellExplanTex_[FIRE_BALL] = Sprite::LoadTexture("fireBallExplan.png");
	spellExplanTex_[MAGIC_MISSILE] = Sprite::LoadTexture("magicMissileExplan.png");
	spellExplanTex_[ICE_BOLT] = Sprite::LoadTexture("iceBoltExplan.png");
	spellExplanTex_[CHAIN_LIGHTNING] = Sprite::LoadTexture("lightningExplan.png");
	spellExplanTex_[ENCHANT_FIRE] = Sprite::LoadTexture("enchantExplan.png");
	spellExplanTex_[FLAME] = Sprite::LoadTexture("flameExplan.png");

	SpellExplanation_.Initialize(spellExplanTex_[0]);
	SpellExplanation_.SetPos({740,300});

	for (int i = 0; i < SPELL_ALL; i++) {
		preview_[i].SetAnchorPoint(0.5f, 0.5f);
	}
	hiLight_.SetAnchorPoint(0.5f, 0.5f);

	for (int i = 0; i < SPELL_SET; i++) {
		set_[i].Initialize(spellTexNum_[setSpell_[i]]);
		set_[i].SetAnchorPoint(0.5f, 0.5f);
	}
	set_[0].SetPos(170, 600);
	set_[1].SetPos(290, 600);
	set_[2].SetPos(410, 600);
	set_[3].SetPos(530, 600);

	preWindowTimer_ = 0;
}

void LoadOut::Update()
{
	Input* input = Input::GetInstance();

	if (isActive_) {
		Player::GetInstance()->GameInitialize();

		PostEffectSecond::SetPos({ 780,50 });
		PostEffectSecond::SetSize({WIN_WIDTH / 3,WIN_HEIGHT / 3});

		//選択中のスペルを使用する
		if (++preWindowTimer_ <= SpellManager::GetInstance()->GetMaxCharge() || SpellManager::GetInstance()->GetMaxCharge() == 0) {
			Player::GetInstance()->ChargeSpell(selectSpell_);
			PlayerWeapon::GetInstance()->PopChargeParticle(SpellManager::GetInstance()->GetSpellType(selectSpell_));
		}
		PlayerWeapon::GetInstance()->Update(false, false);

		//modeごとの異なる処理
		if (selectMode_ == SELECT_SPELL) {
			//セットしたい呪文を選択させる
			if (input->TriggerKey(DIK_D) || input->TriggerLStickRight()) {
				if (selectSpell_ % SPELL_SET == SPELL_SET - 1) {
					selectSpell_ -= SPELL_SET;
				}
				selectSpell_++;
				ResetWindow();
			}
			if (input->TriggerKey(DIK_A) || input->TriggerLStickLeft()) {
				if (selectSpell_ % SPELL_SET == 0) {
					selectSpell_ += SPELL_SET;
				}
				selectSpell_--;
				ResetWindow();
			}
			if (input->TriggerKey(DIK_S) || input->TriggerLStickDown()) {
				if (selectSpell_ >= SPELL_ALL - SPELL_SET) {
					selectSpell_ -= SPELL_ALL;
				}
				selectSpell_+= SPELL_SET;
				ResetWindow();
			}
			if (input->TriggerKey(DIK_W) || input->TriggerLStickUp()) {
				if (selectSpell_ <= SPELL_SET - 1) {
					selectSpell_ += SPELL_ALL;
				}
				selectSpell_ -= SPELL_SET;
				ResetWindow();
			}

			if (input->TriggerKey(DIK_SPACE) || input->TriggerButton(XINPUT_GAMEPAD_A)) {
				selectMode_ = SELECT_NUM;
			}
			if (input->TriggerKey(DIK_0) || input->TriggerButton(XINPUT_GAMEPAD_B)) {
				isActive_ = false;
			}
		}
		else if (selectMode_ == SELECT_NUM) {
			//セットしたい呪文をどこに入れるか選択させる
			if (input->TriggerKey(DIK_D) || input->TriggerLStickRight()) {
				if (selectNum_ % SPELL_SET == SPELL_SET - 1) {
					selectNum_ -= SPELL_SET;
				}
				selectNum_++;
			}
			if (input->TriggerKey(DIK_A) || input->TriggerLStickLeft()) {
				if (selectNum_ % SPELL_SET == 0) {
					selectNum_ += SPELL_SET;
				}
				selectNum_--;
			}

			hiLight_.SetPos(set_[selectNum_].GetPos());

			if (input->TriggerKey(DIK_SPACE) || input->TriggerButton(XINPUT_GAMEPAD_A)) {
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

void LoadOut::BackDrawSp()
{
	if (isActive_) {
		back_.Draw();
	}
}

void LoadOut::Draw()
{
	if (isActive_) {
		hiLight_.Draw();
		for (int i = 0; i < SPELL_ALL; i++) {
			preview_[i].Draw();
		}
		for (int i = 0; i < SPELL_SET; i++) {
			set_[i].Draw();
		}
		SpellExplanation_.Draw();
	}
}

void LoadOut::ResetWindow()
{
	EnemyManager::GetInstance()->GameInitialize();
	FieldManager::GetInstance()->Initialize(SET_SPELL_STAGE);
	Player::GetInstance()->GameInitialize();
	SpellManager::GetInstance()->Initialize();
	ParticleManager::GetInstance()->Clear();
	preWindowTimer_ = 0;
	hiLight_.SetPos(preview_[selectSpell_].GetPos());
	SpellExplanation_.SetTexture(spellExplanTex_[selectSpell_]);
	Enemy::SetIsAllStop(true);
}

void LoadOut::SetSpell(int32_t num, int32_t spellName)
{
	//選択した呪文を選択した番号に入れる
	setSpell_[num] = spellName;
	isDirty_ = true;
}
