#include "BaseEnemy.h"
#include "SpellManager.h"
#include "Player.h"
#include "EffectManager.h"
#include "ParticleManager.h"
#include "Tutorial.h"
#include "Enemy.h"
#include "ModelManager.h"

#include <random>

bool BaseEnemy::sIsDebugStop_ = true;

void BaseEnemy::Initialize(std::string name, Vector3 pos)
{
	//���f���f�[�^�͐�ɔh���N���X�œǂݍ���
	obj_.Initialize(name);
	col_.Initialize();

	//�ʒu
	obj_.pos = pos;
	obj_.rot = { 0,90,0 };
	obj_.scale = { 1,1,1 };

	//�����蔻��
	col_.col.pos = pos;
	col_.col.size = { 1.0f,height_,1.0f };
	col_.gap = {0,height_,0};

	//�t���O
	isDead_ = false;
	isHit_ = false;
	isStop_ = false;
	isAttack_ = false;
	attackTimer_ = 0.0f;

	//�`���[�g���A��
	if (Tutorial::GetInstance()->GetIsTutorial()) {
		sIsDebugStop_ = true;
	}
	else {
		sIsDebugStop_ = false;
	}
}

void BaseEnemy::Update()
{
	//1�t���O�̍��W��ۑ�
	col_.SetOldCol();

	//�V�F�C�N��߂�
	ResetShake();

	//�f�o�t�̓K��
	UpdateDebuff();

	//�����邩�ǂ���
	if (isCanMove()) {
		if (!isAttack_ && !sIsDebugStop_) {
			//�ړ�
			Move();
		}

		//�U��
		Attack();

		//�v���C���[�̕��ւ�������]
		SetAngleToPlayer();

		//�����蔻��ړ�
		SetCol();
	}
	else
	{
		SetShake();
	}

	//�ړ���K��
	WorldTransUpdate();
}

void BaseEnemy::Draw()
{
	obj_.Draw();
	if (ColliderManager::GetInstance()->GetIsShowHitBox()) {
		col_.Draw();
	}
}

void BaseEnemy::WorldTransUpdate()
{
	obj_.Update();
	col_.Update();
}

void BaseEnemy::SetDebuff(uint8_t debuff, uint32_t time)
{
	switch (debuff)
	{
	case FIRE:
		debuff_.isFire = true;
		debuff_.fireTime = time * 60;
		break;
	case THUNDER:
		debuff_.isThunder = true;
		debuff_.thunderTime = time * 60;
		break;
	case ICE:
		debuff_.isIce = true;
		debuff_.iceTime = time * 60;
		break;
	default:
		break;
	}
}

void BaseEnemy::SetIsHit(int32_t subLife, int32_t effectNum)
{
	//���ɓ������Ă����瓖����Ȃ�
	if (isHit_) {
		return;
	}
	else {
		//���������t���O�𗧂Ă�
		isHit_ = true;
		//�̗͂����
		SubLife(subLife, effectNum);
	}
}

bool BaseEnemy::isDebuff()
{
	if (debuff_.isFire || debuff_.isThunder || debuff_.isIce) {
		return true;
	}

	return false;
}

bool BaseEnemy::isCanMove()
{
	if (debuff_.isThunder) {
		return false;
	}

	return true;
}

void BaseEnemy::ResetShake()
{
	obj_.pos.x = col_.col.pos.x;
	obj_.pos.z = col_.col.pos.z;
}

void BaseEnemy::SetIsAttack()
{
	if (!isAttack_) {
		attackTimer_ = 1.0f;
		Player::GetInstance()->SubLife();
	}
	isAttack_ = true;
}

void BaseEnemy::SetAngleToPlayer()
{
	//�C���X�^���X�擾
	Player* player = Player::GetInstance()->GetInstance();

	//�v���C���[�̌������v�Z���ĒP�ʉ�
	Vector2 len = Vector2((player->GetPos() - obj_.pos).x, (player->GetPos() - obj_.pos).z);
	len.normalize();

	//�v���C���[�̕��ɂ���������
	float angle_ = atan2(len.cross(UP), -len.dot(UP)) / PI * -RADIAN - (RADIAN / 2);
	while (obj_.rot.y - angle_ > RADIAN || obj_.rot.y - angle_ < -RADIAN) {
		if (obj_.rot.y - angle_ > RADIAN) {
			obj_.rot.y -= RADIAN * 2;
		}
		if (obj_.rot.y - angle_ < -RADIAN) {
			obj_.rot.y += RADIAN * 2;
		}
	}
	if (obj_.rot.y - angle_ < 0) {
		obj_.rot.y += angleSpeed_ * GetSlow();
		if (obj_.rot.y - angle_ > 0) {
			obj_.rot.y = angle_;
		}
	}
	else {
		obj_.rot.y -= angleSpeed_ * GetSlow();
		if (obj_.rot.y - angle_ < 0) {
			obj_.rot.y = angle_;
		}
	}
}

float BaseEnemy::GetSlow()
{
	if (debuff_.isIce) {
		return 0.5;
	}
	return 1.0f;
}

void BaseEnemy::SubLife(int32_t subLife, int32_t effectNum)
{
	life_ -= subLife;
	EffectManager::GetInstance()->BurstGenerate({ obj_.pos.x,obj_.pos.y + 4,obj_.pos.z }, effectNum, { 1,0,0,1 });
	if (life_ <= 0) {
		isDead_ = true;
	}
}

void BaseEnemy::UpdateDebuff()
{
	if (isDebuff()) {
		if (debuff_.isFire) {
			ParticleManager::GetInstance()->AddFromFile(P_DEBUFF_FIRE, col_.col.pos);
			if (debuff_.fireTime % 40 == 1) {
				SubLife(1, 0);
			}


			if (--debuff_.fireTime < 0) {
				debuff_.isFire = false;
			}
		}
		if (debuff_.isThunder) {
			if (--debuff_.thunderTime < 0) {
				debuff_.isThunder = false;
			}
		}
		if (debuff_.isIce) {
			if (--debuff_.iceTime < 0) {
				debuff_.isIce = false;
			}
		}
	}
}

void BaseEnemy::SetShake()
{
	//�����_��
	std::random_device seed_gen;
	std::mt19937_64 engine(seed_gen());

	//�J�����Ƃ̋����ŃV�F�C�N�̑傫����ς���
	Vector3 len = Camera::GetInstance()->GetEye() - obj_.pos;
	float maxShake = len.length() / 100;
	if (maxShake > 0.5f) {
		maxShake = 0.5f;
	}

	std::uniform_real_distribution<float> x(-maxShake, maxShake);
	std::uniform_real_distribution<float> z(-maxShake, maxShake);

	obj_.pos.x += x(engine);
	obj_.pos.z += z(engine);
}

void BaseEnemy::SetCol()
{
	col_.SetCol(obj_.pos);
}
