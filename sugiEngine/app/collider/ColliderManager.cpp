#include "ColliderManager.h"
#include "SpellManager.h"
#include "EnemyManager.h"
#include "ParticleManager.h"
#include "Player.h"
#include "FieldManager.h"

using namespace std;

ColliderManager::ColliderManager()
{
}

ColliderManager::~ColliderManager()
{
}

ColliderManager* ColliderManager::GetInstance()
{
	static ColliderManager instance;

	return &instance;
}

void ColliderManager::Initialize()
{
	isShowHitBox_ = false;
}

void ColliderManager::Update()
{
	//�G�̔���
	vector<Enemy*> enemysCol = EnemyManager::GetInstance()->GetEnemysList();
	//�}�b�v�̔���
	FieldManager* field = FieldManager::GetInstance();
	//�v���C���[
	Player* player = Player::GetInstance();

#pragma region �t�@�C���[�{�[���̔���
	//�t�@�C�A�[�{�[��
	vector<FireBall*> fireBallsCol = SpellManager::GetInstance()->GetFireBallsCol();

	for (int i = 0; i < fireBallsCol.size(); i++) {
		for (int j = 0; j < enemysCol.size(); j++) {
			if (CheckHitBox(fireBallsCol[i]->GetBoxCol(), enemysCol[j]->GetBoxCol())) {
				fireBallsCol[i]->SetIsHit();
				enemysCol[j]->SetIsHit(1, 1);
				enemysCol[j]->SetDebuff(FIRE, 10);
			}
		}
		for (int j = 0; j < field->GetInstance()->GetColSize(); j++) {
			if (CheckHitBox(fireBallsCol[i]->GetBoxCol(), field->GetInstance()->GetCol(j))) {
				fireBallsCol[i]->SetIsHit();
			}
		}
	}

#pragma endregion

#pragma region �}�W�b�N�~�T�C���̔���
	//�}�W�b�N�~�T�C��
	vector<MagicMissile*> magicMissilesCol = SpellManager::GetInstance()->GetMagicMissilesCol();

	for (int i = 0; i < magicMissilesCol.size(); i++) {
		for (int j = 0; j < enemysCol.size(); j++) {
			if (CheckHitBox(magicMissilesCol[i]->GetBoxCol(), enemysCol[j]->GetBoxCol())) {
				magicMissilesCol[i]->SetIsHit();
				enemysCol[j]->SetIsHit(5, 5);
				enemysCol[j]->SetDebuff(THUNDER, 5);
			}
		}
		for (int j = 0; j < field->GetInstance()->GetColSize(); j++) {
			if (CheckHitBox(magicMissilesCol[i]->GetBoxCol(), field->GetInstance()->GetCol(j))) {
				magicMissilesCol[i]->SetIsHit();
			}
		}
	}

#pragma endregion

#pragma region �A�C�X�{���g�̔���
	//�A�C�X�{���g
	vector<IceBolt*> iceBoltsCol = SpellManager::GetInstance()->GetIceBoltsCol();

	for (int i = 0; i < iceBoltsCol.size(); i++) {
		for (int j = 0; j < enemysCol.size(); j++) {
			if (CheckHitBox(iceBoltsCol[i]->GetBoxCol(), enemysCol[j]->GetBoxCol())) {
				iceBoltsCol[i]->SetIsHit();
				enemysCol[j]->SetIsHit(10, 5);
				enemysCol[j]->SetDebuff(ICE, 8);
			}
		}
		for (int j = 0; j < field->GetInstance()->GetColSize(); j++) {
			if (CheckHitBox(iceBoltsCol[i]->GetBoxCol(), field->GetInstance()->GetCol(j))) {
				iceBoltsCol[i]->SetIsHit();
			}
		}
	}

#pragma endregion

#pragma region �`�F�C�����C�g�j���O�̔���
	//�`�F�C�����C�g�j���O
	vector<ChainLightning*> chainLightningsCol = SpellManager::GetInstance()->GetChainLightningsCol();


	for (int i = 0; i < chainLightningsCol.size(); i++) {
		for (int j = 0; j < enemysCol.size(); j++) {
			if (CheckHitBox(enemysCol[j]->GetBoxCol(), chainLightningsCol[i]->GetBoxCol())) {
				chainLightningsCol[i]->SetIsHit();
				enemysCol[j]->SetIsHit(15, 5);
				enemysCol[j]->SetDebuff(THUNDER, 1);

				//1�̖ڂ̓`�d
				int32_t hitTemp1 = -1;
				float lenTemp = 30;
				for (int k = 0; k < enemysCol.size(); k++) {

					if (j != k) {
						float length = (enemysCol[j]->GetBoxCol().pos - enemysCol[k]->GetBoxCol().pos).length();
						if (lenTemp > length) {
							lenTemp = length;
							hitTemp1 = k;
						}
					}
				}

				//�߂����������
				if (hitTemp1 != -1) {
					//��ԋ߂���Ƀ_���[�W
					enemysCol[hitTemp1]->SetIsHit(15, 5);
					enemysCol[hitTemp1]->SetDebuff(THUNDER, 1);
					//�����܂ł̃p�[�e�B�N��
					Vector3 tempVec = enemysCol[hitTemp1]->GetBoxCol().pos - enemysCol[j]->GetBoxCol().pos;
					Vector3 nowPos = enemysCol[j]->GetBoxCol().pos;
					for (int l = 0; l < 20; l++) {
						nowPos += tempVec / 20;
						ParticleManager::GetInstance()->AddFromFile(P_LIGHTNING, nowPos);
					}

					//2�̖ڂ̓`�d
					int32_t hitTemp2 = -1;
					lenTemp = 30;
					for (int k = 0; k < enemysCol.size(); k++) {

						if (j != k && hitTemp1 != k) {
							float length = (enemysCol[hitTemp1]->GetBoxCol().pos - enemysCol[k]->GetBoxCol().pos).length();
							if (lenTemp > length) {
								lenTemp = length;
								hitTemp2 = k;
							}
						}
					}
					//�߂����������
					if (hitTemp2 != -1) {
						//��ԋ߂���Ƀ_���[�W
						enemysCol[hitTemp2]->SetIsHit(15, 5);
						enemysCol[hitTemp2]->SetDebuff(THUNDER, 1);

						//�����܂ł̃p�[�e�B�N��
						Vector3 tempVec = enemysCol[hitTemp2]->GetBoxCol().pos - enemysCol[hitTemp1]->GetBoxCol().pos;
						Vector3 nowPos = enemysCol[hitTemp1]->GetBoxCol().pos;
						for (int l = 0; l < 20; l++) {
							nowPos += tempVec / 20;
							ParticleManager::GetInstance()->AddFromFile(P_LIGHTNING, nowPos);
						}
					}
				}
			}
		}
	}

#pragma endregion

#pragma region �v���C���[�ƓG�̔���

	for (int i = 0; i < enemysCol.size(); i++) {
		if (CheckHitBox(enemysCol[i]->GetBoxCol(), player->GetBoxCol())) {
			enemysCol[i]->SetIsStop();
			enemysCol[i]->SetIsAttack();
		}
	}

#pragma endregion 

#pragma region �G���m�̓����蔻��
	for (int i = 0; i < enemysCol.size(); i++) {
		for (int j = i + 1; j < enemysCol.size(); j++) {
			if (CheckHitBox(enemysCol[i]->GetBoxCol(), enemysCol[j]->GetBoxCol())) {
				if (enemysCol[i]->GetBoxCol().pos.x <= enemysCol[j]->GetBoxCol().pos.x) {
					enemysCol[i]->AddColX(-0.01f);
					enemysCol[j]->AddColX(0.01f);
				}
				else if (enemysCol[i]->GetBoxCol().pos.x > enemysCol[j]->GetBoxCol().pos.x) {
					enemysCol[i]->AddColX(0.01f);
					enemysCol[j]->AddColX(-0.01f);
				}

				if (enemysCol[i]->GetBoxCol().pos.z <= enemysCol[j]->GetBoxCol().pos.z) {
					enemysCol[i]->AddColZ(-0.01f);
					enemysCol[j]->AddColZ(0.01f);
				}
				else if (enemysCol[i]->GetBoxCol().pos.z > enemysCol[j]->GetBoxCol().pos.z) {
					enemysCol[i]->AddColZ(0.01f);
					enemysCol[j]->AddColZ(-0.01f);
				}
			}
		}
	}

#pragma endregion

#pragma region �G�ƕǂ̔���
	for (int i = 0; i < enemysCol.size(); i++) {
		for (int j = 0; j < field->GetInstance()->GetColSize(); j++) {
			if (CheckHitBox(enemysCol[i]->GetBoxCol(), field->GetInstance()->GetCol(j))) {
				if (CheckHitX(enemysCol[i]->GetBoxCol(), field->GetInstance()->GetCol(j))) {
					if (!CheckHitX(enemysCol[i]->GetOldBoxCol(), field->GetInstance()->GetCol(j))) {
						enemysCol[i]->SetColX(enemysCol[i]->GetOldBoxCol().pos.x);
					}
				}
				if (CheckHitY(enemysCol[i]->GetBoxCol(), field->GetInstance()->GetCol(j))) {
					if (!CheckHitY(enemysCol[i]->GetOldBoxCol(), field->GetInstance()->GetCol(j))) {
						enemysCol[i]->SetColY(enemysCol[i]->GetOldBoxCol().pos.y);
					}
				}
				if (CheckHitZ(enemysCol[i]->GetBoxCol(), field->GetInstance()->GetCol(j))) {
					if (!CheckHitZ(enemysCol[i]->GetOldBoxCol(), field->GetInstance()->GetCol(j))) {
						enemysCol[i]->SetColZ(enemysCol[i]->GetOldBoxCol().pos.z);
					}
				}
				enemysCol[i]->ResetShake();
				enemysCol[i]->WorldTransUpdate();
			}
		}
	}
#pragma endregion

#pragma region �v���C���[�ƕǂ̔���
	for (int i = 0; i < field->GetInstance()->GetColSize(); i++) {
		if (CheckHitBox(player->GetBoxCol(), field->GetInstance()->GetCol(i))) {
			if (CheckHitX(player->GetBoxCol(), field->GetInstance()->GetCol(i))) {
				if (!CheckHitX(player->GetOldBoxCol(), field->GetInstance()->GetCol(i))) {
					player->SetPosX(player->GetOldBoxCol().pos.x);
				}
			}
			if (CheckHitY(player->GetBoxCol(), field->GetInstance()->GetCol(i))) {
				if (!CheckHitY(player->GetOldBoxCol(), field->GetInstance()->GetCol(i))) {
					player->SetPosY(player->GetOldBoxCol().pos.y);
				}
			}
			if (CheckHitZ(player->GetBoxCol(), field->GetInstance()->GetCol(i))) {
				if (!CheckHitZ(player->GetOldBoxCol(), field->GetInstance()->GetCol(i))) {
					player->SetPosZ(player->GetOldBoxCol().pos.z);
				}
			}
			player->WorldTransUpdate();
		}
	}
#pragma endregion
}

bool ColliderManager::CheckWayX(BoxCol a, BoxCol b)
{
	if (a.pos.x >= b.pos.x) {
		return true;
	}
	return false;
}

bool ColliderManager::CheckWayY(BoxCol a, BoxCol b)
{
	if (a.pos.y >= b.pos.y) {
		return true;
	}
	return false;
}

bool ColliderManager::CheckWayZ(BoxCol a, BoxCol b)
{
	if (a.pos.z >= b.pos.z) {
		return true;
	}
	return false;
}


bool ColliderManager::CheckHitX(BoxCol a, BoxCol b)
{
	if (a.pos.x + a.size.x >= b.pos.x - b.size.x && b.pos.x + b.size.x >= a.pos.x - a.size.x) {
		return true;
	}
	return false;
}

bool ColliderManager::CheckHitY(BoxCol a, BoxCol b)
{
	if (a.pos.y + a.size.y >= b.pos.y - b.size.y && b.pos.y + b.size.y >= a.pos.y - a.size.y) {
		return true;
	}
	return false;
}

bool ColliderManager::CheckHitZ(BoxCol a, BoxCol b)
{
	if (a.pos.z + a.size.z >= b.pos.z - b.size.z && b.pos.z + b.size.z >= a.pos.z - a.size.z) {
		return true;
	}
	return false;
}


bool ColliderManager::CheckHitBox(BoxCol a, BoxCol b)
{
	if (CheckHitX(a, b)) {
		if (CheckHitY(a, b)) {
			if (CheckHitZ(a, b)) {
				return true;
			}
		}
	}

	return false;
}

bool ColliderManager::CheckHitEnemyToChainLightning()
{
	//�G�̔���
	vector<Enemy*> enemysCol = EnemyManager::GetInstance()->GetEnemysList();
	FieldManager* field = FieldManager::GetInstance();

	//�`�F�C�����C�g�j���O
	vector<ChainLightning*> chainLightningsCol = SpellManager::GetInstance()->GetChainLightningsCol();


	for (int i = 0; i < chainLightningsCol.size(); i++) {
		for (int j = 0; j < enemysCol.size(); j++) {
			if (CheckHitBox(enemysCol[j]->GetBoxCol(), chainLightningsCol[i]->GetBoxCol())) {
				return true;
			}
		}
		for (int j = 0; j < field->GetInstance()->GetColSize(); j++) {
			if (CheckHitBox(chainLightningsCol[i]->GetBoxCol(), field->GetInstance()->GetCol(j))) {
				return true;
			}
		}

	}

	return false;
}

