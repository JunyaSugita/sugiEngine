#include "ColliderManager.h"
#include "Player.h"
#include "ParticleManager.h"
#include "NaviPointManager.h"
#include "ChainLightning.h"
#include "EffectManager.h"

using namespace std;

ColliderManager* ColliderManager::GetInstance()
{
	//インスタンス生成
	static ColliderManager instance;

	return &instance;
}

void ColliderManager::Initialize()
{
	colliders_.clear();
}

void ColliderManager::Update()
{
	forward_list<BaseCol*>::iterator itA;
	forward_list<BaseCol*>::iterator itB;

	//総当たり
	itA = colliders_.begin();
	for (; itA != colliders_.end(); ++itA) {
		itB = itA;
		++itB;
		for (; itB != colliders_.end(); ++itB) {
			BaseCol* colA = *itA;
			BaseCol* colB = *itB;

			//プレイヤーと壁の判定
			CheckHitPlayerToWall(colA, colB);

			//敵と壁の判定
			CheckHitEnemyToWall(colA, colB);

			//呪文と壁の判定
			CheckHitSpellToWall(colA, colB);

			//呪文と敵の判定
			CheckHitSpellToEnemy(colA, colB);

			//敵と敵の判定
			CheckHitEnemyToEnemy(colA, colB);

			//プレイヤーと敵の判定
			CheckHitPlayerToEnemy(colA, colB);

			//プレイヤーとゴールの判定
			CheckHitPlayerToGoal(colA, colB);
		}
	}
}

void ColliderManager::Draw()
{
	if (showHitBox_) {
		forward_list<BaseCol*>::iterator itA = colliders_.begin();
		for (; itA != colliders_.end(); ++itA) {
			BaseCol* colA = *itA;

			if (colA->GetColType() != WALL) {
				colA->Draw();
			}
		}
	}
}

void ColliderManager::AddCollider(BaseCol* baseCol)
{
	colliders_.push_front(baseCol);
}

void ColliderManager::DeleteCollider(BaseCol* baseCol)
{
	colliders_.remove(baseCol);
}

void ColliderManager::CheckHitPlayerToWall(BaseCol* a, BaseCol* b)
{
	if (a->GetColType() == PLAYER && b->GetColType() == WALL) {
		HitPlayerToWall(a, b);
	}
	else if (a->GetColType() == WALL && b->GetColType() == PLAYER) {
		HitPlayerToWall(b, a);
	}
}

void ColliderManager::CheckHitEnemyToWall(BaseCol* a, BaseCol* b)
{
	if (a->GetColType() == ENEMY && b->GetColType() == WALL) {
		HitEnemyToWall(a, b);
	}
	else if (a->GetColType() == WALL && b->GetColType() == ENEMY) {
		HitEnemyToWall(b, a);
	}
}

void ColliderManager::CheckHitSpellToWall(BaseCol* a, BaseCol* b)
{
	if (a->GetColType() == SPELL && b->GetColType() == WALL) {
		HitSpellToWall(a, b);
	}
	else if (a->GetColType() == WALL && b->GetColType() == SPELL) {
		HitSpellToWall(b, a);
	}
}

void ColliderManager::CheckHitSpellToEnemy(BaseCol* a, BaseCol* b)
{
	if (a->GetColType() == SPELL && b->GetColType() == ENEMY) {
		HitSpellToEnemy(a, b);
	}
	else if (a->GetColType() == ENEMY && b->GetColType() == SPELL) {
		HitSpellToEnemy(b, a);
	}
}

void ColliderManager::CheckHitPlayerToEnemy(BaseCol* a, BaseCol* b)
{
	if (a->GetColType() == PLAYER && b->GetColType() == ENEMY) {
		HitPlayerToEnemy(a, b);
	}
	else if (a->GetColType() == ENEMY && b->GetColType() == PLAYER) {
		HitPlayerToEnemy(b, a);
	}
}

void ColliderManager::CheckHitEnemyToEnemy(BaseCol* a, BaseCol* b)
{
	if (a->GetColType() == ENEMY && b->GetColType() == ENEMY) {
		HitEnemyToEnemy(a, b);
	}
}

void ColliderManager::CheckHitPlayerToGoal(BaseCol* a, BaseCol* b)
{
	if (a->GetColType() == PLAYER && b->GetColType() == GOAL) {
		HitPlayerToGoal(a, b);
	}
	else if (a->GetColType() == GOAL && b->GetColType() == PLAYER) {
		HitPlayerToGoal(b, a);
	}
}

void ColliderManager::HitPlayerToWall(BaseCol* player, BaseCol* wall)
{
	if (CheckHitBox(player->GetCol(), wall->GetCol())) {
		//X押し戻し
		if (CheckHitX(player->GetCol(), wall->GetCol())) {
			if (!CheckHitX(player->GetOldCol(), wall->GetOldCol())) {
				player->SetColX(player->GetOldCol().pos.x);
				player->HitChangePos();
			}
		}
		//Y押し戻し
		if (CheckHitY(player->GetCol(), wall->GetCol())) {
			if (!CheckHitY(player->GetOldCol(), wall->GetOldCol())) {
				player->SetColY(player->GetOldCol().pos.y);
				player->HitChangePos();
			}
		}
		//Z押し戻し
		if (CheckHitZ(player->GetCol(), wall->GetCol())) {
			if (!CheckHitZ(player->GetOldCol(), wall->GetOldCol())) {
				player->SetColZ(player->GetOldCol().pos.z);
				player->HitChangePos();
			}
		}
	}
}

void ColliderManager::HitEnemyToWall(BaseCol* enemy, BaseCol* wall)
{
	if (CheckHitBox(enemy->GetCol(), wall->GetCol())) {
		//X押し戻し
		if (CheckHitX(enemy->GetCol(), wall->GetCol())) {
			if (!CheckHitX(enemy->GetOldCol(), wall->GetOldCol())) {
				enemy->SetColX(enemy->GetOldCol().pos.x);
				enemy->HitChangePos();
			}
		}
		//Y押し戻し
		if (CheckHitY(enemy->GetCol(), wall->GetCol())) {
			if (!CheckHitY(enemy->GetOldCol(), wall->GetOldCol())) {
				enemy->SetColY(enemy->GetOldCol().pos.y);
				enemy->HitChangePos();
			}
		}
		//Z押し戻し
		if (CheckHitZ(enemy->GetCol(), wall->GetCol())) {
			if (!CheckHitZ(enemy->GetOldCol(), wall->GetOldCol())) {
				enemy->SetColZ(enemy->GetOldCol().pos.z);
				enemy->HitChangePos();
			}
		}
	}
}

void ColliderManager::HitEnemyToEnemy(BaseCol* enemy1, BaseCol* enemy2)
{
	if (!enemy1->GetIsDown() && !enemy2->GetIsDown()) {
		if (CheckHitCircle(enemy1->GetCol(), enemy2->GetCol())) {
			if (enemy1->GetCol().pos.x <= enemy2->GetCol().pos.x) {
				enemy1->AddColX(-PUSH_LEN);
				enemy2->AddColX(PUSH_LEN);
			}
			else if (enemy1->GetCol().pos.x > enemy2->GetCol().pos.x) {
				enemy1->AddColX(PUSH_LEN);
				enemy2->AddColX(-PUSH_LEN);
			}

			if (enemy1->GetCol().pos.z <= enemy2->GetCol().pos.z) {
				enemy1->AddColZ(-PUSH_LEN);
				enemy2->AddColZ(PUSH_LEN);
			}
			else if (enemy1->GetCol().pos.z > enemy2->GetCol().pos.z) {
				enemy1->AddColZ(PUSH_LEN);
				enemy2->AddColZ(-PUSH_LEN);
			}
		}
	}
	//後者のみが倒れてる
	else if (!enemy1->GetIsDown() && enemy2->GetIsDown()) {
		if (CheckHitCircle(enemy1->GetCol(), enemy2->GetCol())) {
			enemy1->SetDownHitEnemy(enemy2->GetDownHitEnemy());
		}
	}
	//前者のみが倒れている
	else if (enemy1->GetIsDown() && !enemy2->GetIsDown()) {
		if (CheckHitCircle(enemy1->GetCol(), enemy2->GetCol())) {
			enemy2->SetDownHitEnemy(enemy1->GetDownHitEnemy());
		}
	}
}

void ColliderManager::HitSpellToWall(BaseCol* spell, BaseCol* wall)
{
	if (CheckHitBox(spell->GetCol(), wall->GetCol())) {
		spell->SetIsHit();
	}
}

void ColliderManager::HitSpellToEnemy(BaseCol* spell, BaseCol* enemy)
{
	//まだ当たってない呪文と倒れている敵は判定しない
	if (!spell->GetIsHit() && enemy->GetIsDown()) {
		return;
	}
	if (CheckHitBox(spell->GetCol(), enemy->GetCol())) {
		if (!spell->GetIsHit()) {
			//敵に当たった瞬間
			ParticleManager::GetInstance()->AddFromFile(P_DAMAGE, enemy->GetPos());
			enemy->SetShakeTime(TIME_SHAKE);
		}
		spell->SetIsHit();
		enemy->SubLife(spell->GetDamage());
		enemy->SetDebuff(spell->GetDebuff(), DEBUFF_TIME);
	}
}

void ColliderManager::HitPlayerToEnemy(BaseCol* player, BaseCol* enemy)
{
	if (CheckHitCircle(player->GetCol(), enemy->GetCol())) {
		if (!enemy->GetIsDown()) {
			enemy->SetIsStop();
			enemy->SetIsAttack();
		}
		else {
			enemy->DownHitPlayer();
		}
	}
}

void ColliderManager::HitPlayerToGoal(BaseCol* player, BaseCol* goal)
{
	if (CheckHitBox(player->GetCol(), goal->GetCol())) {
		goal->SetClear();
	}
}

bool ColliderManager::CanMoveToPlayer(Vector3 pos)
{
	Vector3 playerPos = Camera::GetInstance()->GetEye();

	forward_list<BaseCol*>::iterator itA = colliders_.begin();
	for (; itA != colliders_.end(); ++itA) {
		BaseCol* colA = *itA;
		if (colA->GetColType() == WALL) {
			if (CheckHitLineToBox(pos, playerPos, colA->GetCol())) {
				return false;
			}
		}
	}
	return true;
}

bool ColliderManager::CanMoveToNaviPoint(Vector3 pos1, Vector3 pos2)
{
	forward_list<BaseCol*>::iterator itA = colliders_.begin();
	for (; itA != colliders_.end(); ++itA) {
		BaseCol* colA = *itA;
		if (colA->GetColType() == WALL) {
			if (CheckHitLineToBox(pos1, pos2, colA->GetCol())) {
				return false;
			}
		}
	}

	return true;
}

Vector3 ColliderManager::CanMoveEnemyToNaviPoint(Vector3 pos)
{
	vector<NaviPoint> naviPoints = NaviPointManager::GetInstance()->GetNaviPoints();

	float score = 99999;
	int32_t num = -1;

	for (int i = 0; i < naviPoints.size(); i++) {
		if (score <= naviPoints[i].score) {
			continue;
		}

		forward_list<BaseCol*>::iterator itA = colliders_.begin();
		for (; itA != colliders_.end(); ++itA) {
			BaseCol* colA = *itA;
			if (colA->GetColType() == WALL) {
				if (CheckHitLineToBox(pos, naviPoints[i].pos, colA->GetCol())) {
					continue;
				}
			}
		}

		score = naviPoints[i].score;
		num = i;
	}

	if (num == -1) {
		return pos;
	}

	return naviPoints[num].pos;
}

bool ColliderManager::CheckHitEnemyToRay(Vector3 pos, Vector3 ray)
{
	Vector3 endPos = pos + ray * 100;

	int32_t type = -1;
	float len = 99999;

	forward_list<BaseCol*>::iterator itA = colliders_.begin();
	for (; itA != colliders_.end(); ++itA) {
		BaseCol* colA = *itA;
		if (colA->GetColType() == ENEMY || colA->GetColType() == WALL) {
			if (CheckHitLineToBox(pos, endPos, colA->GetCol())) {
				Vector3 tempPos = CheckHitPosLineToBox(pos, endPos, colA->GetCol());
				float tempLen = (tempPos - pos).length();
				if (len > tempLen) {
					type = colA->GetColType();
					len = tempLen;
				}
			}
		}
	}

	if (type != ENEMY) {
		return false;
	}
	else {
		return true;
	}
}

Vector3 ColliderManager::CheckHitPosEnemyToRay(Vector3 pos, Vector3 ray)
{
	Vector3 endPos = pos + ray * 100;

	int32_t type = -1;
	float len = 99999;
	Vector3 ansPos;

	forward_list<BaseCol*>::iterator itA = colliders_.begin();
	for (; itA != colliders_.end(); ++itA) {
		BaseCol* colA = *itA;
		if (colA->GetColType() == ENEMY) {
			if (CheckHitLineToBox(pos, endPos, colA->GetCol())) {
				Vector3 tempPos = CheckHitPosLineToBox(pos, endPos, colA->GetCol());
				float tempLen = (tempPos - pos).length();
				if (len > tempLen) {
					type = colA->GetColType();
					len = tempLen;
					ansPos = colA->GetCol().pos;
					colA->SubLife(ChainLightning::DAMAGE);
					colA->SetDebuff(D_STAN, ChainLightning::TIME_STAN);
				}
			}
		}
	}

	return ansPos;
}

bool ColliderManager::CheckNearEnemy(int32_t num)
{
	int32_t tempNum = 0;

	switch (num)
	{
	case 0:
		tempNum = 1;
		break;
	case 2:
		tempNum = 2;
		break;

	default:
		return false;
		break;
	}

	//プレイヤー
	Player* player = Player::GetInstance();

	///プレイヤーと敵
	//プレイヤーが無敵かどうか
	if (!Player::GetInstance()->GetInvincible()) {
		forward_list<BaseCol*>::iterator itA = colliders_.begin();
		for (; itA != colliders_.end(); ++itA) {
			BaseCol* colA = *itA;
			if (colA->GetColType() == ENEMY) {
				//敵がダウンしているかどうか
				if (!colA->GetIsDown()) {
					//判定
					if (CheckHitCircle(colA->GetCol(), { player->GetBoxCol().pos,{70,50,70} })) {
						if (colA->GetSerial() == tempNum) {
							return true;
						}
					}
				}
			}
		}
	}

	return false;
}

void ColliderManager::LightningEnemyToEnemy(Vector3 pos)
{
	forward_list<BaseCol*>::iterator itA = colliders_.begin();
	for (; itA != colliders_.end(); ++itA) {
		BaseCol* colA = *itA;
		if (colA->GetColType() == ENEMY) {
			if (CheckHitLineToBox(pos, colA->GetPos(), colA->GetCol()) && CheckHitCircle({ pos,{10,10,10} }, colA->GetCol())) {
				Vector3 tempPos = CheckHitPosLineToBox(pos, colA->GetPos(), colA->GetCol());
				colA->SubLife(ChainLightning::DAMAGE);
				colA->SetDebuff(D_STAN, ChainLightning::TIME_STAN);
				EffectManager::GetInstance()->BoltGenerate(pos, colA->GetCol().pos, { (Player::GetInstance()->GetCameraAngle().y) * -1,Player::GetInstance()->GetCameraAngle().x,0 }, { 0.5f,0.5f,1,0.5f });
			}
		}
	}
}

bool ColliderManager::CheckHitBox(Col a, Col b)
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

bool ColliderManager::CheckHitCircle(Col a, Col b)
{
	Vector3 temp = a.pos - b.pos;

	if (temp.length() < a.size.x + b.size.x) {
		return true;
	}

	return false;

}

bool ColliderManager::CheckHitX(Col a, Col b)
{
	if (a.pos.x + a.size.x >= b.pos.x - b.size.x && b.pos.x + b.size.x >= a.pos.x - a.size.x) {
		return true;
	}
	return false;
}

bool ColliderManager::CheckHitY(Col a, Col b)
{
	if (a.pos.y + a.size.y >= b.pos.y - b.size.y && b.pos.y + b.size.y >= a.pos.y - a.size.y) {
		return true;
	}
	return false;
}

bool ColliderManager::CheckHitZ(Col a, Col b)
{
	if (a.pos.z + a.size.z >= b.pos.z - b.size.z && b.pos.z + b.size.z >= a.pos.z - a.size.z) {
		return true;
	}
	return false;
}

bool ColliderManager::CheckHitLineToBox(Vector3 posS, Vector3 posE, Col a)
{
	//方向ベクトル取得
	Vector3 way = posE - posS;
	way.normalize();

	//x軸処理
	Vector2 hitXLength = {};
	Vector2 AABBXPos;
	AABBXPos.x = a.pos.x - (a.size.x + 1);
	AABBXPos.y = a.pos.x + (a.size.x + 1);

	//線分の判定取り
	if (posS.x < AABBXPos.x && posE.x < AABBXPos.x) {
		return false;
	}
	if (posS.x > AABBXPos.y && posE.x > AABBXPos.y) {
		return false;
	}

	hitXLength.x = (AABBXPos.x - posS.x) / way.x;
	hitXLength.y = (AABBXPos.y - posS.x) / way.x;

	//xを小さくしたいのでxの方が大きい時、入れ替える。
	if (hitXLength.x > hitXLength.y) {
		float temp = hitXLength.x;
		hitXLength.x = hitXLength.y;
		hitXLength.y = temp;
	}

	//y軸処理
	Vector2 hitYLength = {};
	Vector2 AABBYPos;
	AABBYPos.x = a.pos.y - (a.size.y + 1);
	AABBYPos.y = a.pos.y + (a.size.y + 1);

	//線分の判定取り
	if (posS.y < AABBYPos.x && posE.y < AABBYPos.x) {
		return false;
	}
	if (posS.y > AABBYPos.y && posE.y > AABBYPos.y) {
		return false;
	}

	hitYLength.x = (AABBYPos.x - posS.y) / way.y;
	hitYLength.y = (AABBYPos.y - posS.y) / way.y;

	//xを小さくしたいのでxの方が大きい時、入れ替える。
	if (hitYLength.x > hitYLength.y) {
		float temp = hitYLength.x;
		hitYLength.x = hitYLength.y;
		hitYLength.y = temp;
	}

	//z軸処理
	Vector2 hitZLength = {};
	Vector2 AABBZPos;
	AABBZPos.x = a.pos.z - (a.size.z + 1);
	AABBZPos.y = a.pos.z + (a.size.z + 1);

	//線分の判定取り
	if (posS.z < AABBZPos.x && posE.z < AABBZPos.x) {
		return false;
	}
	if (posS.z > AABBZPos.y && posE.z > AABBZPos.y) {
		return false;
	}

	hitZLength.x = (AABBZPos.x - posS.z) / way.z;
	hitZLength.y = (AABBZPos.y - posS.z) / way.z;

	//xを小さくしたいのでxの方が大きい時、入れ替える。
	if (hitZLength.x > hitZLength.y) {
		float temp = hitZLength.x;
		hitZLength.x = hitZLength.y;
		hitZLength.y = temp;
	}

	//遅い入りと早い出を計算
	Vector2 inOut;

	//入り
	inOut.x = Max(hitXLength.x, hitYLength.x, hitZLength.x);

	//出
	inOut.y = Min(hitXLength.y, hitYLength.y, hitZLength.y);

	//y-xが+なら当たっている
	if (inOut.y - inOut.x > 0) {
		return true;
	}

	return false;
}

Vector3 ColliderManager::CheckHitPosLineToBox(Vector3 posS, Vector3 posE, Col a)
{
	//方向ベクトル取得
	Vector3 way = posE - posS;
	way.normalize();

	//x軸処理
	Vector2 hitXLength = {};
	Vector2 AABBXPos;
	AABBXPos.x = a.pos.x - (a.size.x + 1);
	AABBXPos.y = a.pos.x + (a.size.x + 1);

	hitXLength.x = (AABBXPos.x - posS.x) / way.x;
	hitXLength.y = (AABBXPos.y - posS.x) / way.x;

	//xを小さくしたいのでxの方が大きい時、入れ替える。
	if (hitXLength.x > hitXLength.y) {
		float temp = hitXLength.x;
		hitXLength.x = hitXLength.y;
		hitXLength.y = temp;
	}

	//y軸処理
	Vector2 hitYLength = {};
	Vector2 AABBYPos;
	AABBYPos.x = a.pos.y - (a.size.y + 1);
	AABBYPos.y = a.pos.y + (a.size.y + 1);

	hitYLength.x = (AABBYPos.x - posS.y) / way.y;
	hitYLength.y = (AABBYPos.y - posS.y) / way.y;

	//xを小さくしたいのでxの方が大きい時、入れ替える。
	if (hitYLength.x > hitYLength.y) {
		float temp = hitYLength.x;
		hitYLength.x = hitYLength.y;
		hitYLength.y = temp;
	}

	//z軸処理
	Vector2 hitZLength = {};
	Vector2 AABBZPos;
	AABBZPos.x = a.pos.z - (a.size.z + 1);
	AABBZPos.y = a.pos.z + (a.size.z + 1);

	hitZLength.x = (AABBZPos.x - posS.z) / way.z;
	hitZLength.y = (AABBZPos.y - posS.z) / way.z;

	//xを小さくしたいのでxの方が大きい時、入れ替える。
	if (hitZLength.x > hitZLength.y) {
		float temp = hitZLength.x;
		hitZLength.x = hitZLength.y;
		hitZLength.y = temp;
	}

	//遅い入りと早い出を計算
	Vector2 inOut;

	//入り
	inOut.x = Max(hitXLength.x, hitYLength.x, hitZLength.x);

	//出
	inOut.y = Min(hitXLength.y, hitYLength.y, hitZLength.y);

	return posS + way * inOut.x;
}
