#pragma once
#include "GrovalSetting.h"
#include "WorldTransform.h"
#include "Model.h"
#include "Object3d.h"
#include "ColliderManager.h"

struct DebuffM {
	bool isFire;
	int32_t fireTime;
	bool isThunder;
	int32_t thunderTime;
	bool isIce;
	int32_t iceTime;
};

class BaseEnemy {
public:
	static void StaticInitialize();
	virtual void Initialize(Vector3 pos);
	virtual void Update();
	virtual void Draw();
	virtual void WorldTransUpdate();

#pragma region inline�Q
	//inline�Q
	Vector3 GetPos() {
		return pos_;
	}
	void SetCol(Vector3 vec) {
		boxCol_.pos = vec;
	}
	void SetColX(float x) {
		boxCol_.pos.x = x;
	}
	void SetColY(float y) {
		boxCol_.pos.y = y;
	}
	void SetColZ(float z) {
		boxCol_.pos.z = z;
	}
	void AddCol(Vector3 vec) {
		boxCol_.pos += vec;
	}
	void AddColX(float x) {
		boxCol_.pos.x += x;
	}
	void AddColZ(float z) {
		boxCol_.pos.z += z;
	}
	bool GetIsDead() {
		return isDead_;
	}
	void SetIsDead() {
		isDead_ = true;
	}
	void ResetIsHit() {
		isHit_ = false;
	}
	BoxCol GetBoxCol() {
		return boxCol_;
	}
	BoxCol GetOldBoxCol() {
		return oldBoxCol_;
	}
	void SetIsStop() {
		isStop_ = true;
	}
	uint32_t GetLife() {
		return life_;
	}

	//�f�o�b�O�p
	
	// �G�̓������~�߂��蓮�������萧�䂷��
	static void SetIsDebugStop() {
		sIsDebugStop_ = (sIsDebugStop_ + 1) % 2;
	}

#pragma endregion

	/// <summary>
	/// �f�o�t��t�^
	/// </summary>
	/// <param name="debuff">�f�o�t�̎�ޔԍ�</param>
	/// <param name="time">�f�o�t��K�����鎞��/f</param>
	void SetDebuff(uint8_t debuff, uint32_t time);

	/// <summary>
	/// HP�����炵�A�G�t�F�N�g���o��
	/// </summary>
	/// <param name="subLife">hp������</param>
	/// <param name="effectNum">�G�t�F�N�g���o����</param>
	void SetIsHit(int32_t subLife = 1, int32_t effectNum = 0);

	/// <summary>
	/// �f�o�t��ԂɂȂ��Ă��邩
	/// </summary>
	/// <returns>�f�o�t��ԂɂȂ��Ă��邩</returns>
	bool isDebuff();

	/// <summary>
	/// �������Ԃ�
	/// </summary>
	/// <returns>�������Ԃ�</returns>
	bool isCanMove();

	// �V�F�C�N�ňړ����Ă�������߂�
	void ResetShake();

	// �U�����ăv���C���[�Ƀ_���[�W��^����
	void SetIsAttack();

protected:
	virtual void SetWorldTrans();
	virtual void Move() = 0;
	virtual void Attack() = 0;

	// �v���C���[�̕���������
	void SetAngleToPlayer();

	/// <summary>
	/// �ړ����x�{��
	/// </summary>
	/// <returns>�{��</returns>
	float GetSlow();

	/// <summary>
	/// hp�����炵�A�G�t�F�N�g���o��
	/// </summary>
	/// <param name="subLife">���炷hp��</param>
	/// <param name="effectNum">�o���G�t�F�N�g��</param>
	void SubLife(int32_t subLife, int32_t effectNum);

	// �f�o�t�̌��ʂ��X�V
	void UpdateDebuff();

	//���g���V�F�C�N������
	void SetShake();
	
	//���g�̃R���W������ݒ�
	void SetCol();

protected:
	const Vector2 UP = { 0,-1 };
	const float RADIAN = 180;

	static std::unique_ptr<Model> sColModel_;
	static std::unique_ptr<Model> sModel_;

	//�{��
	WorldTransform worldTrans_;
	Vector3 pos_;
	Vector3 rot_;
	Vector3 scale_;

	//�����蔻��
	BoxCol boxCol_;
	BoxCol oldBoxCol_;
	float height_;

	//�̗�
	int32_t life_;

	//���񂾂��ǂ���
	bool isDead_;

	//�v���C���[�Ƃ̋������L�^
	Vector2 toPlayer;

	//���i�q�b�g�̉���t���O
	bool isHit_;

	//�f�o�t
	DebuffM debuff_;

	//�{�̂̃��f���֌W
	std::unique_ptr<Object3d> obj_;

	
	//�����蔻��
	WorldTransform colWorldTrans_;
	//�����蔻��̃��f���֌W
	std::unique_ptr<Object3d> colObj_;

	//�����~�߂邩
	bool isStop_;

	//�U��
	float attackTimer_;
	bool isAttack_;

	//��]���x
	float angleSpeed_;

	//�f�o�b�O
	static bool sIsDebugStop_;
};