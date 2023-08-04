#include "ColliderManager.h"
#include "SpellManager.h"
#include "EnemyManager.h"
#include "ParticleManager.h"
#include "Player.h"
#include "FieldManager.h"
#include "NavePointManager.h"

using namespace std;

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
	//敵の判定
	vector<BaseEnemy*> enemysCol = EnemyManager::GetInstance()->GetEnemysList();
	//マップの判定
	FieldManager* field = FieldManager::GetInstance();
	//プレイヤー
	Player* player = Player::GetInstance();

#pragma region ファイヤーボールの判定
	//ファイアーボール
	vector<FireBall*> fireBallsCol = SpellManager::GetInstance()->GetFireBallsCol();

	for (int i = 0; i < fireBallsCol.size(); i++) {
		for (int j = 0; j < enemysCol.size(); j++) {
			if (CheckHitBox(fireBallsCol[i]->GetBoxCol(), enemysCol[j]->GetBoxCol())) {
				fireBallsCol[i]->SetIsHit();
				enemysCol[j]->SetIsHit(1, 1);
				enemysCol[j]->SetDebuff(FIRE, 10);
			}
		}
		for (int j = 0; j < field->GetColSize(); j++) {
			if (CheckHitBox(fireBallsCol[i]->GetBoxCol(), field->GetCol(j))) {
				fireBallsCol[i]->SetIsHit();
			}
		}
	}

#pragma endregion

#pragma region マジックミサイルの判定
	//マジックミサイル
	vector<MagicMissile*> magicMissilesCol = SpellManager::GetInstance()->GetMagicMissilesCol();

	for (int i = 0; i < magicMissilesCol.size(); i++) {
		for (int j = 0; j < enemysCol.size(); j++) {
			if (CheckHitBox(magicMissilesCol[i]->GetBoxCol(), enemysCol[j]->GetBoxCol())) {
				magicMissilesCol[i]->SetIsHit();
				enemysCol[j]->SetIsHit(8, 5);
			}
		}
		for (int j = 0; j < field->GetColSize(); j++) {
			if (CheckHitBox(magicMissilesCol[i]->GetBoxCol(), field->GetCol(j))) {
				magicMissilesCol[i]->SetIsHit();
			}
		}
	}

#pragma endregion

#pragma region アイスボルトの判定
	//アイスボルト
	vector<IceBolt*> iceBoltsCol = SpellManager::GetInstance()->GetIceBoltsCol();

	for (int i = 0; i < iceBoltsCol.size(); i++) {
		for (int j = 0; j < enemysCol.size(); j++) {
			if (CheckHitBox(iceBoltsCol[i]->GetBoxCol(), enemysCol[j]->GetBoxCol())) {
				iceBoltsCol[i]->SetIsHit();
				enemysCol[j]->SetIsHit(20, 5);
				enemysCol[j]->SetDebuff(ICE, 12);
			}
		}
		for (int j = 0; j < field->GetColSize(); j++) {
			if (CheckHitBox(iceBoltsCol[i]->GetBoxCol(), field->GetCol(j))) {
				iceBoltsCol[i]->SetIsHit();
			}
		}
	}

#pragma endregion

#pragma region チェインライトニングの判定
	//チェインライトニング
	vector<ChainLightning*> chainLightningsCol = SpellManager::GetInstance()->GetChainLightningsCol();


	for (int i = 0; i < chainLightningsCol.size(); i++) {
		for (int j = 0; j < enemysCol.size(); j++) {
			if (CheckHitBox(enemysCol[j]->GetBoxCol(), chainLightningsCol[i]->GetBoxCol())) {
				chainLightningsCol[i]->SetIsHit();
				enemysCol[j]->SetIsHit(15, 5);
				enemysCol[j]->SetDebuff(THUNDER, 1);

				//1体目の伝播
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

				//近いやつがいたら
				if (hitTemp1 != -1) {
					//一番近いやつにダメージ
					enemysCol[hitTemp1]->SetIsHit(15, 5);
					enemysCol[hitTemp1]->SetDebuff(THUNDER, 1);
					//そこまでのパーティクル
					Vector3 tempVec = enemysCol[hitTemp1]->GetBoxCol().pos - enemysCol[j]->GetBoxCol().pos;
					Vector3 nowPos = enemysCol[j]->GetBoxCol().pos;
					for (int l = 0; l < 20; l++) {
						nowPos += tempVec / 20;
						ParticleManager::GetInstance()->AddFromFile(P_LIGHTNING, nowPos);
					}

					//2体目の伝播
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
					//近いやつがいたら
					if (hitTemp2 != -1) {
						//一番近いやつにダメージ
						enemysCol[hitTemp2]->SetIsHit(15, 5);
						enemysCol[hitTemp2]->SetDebuff(THUNDER, 1);

						//そこまでのパーティクル
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

#pragma region プレイヤーと敵の判定

	for (int i = 0; i < enemysCol.size(); i++) {
		if (CheckHitBox(enemysCol[i]->GetBoxCol(), player->GetBoxCol())) {
			enemysCol[i]->SetIsStop();
			enemysCol[i]->SetIsAttack();
		}
	}

#pragma endregion 

#pragma region 敵同士の当たり判定
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

#pragma region 敵と壁の判定
	for (int i = 0; i < enemysCol.size(); i++) {
		for (int j = 0; j < field->GetColSize(); j++) {
			if (CheckHitBox(enemysCol[i]->GetBoxCol(), field->GetCol(j))) {
				if (CheckHitX(enemysCol[i]->GetBoxCol(), field->GetCol(j))) {
					if (!CheckHitX(enemysCol[i]->GetOldBoxCol(), field->GetCol(j))) {
						enemysCol[i]->SetColX(enemysCol[i]->GetOldBoxCol().pos.x);
					}
				}
				if (CheckHitY(enemysCol[i]->GetBoxCol(), field->GetCol(j))) {
					if (!CheckHitY(enemysCol[i]->GetOldBoxCol(), field->GetCol(j))) {
						enemysCol[i]->SetColY(enemysCol[i]->GetOldBoxCol().pos.y);
					}
				}
				if (CheckHitZ(enemysCol[i]->GetBoxCol(), field->GetCol(j))) {
					if (!CheckHitZ(enemysCol[i]->GetOldBoxCol(), field->GetCol(j))) {
						enemysCol[i]->SetColZ(enemysCol[i]->GetOldBoxCol().pos.z);
					}
				}
				enemysCol[i]->ResetShake();
				enemysCol[i]->WorldTransUpdate();
			}
		}
	}
#pragma endregion

#pragma region プレイヤーと壁の判定
	for (int i = 0; i < field->GetColSize(); i++) {
		if (CheckHitBox(player->GetBoxCol(), field->GetCol(i))) {
			if (CheckHitX(player->GetBoxCol(), field->GetCol(i))) {
				if (!CheckHitX(player->GetOldBoxCol(), field->GetCol(i))) {
					player->SetPosX(player->GetOldBoxCol().pos.x);
				}
			}
			if (CheckHitY(player->GetBoxCol(), field->GetCol(i))) {
				if (!CheckHitY(player->GetOldBoxCol(), field->GetCol(i))) {
					player->SetPosY(player->GetOldBoxCol().pos.y);
				}
			}
			if (CheckHitZ(player->GetBoxCol(), field->GetCol(i))) {
				if (!CheckHitZ(player->GetOldBoxCol(), field->GetCol(i))) {
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
	//敵の判定
	vector<BaseEnemy*> enemysCol = EnemyManager::GetInstance()->GetEnemysList();
	FieldManager* field = FieldManager::GetInstance();

	//チェインライトニング
	vector<ChainLightning*> chainLightningsCol = SpellManager::GetInstance()->GetChainLightningsCol();


	for (int i = 0; i < chainLightningsCol.size(); i++) {
		for (int j = 0; j < enemysCol.size(); j++) {
			if (CheckHitBox(enemysCol[j]->GetBoxCol(), chainLightningsCol[i]->GetBoxCol())) {
				return true;
			}
		}
		for (int j = 0; j < field->GetColSize(); j++) {
			if (CheckHitBox(chainLightningsCol[i]->GetBoxCol(), field->GetCol(j))) {
				return true;
			}
		}

	}

	return false;
}

bool ColliderManager::CanMovePlayerVec(Vector3 pos)
{
	FieldManager* field = FieldManager::GetInstance();

	Vector3 playerPos = Player::GetInstance()->GetBoxCol().pos;
	playerPos.y = 1;
	Vector3 myPos = pos;
	myPos.y = 1;

	Vector3 vecN;
	Vector3 vec = vecN = playerPos - myPos;
	vecN.normalize();

	if (vecN.x == 0) {
		vecN.x = 0.0001f;
	}
	int32_t num = int32_t(vec.x / vecN.x);

	for (int j = 0; j < num; j++) {
		myPos += vecN;
		for (int i = 0; i < field->GetColSize(); i++) {
			if (CheckHitBox({ myPos ,{1.5f,0.01f,1.5f} }, field->GetCol(i))) {
				return false;
			}
		}
	}
	return true;
}

int32_t ColliderManager::CanMoveNavePointVec(Vector3 pos)
{
	FieldManager* field = FieldManager::GetInstance();
	NavePointManager* navePointM = NavePointManager::GetInstance();
	Player* player = Player::GetInstance();

	int32_t temp = -1;

	for (int k = 0; k < field->GetNavePointNum(); k++) {
		if (navePointM->GetNavePoint(k).score == 99999) {
			continue;
		}

		bool isHit_ = false;

		Vector3 navePoint = navePointM->GetNavePoint(k).pos;
		navePoint.y = 1;
		Vector3 myPos = pos;
		myPos.y = 1;

		Vector3 vecN;
		Vector3 vec = vecN = navePoint - myPos;
		vecN.normalize();

		if (vecN.x == 0) {
			vecN.x = 0.0001f;
		}
		int32_t num = int32_t(vec.x / vecN.x);

		for (int j = 0; j < num; j++) {
			myPos += vecN;
			for (int i = 0; i < field->GetColSize(); i++) {
				if (CheckHitBox({ myPos ,{1.5f,0.01f,1.5f} }, field->GetCol(i))) {
					isHit_ = true;
					break;
				}
			}
			if (isHit_) {
				break;
			}
		}

		if (!isHit_) {
			if (temp != -1) {
				if (navePointM->GetNavePoint(k).score > navePointM->GetNavePoint(temp).score) {
					continue;
				}
			}
			temp = k;
		}
	}
	return temp;
}

void ColliderManager::SetNavePointScore()
{
	FieldManager* field = FieldManager::GetInstance();
	NavePointManager* navePointM = NavePointManager::GetInstance();

	for (int i = 0; i < field->GetNavePointNum(); i++) {
		navePointM->SetNaveScore(i, 99999);
	}

	//1回目
	for (int k = 0; k < field->GetNavePointNum(); k++) {
		bool isHit = false;
		Vector3 playerPos = Player::GetInstance()->GetBoxCol().pos;
		playerPos.y = 0.3f;
		Vector3 myPos = navePointM->GetNavePoint(k).pos;
		myPos.y = 0.3f;

		Vector3 vecN;
		Vector3 vec = vecN = playerPos - myPos;
		vecN.normalize();

		if (vecN.x == 0) {
			vecN.x = 0.0001f;
		}
		int32_t num = int32_t(vec.x / vecN.x);

		for (int j = 0; j < num; j++) {
			myPos += vecN;
			for (int i = 0; i < field->GetColSize(); i++) {
				if (CheckHitBox({ myPos ,{1.5f,0.01f,1.5f} }, field->GetCol(i))) {
					isHit = true;
					break;
				}
			}
			if (isHit) {
				break;
			}
		}

		if (!isHit) {
			navePointM->SetNaveScore(k, vec.length());
		}
	}

	for (int m = 0; m < 4; m++) {
		//2回目以降
		for (int l = 0; l < field->GetNavePointNum(); l++) {
			//まだ設定されていなければ終わる
			if (navePointM->GetNavePoint(l).score == 99999) {
				continue;
			}
			for (int k = 0; k < field->GetNavePointNum(); k++) {
				//設定されていたら終わる
				if (navePointM->GetNavePoint(k).score != 99999) {
					continue;
				}

				bool isHit = false;
				Vector3 naveEndPos = navePointM->GetNavePoint(l).pos;
				naveEndPos.y = 0.3f;
				Vector3 myPos = navePointM->GetNavePoint(k).pos;
				myPos.y = 0.3f;

				Vector3 vecN;
				Vector3 vec = vecN = naveEndPos - myPos;
				vecN.normalize();

				//ゼロ除算回避
				if (vecN.x == 0) {
					vecN.x = 0.0001f;
				}
				int32_t num = int32_t(vec.x / vecN.x);

				for (int j = 0; j < num; j++) {
					myPos += vecN;
					for (int i = 0; i < field->GetColSize(); i++) {
						if (CheckHitBox({ myPos ,{1.5f,0.01f,1.5f} }, field->GetCol(i))) {
							isHit = true;
							break;
						}
					}
					if (isHit) {
						break;
					}
				}

				if (!isHit) {
					navePointM->SetNaveScore(k, navePointM->GetNavePoint(l).score + vec.length());
				}
			}
		}
	}
}

