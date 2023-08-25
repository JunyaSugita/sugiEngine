#include "Player.h"
#include "Camera.h"
#include "Input.h"
#include "EnemyManager.h"
#include "PlayerWeapon.h"
#include "SpellManager.h"
#include "UIManager.h"
#include "GameManager.h"
#include "Tutorial.h"
#include "LoadOut.h"
#include "ItemManager.h"
#include "ClearChecker.h"

Player* Player::GetInstance()
{
	static Player instance;

	return &instance;
}

void Player::Initialize()
{
	PlayerWeapon::GetInstance()->Initialize();
	ItemManager::GetInstance()->Initialize();

	damageTex_ = Sprite::LoadTexture("damage.png");
	damageSp_.Initialize(damageTex_);

	isInvincible_ = false;
	
	GameInitialize();
}

void Player::GameInitialize()
{
	pos_ = { 0,0,0 };
	rot_ = { 0,0,0 };
	scale_ = { 1,1,1 };
	cameraAngle_ = { 0,0 };
	life_ = MAX_LIFE;
	isAttack_ = false;
	presetSpell_ = 0;
	spellAngle_ = 0;

	boxCol_.pos = pos_;
	boxCol_.size = { 2.5f,2.2f,2.5f };
	oldBoxCol_.pos = pos_;
	oldBoxCol_.size = { 2.5f,2.2f,2.5f };

	damageSp_.SetColor(1, 1, 1, 0);
	damageAlpha_ = 0;
	naveTimer_ = 10000;
}

void Player::Update()
{
	//��e���o
	if (damageAlpha_ > 0) {
		damageAlpha_ -= 0.01f;
		damageSp_.SetColor(1, 1, 1, damageAlpha_);
	}

	//�Q�[���I��or�Q�[���I�[�o�[�œ����Ȃ�����
	if (ClearChecker::GetInstance()->GetIsClear() || life_ <= 0) {
		PlayerWeapon::GetInstance()->Update(false,false);
		return;
	}
	//���[�h�A�E�g�ύX���������Ȃ�����
	if (LoadOut::GetInstance()->GetIsActive()) {
		return;
	}

	//1�t���[���O�̏���ۑ�
	oldBoxCol_.pos = pos_;

	Move();
	WorldTransUpdate();

	//�U��
	Attack();

	CameraMove();
}

void Player::Draw()
{
	ItemManager::GetInstance()->Draw();
	PlayerWeapon::GetInstance()->Draw();
}

void Player::SpDraw()
{
	ItemManager::GetInstance()->DrawSprite();
	damageSp_.Draw();
}

void Player::SubLife()
{
	if (life_ > 0) {
		if (!Tutorial::GetInstance()->GetIsTutorial() || isInvincible_) {
			life_-= 1000;
		}
		damageAlpha_ = 1.0f;
		Camera::GetInstance()->SetShake(0.05f);
	}
}

bool Player::GetIsCanAction()
{
	if (isAttack_ || isSpell_ || SpellManager::GetInstance()->GetIsUseSpell() || ItemManager::GetInstance()->GetIsUse()) {
		return false;
	}
	return true;
}

void Player::Move()
{
	//�C���X�^���X�擾
	Input* input = Input::GetInstance();

	Vector3 moveZ = { frontVec_.x,0,frontVec_.z };
	moveZ.normalize();
	Vector3 moveX = { rightVec_.x,0,rightVec_.z };
	moveX.normalize();

	//�ړ����x�ቺ����
	float slow = 1;
	if (!GetIsCanAction()) {
		slow = SPEED_SLOW;
	}

	//�ړ�
	if (input->PushKey(DIK_W)) {
		pos_ += moveZ * SPEED_MOVE * slow;
	}
	if (input->PushKey(DIK_S)) {
		pos_ -= moveZ * SPEED_MOVE * slow;
	}
	if (input->PushKey(DIK_A)) {
		pos_ -= moveX * SPEED_MOVE * slow;
	}
	if (input->PushKey(DIK_D)) {
		pos_ += moveX * SPEED_MOVE * slow;
	}

	float stickX = float(input->GetLStickX()) / 32768.0f;
	float stickY = float(input->GetLStickY()) / 32768.0f;

	//�ړ�
	if (input->GetLStickY()) {
		pos_ += moveZ * SPEED_MOVE * stickY * slow;
	}
	if (input->GetLStickX()) {
		pos_ += moveX * SPEED_MOVE * stickX * slow;
	}

	//�����蔻��ړ�
	boxCol_.pos = pos_;

	//navePoint�̏d�݂Â�
	if (++naveTimer_ > TIME_NAVE) {
		ColliderManager::GetInstance()->SetNavePointScore();
		naveTimer_ = 0;
	}

	//�|�[�V�����q�[��
	HealLife();
}

void Player::CameraMove()
{
	//�C���X�^���X�擾
	Input* input = Input::GetInstance();
	Camera* camera = Camera::GetInstance();

	//�J��������
	if (input->PushKey(DIK_LEFT)) {
		cameraAngle_.x -= SPEED_CAMERA;
	}
	if (input->PushKey(DIK_RIGHT)) {
		cameraAngle_.x += SPEED_CAMERA;
	}
	if (input->PushKey(DIK_UP)) {
		//�ő�l�ݒ�
		if (cameraAngle_.y <= 90) {
			cameraAngle_.y += SPEED_CAMERA;
		}
	}
	if (input->PushKey(DIK_DOWN)) {
		//�ŏ��l�ݒ�
		if (cameraAngle_.y >= -90) {
			cameraAngle_.y -= SPEED_CAMERA;
		}
	}

	float stickX = float(input->GetRStickX()) / 32768.0f;
	float stickY = float(input->GetRStickY()) / 32768.0f;

	if (input->GetLTrigger() < 50 && !(input->TriggerKey(DIK_1) || input->TriggerKey(DIK_2) || input->TriggerKey(DIK_3) || input->TriggerKey(DIK_4) || input->TriggerKey(DIK_5))) {
		if (input->GetRStickX()) {
			cameraAngle_.x += SPEED_CAMERA * stickX;
		}

		if (input->GetRStickY()) {
			cameraAngle_.y += SPEED_CAMERA * stickY;

			//�ő�l�ݒ�
			if (cameraAngle_.y > RAD / 2) {
				cameraAngle_.y = RAD / 2;
			}
			//�ŏ��l�ݒ�
			if (cameraAngle_.y < -RAD / 2) {
				cameraAngle_.y = -RAD / 2;
			}
		}
	}
	else {
		Vector2 len = Vector2(input->GetRStickX(), input->GetRStickY());
		float length = len.length();
		if (len.length() > 20000 || input->TriggerKey(DIK_1) || input->TriggerKey(DIK_2) || input->TriggerKey(DIK_3) || input->TriggerKey(DIK_4) || input->TriggerKey(DIK_5)) {
			len.normalize();
			if (input->GetRStickX() != 0 && input->GetRStickY() != 0) {
				spellAngle_ = (atan2(len.cross({ 0,-1 }), -len.dot({ 0,-1 })) / PI * -RAD - (RAD / 2)) + RAD / 2 * 3;
			}

			//�X�e�B�b�N��|���������̎����ɕς���
			if (spellAngle_ >= RAD && spellAngle_ < 72 + RAD) {
				presetSpell_ = 0;
				SpellManager::GetInstance()->ResetChargeTime();
			}
			else if (spellAngle_ >= 72 + RAD && spellAngle_ < 144 + RAD) {
				presetSpell_ = 1;
				SpellManager::GetInstance()->ResetChargeTime();
			}
			else if (spellAngle_ >= 144 + RAD && spellAngle_ < 180 + RAD || spellAngle_ >= 0 && spellAngle_ < 36) {
				presetSpell_ = 2;
				SpellManager::GetInstance()->ResetChargeTime();
			}
			else if (spellAngle_ >= 36 && spellAngle_ < 108) {
				presetSpell_ = 3;
				SpellManager::GetInstance()->ResetChargeTime();
			}
			else if (spellAngle_ >= 108 && spellAngle_ < RAD) {
				presetSpell_ = 4;
				SpellManager::GetInstance()->ResetChargeTime();
			}

			//�L�[�Ή�
			if (input->TriggerKey(DIK_1)) {
				presetSpell_ = 0;
				SpellManager::GetInstance()->ResetChargeTime();
			}
			else if (input->TriggerKey(DIK_2)) {
				presetSpell_ = 1;
				SpellManager::GetInstance()->ResetChargeTime();
			}
			else if (input->TriggerKey(DIK_3)) {
				presetSpell_ = 2;
				SpellManager::GetInstance()->ResetChargeTime();
			}
			else if (input->TriggerKey(DIK_4)) {
				presetSpell_ = 3;
				SpellManager::GetInstance()->ResetChargeTime();
			}
			else if (input->TriggerKey(DIK_5)) {
				presetSpell_ = 4;
				SpellManager::GetInstance()->ResetChargeTime();
			}
			else if (input->TriggerKey(DIK_6)) {
				presetSpell_ = 5;
				SpellManager::GetInstance()->ResetChargeTime();
			}

		}
	}

	frontVec_.x = float(sin(Radian(cameraAngle_.x)));
	frontVec_.y = float(sin(Radian(cameraAngle_.y)));
	frontVec_.z = float(cos(Radian(cameraAngle_.x)));
	rightVec_.x = float(sin(Radian(cameraAngle_.x + RAD / 2)));
	rightVec_.y = float(sin(Radian(cameraAngle_.y)));
	rightVec_.z = float(cos(Radian(cameraAngle_.x + RAD / 2)));

	//�J��������
	camera->SetEye(pos_ + CAMERA_EYE);//�ڐ��ɃJ�����𒲐�
	camera->SetTarget(pos_ + frontVec_ + CAMERA_EYE);//�ڐ��ɃJ�����𒲐�
}

void Player::Attack()
{
	//�C���X�^���X�擾
	Input* input = Input::GetInstance();
	PlayerWeapon* weapon = PlayerWeapon::GetInstance();
	SpellManager* spellM = SpellManager::GetInstance();

	bool isAttackOn = false;

	if ((input->TriggerKey(DIK_SPACE) || input->TriggerButton(XINPUT_GAMEPAD_RIGHT_SHOULDER)) && GetIsCanAction()) {
		//�U���t���O�𗧂Ă�
		isAttack_ = true;
		attackTime_ = TIME_ATTACK_NORMAL;
	}
	//�����r��
	if ((input->PushKey(DIK_E) || input->ReleaseKey(DIK_E) || input->PushButton(XINPUT_GAMEPAD_LEFT_SHOULDER) || input->ReleaseButton(XINPUT_GAMEPAD_LEFT_SHOULDER)) && !spellM->GetIsUseSpell()) {
		switch (LoadOut::GetInstance()->GetSpell(presetSpell_))
		{
		case FIRE_BALL:
			spellM->ChargeFireBall();
			break;
		case MAGIC_MISSILE:
			spellM->ChargeMagicMissile();
			break;
		case ICE_BOLT:
			spellM->ChargeIceBolt();
			break;
		case CHAIN_LIGHTNING:
			spellM->ChargeChainLightning();
			break;
		case ENCHANT_FIRE:
			spellM->ChargeEnchantFire();
			break;
		case FLAME:
			spellM->ChargeFlame();
		default:
			break;
		}

		isSpell_ = true;
	}
	else {
		isSpell_ = false;
	}

	//�U������`�F�b�N
	if (attackTime_ < TIME_ATTACK_NORMAL - TIME_ATTACK_START_NORMAL && attackTime_ > TIME_ATTACK_NORMAL - TIME_ATTACK_END_NORMAL) {
		isAttackOn = true;
	}
	//�U���I��
	if (attackTime_ <= 0) {
		//�U���t���O������
		isAttack_ = false;
		//�G�̑��i�q�b�g����t���O������
		EnemyManager::GetInstance()->ResetIsHit();
	}
	else {
		//���Ԃ����炷
		attackTime_--;
	}

	weapon->Update(isAttack_, isAttackOn);

	//�������Ă��Ȃ����̂�
	if (GetIsCanAction()) {
		ItemManager::GetInstance()->Use();
	}
	//���ł�
	ItemManager::GetInstance()->Update();
	//ItemManager::GetInstance()->ChangeItem();
}

void Player::HealLife()
{
	if (healingLife_ > 0) {
		healingLife_--;
		if (life_ < MAX_LIFE) {
			life_++;
		}
	}
}

void Player::WorldTransUpdate()
{
	worldTrans_.SetPos(pos_ + CAMERA_EYE);
	worldTrans_.SetRot(rot_);
	worldTrans_.SetScale(scale_);
}

float Radian(float r) {
	return r * (PI / RAD);
}