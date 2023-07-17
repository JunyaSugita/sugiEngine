#include "ColliderManager.h"
#include "SpellManager.h"
#include "EnemyManager.h"
#include "ParticleManager.h"

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

#pragma region �t�@�C���[�{�[���̔���
	//�t�@�C�A�[�{�[��
	vector<FireBall*> fireBallsCol = SpellManager::GetInstance()->GetFireBallsCol();

	for (int i = 0; i < enemysCol.size(); i++) {
		for (int j = 0; j < fireBallsCol.size(); j++) {
			if (CheckHitBox(enemysCol[i]->GetBoxCol(), fireBallsCol[j]->GetBoxCol())) {
				enemysCol[i]->SetIsHit(1,1);
				enemysCol[i]->SetDebuff(FIRE,10);
				fireBallsCol[j]->SetIsHit();
			}
		}
	}

#pragma endregion

#pragma region �}�W�b�N�~�T�C���̔���
	//�}�W�b�N�~�T�C��
	vector<MagicMissile*> magicMissilesCol = SpellManager::GetInstance()->GetMagicMissilesCol();

	for (int i = 0; i < enemysCol.size(); i++) {
		for (int j = 0; j < magicMissilesCol.size(); j++) {
			if (CheckHitBox(enemysCol[i]->GetBoxCol(), magicMissilesCol[j]->GetBoxCol())) {
				enemysCol[i]->SetIsHit(5, 5);
				enemysCol[i]->SetDebuff(THUNDER,5);
				magicMissilesCol[j]->SetIsHit();
			}
		}
	}

#pragma endregion

#pragma region �A�C�X�{���g�̔���
	//�A�C�X�{���g
	vector<IceBolt*> iceBoltsCol = SpellManager::GetInstance()->GetIceBoltsCol();

	for (int i = 0; i < enemysCol.size(); i++) {
		for (int j = 0; j < iceBoltsCol.size(); j++) {
			if (CheckHitBox(enemysCol[i]->GetBoxCol(), iceBoltsCol[j]->GetBoxCol())) {
				enemysCol[i]->SetIsHit(10, 5);
				enemysCol[i]->SetDebuff(ICE, 8);
				iceBoltsCol[j]->SetIsHit();
			}
		}
	}

#pragma endregion

#pragma region �`�F�C�����C�g�j���O�̔���
	//�`�F�C�����C�g�j���O
	vector<ChainLightning*> chainLightningsCol = SpellManager::GetInstance()->GetChainLightningsCol();

	for (int i = 0; i < enemysCol.size(); i++) {
		for (int j = 0; j < chainLightningsCol.size(); j++) {
			if (CheckHitBox(enemysCol[i]->GetBoxCol(), chainLightningsCol[j]->GetBoxCol())) {
				enemysCol[i]->SetIsHit(15, 5);
				enemysCol[i]->SetDebuff(THUNDER, 1);
				chainLightningsCol[j]->SetIsHit();

				//1�̖ڂ̓`�d
				int32_t hitTemp1 = -1;
				float lenTemp = 30;
				for (int k = 0; k < enemysCol.size(); k++) {
					
					if (i != k) {
						float length = (enemysCol[i]->GetBoxCol().pos - enemysCol[k]->GetBoxCol().pos).length();
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
					Vector3 tempVec = enemysCol[hitTemp1]->GetBoxCol().pos - enemysCol[i]->GetBoxCol().pos;
					Vector3 nowPos = enemysCol[i]->GetBoxCol().pos;
					for (int l = 0; l < 20;l++) {
						nowPos += tempVec / 20;
						ParticleManager::GetInstance()->AddFromFile(P_LIGHTNING,nowPos);
					}

					//2�̖ڂ̓`�d
					int32_t hitTemp2 = -1;
					lenTemp = 30;
					for (int k = 0; k < enemysCol.size(); k++) {

						if (i != k && hitTemp1 != k) {
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
}

bool ColliderManager::CheckHitBox(BoxCol a, BoxCol b)
{
	if (a.pos.x + a.width >= b.pos.x - b.width && b.pos.x + b.width >= a.pos.x - a.width) {
		if (a.pos.z + a.width >= b.pos.z - b.width && b.pos.z + b.width >= a.pos.z - a.width) {
			if (a.pos.y + a.height >= b.pos.y - b.height && b.pos.y + b.height >= a.pos.y - a.height) {
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

	//�`�F�C�����C�g�j���O
	vector<ChainLightning*> chainLightningsCol = SpellManager::GetInstance()->GetChainLightningsCol();

	for (int i = 0; i < enemysCol.size(); i++) {
		for (int j = 0; j < chainLightningsCol.size(); j++) {
			if (CheckHitBox(enemysCol[i]->GetBoxCol(), chainLightningsCol[j]->GetBoxCol())) {
				return true;
			}
		}
	}
	return false;
}
