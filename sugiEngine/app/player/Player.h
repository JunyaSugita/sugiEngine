#pragma once
#include "SugiMath.h"
#include "WorldTransform.h"
#include "SpellManager.h"
#include "Sprite.h"

class Player final{
private:
	Player() = default;
	~Player() = default;
public:
	//�R�s�[�R���X�g���N�^����
	Player(const Player& obj) = delete;
	//������Z�q�𖳌�
	Player& operator=(const Player& obj) = delete;

	static Player* GetInstance();

public:
	void Initialize();
	void GameInitialize();
	void Update();
	void Draw();
	void SpDraw();

	//pos
	Vector3 GetPos() {
		return pos_;
	}
	void SetPos(const Vector3& pos) {
		pos_ = pos;
	}
	void SetPosX(float pos) {
		pos_.x = pos;
	}
	void SetPosY(float pos) {
		pos_.y = pos;
	}
	void SetPosZ(float pos) {
		pos_.z = pos;
	}

	//life
	uint32_t GetLife() {
		return life_;
	}
	void SubLife();

	//worldTrans
	WorldTransform& GetWorldTrans() {
		return worldTrans_;
	}

	//cameraAngle
	Vector2 GetCameraAngle() {
		return cameraAngle_;
	}

	//time 
	float GetTime() {
		return attackTime_;
	};

	bool GetIsCanAction();
	bool GetIsSpell() {
		return isSpell_;
	}

	void SetIsSpell(bool is) {
		isSpell_ = is;
	}

	void SetPresetSpell(uint32_t spellName) {
		presetSpell_ = spellName;
	}
	uint32_t GetPresetSpell(){
		return presetSpell_;
	}

	BoxCol GetBoxCol() {
		return boxCol_;
	}

	BoxCol GetOldBoxCol() {
		return oldBoxCol_;
	}

	void WorldTransUpdate();

	//��
	void AddHealNum(int32_t num) {
		healingLife_ += num;
	}
	int32_t GetHeal() {
		return healingLife_;
	}

	//���G
	void SetInvincible() {
		isInvincible_ = (isInvincible_ + 1) % 2;
	}

private:
	void Move();
	void CameraMove();
	void Attack();
	void HealLife();

public:
	const Vector3 CAMERA_EYE = { 0.0f,5.0f,0.0f };//�v���C���[�̖ڐ�����
	const float SPEED_MOVE = 0.5f;	//�v���C���[�̃X�s�[�h
	const float SPEED_SLOW = 0.5f;	//�v���C���[��slow��Ԃł̃X�s�[�h�̔{��
	const float SPEED_CAMERA = 3.0f;	//�J�����̃X�s�[�h
	const float TIME_ATTACK_START_NORMAL = 0.3f * 60.0f;//�ʏ�U���J�n����U�����肪�o��܂ł̎���
	const float TIME_ATTACK_END_NORMAL = 0.75f * 60.0f;//�ʏ�U���J�n����U�����肪�����Ȃ�܂ł̎���
	const float TIME_ATTACK_NORMAL = 0.8f * 60.0f;	//�ʏ�U���J�n����I���܂ł̃X�s�[�h
	const float TIME_NAVE = 1.0f * 60;
	const int32_t MAX_LIFE = 10000;
	const float MAX_Y = 3.0f;

private:
	WorldTransform worldTrans_;
	Vector3 pos_;
	Vector3 rot_;
	Vector3 scale_;
	Vector2 cameraAngle_;//�J�����p�x
	Vector3 frontVec_;//���ʃx�N�g��
	Vector3 rightVec_;//�E�x�N�g��

	int32_t life_;//�̗�
	int32_t healingLife_;	//�|�[�V�����̉�
	int32_t exLife_;	//�����̉�

	bool isAttack_;//�U���t���O
	bool isSpell_;//���@�t���O
	float attackTime_;//�U������

	//��������
	uint32_t presetSpell_;
	float spellAngle_;

	//�����蔻��
	BoxCol boxCol_;
	BoxCol oldBoxCol_;

	//�_���[�W���o
	int32_t damageTex_;
	Sprite damageSp_;
	float damageAlpha_;

	//���G
	bool isInvincible_;

	//navePoint�̃^�C�~���O
	float naveTimer_;
};

float Radian(float r);
