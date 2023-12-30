//当たり判定を管理するマネージャークラス

#include "ColliderManager.h"
#include "SpellManager.h"
#include "EnemyManager.h"
#include "ParticleManager.h"
#include "Player.h"
#include "FieldManager.h"
#include "NaviPointManager.h"
#include "EffectManager.h"
#include "ClearChecker.h"


using namespace std;

ColliderManager* ColliderManager::GetInstance()
{
	//インスタンス生成
	static ColliderManager instance;

	return &instance;
}

void ColliderManager::Initialize()
{
	//初期化
	isShowHitBox_ = false;
}

void ColliderManager::Update()
{
	//呪文の判定
	CheckSpellCol();

	//チェインライトニングの判定
	CheckChainLightningCol();

	//プレイヤーと敵の判定
	CheckPlayerToEnemyCol();

	//敵同士の当たり判定
	CheckEnemyToEnemyCol();

	//敵と壁の判定
	CheckEnemyToFieldCol();

	//プレイヤーと壁の判定
	CheckPlayerToFieldCol();

	//プレイヤーとゴールの判定
	CheckPlayerToGoalCol();
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

bool ColliderManager::CheckHitCircle(BoxCol a, BoxCol b)
{
	Vector3 temp = a.pos - b.pos;

	if (temp.length() < a.size.x + b.size.x) {
		return true;
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
			if (CheckHitBox({ myPos ,TEMP_ENEMY_HITBOX }, field->GetCol(i))) {
				return false;
			}
		}
	}
	return true;
}

int32_t ColliderManager::CanMoveNaviPointVec(Vector3 pos)
{
	FieldManager* field = FieldManager::GetInstance();
	NaviPointManager* navePointM = NaviPointManager::GetInstance();

	int32_t temp = -1;

	for (int k = 0; k < field->GetNaviPointNum(); k++) {
		if (navePointM->GetNaviPoint(k).score == 99999) {
			continue;
		}

		bool isHit_ = false;

		Vector3 navePoint = navePointM->GetNaviPoint(k).pos;
		navePoint.y = TEMP_Y;
		Vector3 myPos = pos;
		myPos.y = TEMP_Y;

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
				if (CheckHitBox({ myPos ,TEMP_ENEMY_HITBOX }, field->GetCol(i))) {
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
				if (navePointM->GetNaviPoint(k).score > navePointM->GetNaviPoint(temp).score) {
					continue;
				}
			}
			temp = k;
		}
	}
	return temp;
}

void ColliderManager::SetNaviPointScore()
{
	FieldManager* field = FieldManager::GetInstance();
	NaviPointManager* navePointM = NaviPointManager::GetInstance();

	for (int i = 0; i < field->GetNaviPointNum(); i++) {
		navePointM->SetNaviScore(i, 99999);
	}

	//1回目
	for (int k = 0; k < field->GetNaviPointNum(); k++) {
		bool isHit = false;
		Vector3 playerPos = Player::GetInstance()->GetBoxCol().pos;
		playerPos.y = TEMP_Y;
		Vector3 myPos = navePointM->GetNaviPoint(k).pos;
		myPos.y = TEMP_Y;

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
				if (CheckHitBox({ myPos ,TEMP_ENEMY_HITBOX }, field->GetCol(i))) {
					isHit = true;
					break;
				}
			}
			if (isHit) {
				break;
			}
		}

		if (!isHit) {
			navePointM->SetNaviScore(k, vec.length());
		}
	}

	for (int m = 0; m < 2; m++) {
		//2回目以降
		for (int l = 0; l < field->GetNaviPointNum(); l++) {
			//まだ設定されていなければ終わる
			if (navePointM->GetNaviPoint(l).score == 99999) {
				continue;
			}
			for (int k = 0; k < field->GetNaviPointNum(); k++) {
				//設定されていたら終わる
				if (navePointM->GetNaviPoint(k).score != 99999) {
					continue;
				}

				bool isHit = false;
				Vector3 naveEndPos = navePointM->GetNaviPoint(l).pos;
				naveEndPos.y = TEMP_Y;
				Vector3 myPos = navePointM->GetNaviPoint(k).pos;
				myPos.y = TEMP_Y;

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
						if (CheckHitBox({ myPos ,TEMP_ENEMY_HITBOX }, field->GetCol(i))) {
							isHit = true;
							break;
						}
					}
					if (isHit) {
						break;
					}
				}

				if (!isHit) {
					navePointM->SetNaviScore(k, navePointM->GetNaviPoint(l).score + vec.length());
				}
			}
		}
	}
}

void ColliderManager::CheckSpellCol()
{
	//呪文の判定
	vector<BaseSpell*> spellsCol = SpellManager::GetInstance()->GetSpellsCol();
	//敵の判定
	vector<BaseEnemy*> enemysCol = EnemyManager::GetInstance()->GetEnemysList();
	//マップの判定
	FieldManager* field = FieldManager::GetInstance();

	for (int i = 0; i < spellsCol.size(); i++) {
		//当たり判定を行わない呪文は弾く
		if (!spellsCol[i]->GetIsCalcCol()) {
			continue;
		}

		//呪文と敵の判定
		for (int j = 0; j < enemysCol.size(); j++) {
			//まだ当たってない呪文と倒れている敵は判定しない
			if (!spellsCol[i]->GetIsHit() && enemysCol[j]->GetIsDown()) {
				continue;
			}

			//判定
			if (CheckHitBox(spellsCol[i]->GetBoxCol(), enemysCol[j]->GetBoxCol())) {
				//呪文が敵に当たっている時
				if (!spellsCol[i]->GetIsHit()) {
					//敵に当たった瞬間
					ParticleManager::GetInstance()->AddFromFile(P_DAMAGE, enemysCol[j]->GetPos());
					enemysCol[j]->SetShakeTime(TIME_SHAKE);
				}
				spellsCol[i]->SetIsHit();
				enemysCol[j]->SetIsHit(spellsCol[i]->GetDamage());
				enemysCol[j]->SetDebuff(spellsCol[i]->GetDebuff(), TIME_DEBUFF);
			}
		}
		//呪文と壁や床との判定
		for (int j = 0; j < field->GetColSize(); j++) {
			//判定
			if (CheckHitBox(spellsCol[i]->GetBoxCol(), field->GetCol(j))) {
				//呪文が壁や床と当たっている時
				spellsCol[i]->SetIsHit();
			}
		}
	}
}

void ColliderManager::CheckChainLightningCol()
{
	//敵の判定
	vector<BaseEnemy*> enemysCol = EnemyManager::GetInstance()->GetEnemysList();
	//チェインライトニング
	vector<ChainLightning*> chainLightningsCol = SpellManager::GetInstance()->GetChainLightningsCol();

	//敵との判定を取る
	for (int i = 0; i < chainLightningsCol.size(); i++) {
		for (int j = 0; j < enemysCol.size(); j++) {
			//判定
			if (CheckHitBox(enemysCol[j]->GetBoxCol(), chainLightningsCol[i]->GetBoxCol())) {
				//ライトニングの判定先に敵がいたとき
				chainLightningsCol[i]->SetIsHit();
				enemysCol[j]->SetIsHit(ChainLightning::DAMAGE);
				enemysCol[j]->SetDebuff(D_STAN, ChainLightning::TIME_STAN);

				//1体目の伝播
				int32_t hitTemp1 = -1;
				float lenTemp = 30;

				//近い敵を探す
				for (int k = 0; k < enemysCol.size(); k++) {
					//被り防止
					if (j != k) {
						float length = (enemysCol[j]->GetBoxCol().pos - enemysCol[k]->GetBoxCol().pos).length();
						//距離を比べて近い敵をhitTemp1に代入
						if (lenTemp > length) {
							lenTemp = length;
							hitTemp1 = k;
						}
					}
				}

				//近いやつがいたら
				if (hitTemp1 != -1) {
					//一番近いやつにダメージ
					enemysCol[hitTemp1]->SetIsHit(ChainLightning::DAMAGE);
					enemysCol[hitTemp1]->SetDebuff(D_STAN, ChainLightning::TIME_STAN);
					//そこまでのパーティクル
					EffectManager::GetInstance()->BoltGenerate(enemysCol[j]->GetBoxCol().pos, enemysCol[hitTemp1]->GetBoxCol().pos, { 0,0,0 }, { 0.5f,0.5f,1,0.5f });

					//2体目の伝播
					int32_t hitTemp2 = -1;
					lenTemp = 30;

					//近い敵を探す
					for (int k = 0; k < enemysCol.size(); k++) {
						//被り防止
						if (j != k && hitTemp1 != k) {
							float length = (enemysCol[hitTemp1]->GetBoxCol().pos - enemysCol[k]->GetBoxCol().pos).length();
							//より近い敵をhitTemp2に代入
							if (lenTemp > length) {
								lenTemp = length;
								hitTemp2 = k;
							}
						}
					}
					//近いやつがいたら
					if (hitTemp2 != -1) {
						//一番近いやつにダメージ
						enemysCol[hitTemp2]->SetIsHit(ChainLightning::DAMAGE);
						enemysCol[hitTemp2]->SetDebuff(D_STAN, ChainLightning::TIME_STAN);

						//そこまでのパーティクル
						EffectManager::GetInstance()->BoltGenerate(enemysCol[hitTemp1]->GetBoxCol().pos, enemysCol[hitTemp2]->GetBoxCol().pos, { 0,0,0 }, { 0.5f,0.5f,1,0.5f });
					}
				}
			}
		}
	}
}

void ColliderManager::CheckPlayerToEnemyCol()
{
	//敵の判定
	vector<BaseEnemy*> enemysCol = EnemyManager::GetInstance()->GetEnemysList();
	//プレイヤー
	Player* player = Player::GetInstance();

	///プレイヤーと敵
	//プレイヤーが無敵かどうか
	if (!Player::GetInstance()->GetInvincible()) {
		for (int i = 0; i < enemysCol.size(); i++) {
			//敵がダウンしているかどうか
			if (!enemysCol[i]->GetIsDown()) {
				//判定
				if (CheckHitBox(enemysCol[i]->GetBoxCol(), player->GetBoxCol())) {
					enemysCol[i]->SetIsStop();
					enemysCol[i]->SetIsAttack();
				}
			}
			else {
				//ダウンしている時の判定
				if (CheckHitCircle(enemysCol[i]->GetBoxCol(), player->GetBoxCol())) {
					enemysCol[i]->DownHitPlayer();
				}
			}
		}
	}
}

void ColliderManager::CheckEnemyToEnemyCol()
{
	//敵の判定
	vector<BaseEnemy*> enemysCol = EnemyManager::GetInstance()->GetEnemysList();

	for (int i = 0; i < enemysCol.size(); i++) {
		for (int j = i + 1; j < enemysCol.size(); j++) {
			if (!enemysCol[i]->GetIsDown() && !enemysCol[j]->GetIsDown()) {
				if (CheckHitCircle(enemysCol[i]->GetBoxCol(), enemysCol[j]->GetBoxCol())) {
					if (enemysCol[i]->GetBoxCol().pos.x <= enemysCol[j]->GetBoxCol().pos.x) {
						enemysCol[i]->AddColX(-PUSH_LEN);
						enemysCol[j]->AddColX(PUSH_LEN);
					}
					else if (enemysCol[i]->GetBoxCol().pos.x > enemysCol[j]->GetBoxCol().pos.x) {
						enemysCol[i]->AddColX(PUSH_LEN);
						enemysCol[j]->AddColX(-PUSH_LEN);
					}

					if (enemysCol[i]->GetBoxCol().pos.z <= enemysCol[j]->GetBoxCol().pos.z) {
						enemysCol[i]->AddColZ(-PUSH_LEN);
						enemysCol[j]->AddColZ(PUSH_LEN);
					}
					else if (enemysCol[i]->GetBoxCol().pos.z > enemysCol[j]->GetBoxCol().pos.z) {
						enemysCol[i]->AddColZ(PUSH_LEN);
						enemysCol[j]->AddColZ(-PUSH_LEN);
					}
				}
			}
			//後者のみが倒れてる
			else if (!enemysCol[i]->GetIsDown() && enemysCol[j]->GetIsDown()) {
				if (CheckHitCircle(enemysCol[i]->GetBoxCol(), enemysCol[j]->GetBoxCol())) {
					enemysCol[i]->SetDownHitEnemy(enemysCol[j]->GetDownHitEnemy());
				}
			}
			//前者のみが倒れている
			else if (enemysCol[i]->GetIsDown() && !enemysCol[j]->GetIsDown()) {
				if (CheckHitCircle(enemysCol[i]->GetBoxCol(), enemysCol[j]->GetBoxCol())) {
					enemysCol[j]->SetDownHitEnemy(enemysCol[i]->GetDownHitEnemy());
				}
			}
		}
	}
}

void ColliderManager::CheckEnemyToFieldCol()
{
	//敵の判定
	vector<BaseEnemy*> enemysCol = EnemyManager::GetInstance()->GetEnemysList();
	//マップの判定
	FieldManager* field = FieldManager::GetInstance();

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
				enemysCol[i]->WorldTransUpdate();
			}
		}
	}
}

void ColliderManager::CheckPlayerToFieldCol()
{
	//マップの判定
	FieldManager* field = FieldManager::GetInstance();
	//プレイヤー
	Player* player = Player::GetInstance();

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
}

void ColliderManager::CheckPlayerToGoalCol()
{
	//プレイヤー
	Player* player = Player::GetInstance();
	//ゴール
	ClearChecker* clear = ClearChecker::GetInstance();

	if (CheckHitBox(player->GetBoxCol(), clear->GetCol())) {
		clear->SetClear();
	}
}

