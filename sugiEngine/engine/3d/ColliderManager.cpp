#include "ColliderManager.h"
#include "Player.h"
#include "ParticleManager.h"

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

			colA->Draw();
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
	if (CheckHitCircle(spell->GetCol(), enemy->GetCol())) {
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

bool ColliderManager::CanMoveToPlayer(Vector3 pos, Vector3 col)
{
	Vector3 playerPos = Player::GetInstance()->GetPos();
	Vector3 way = playerPos - pos;
	way /= 50;
	way.y = 0;

	forward_list<BaseCol*>::iterator itA = colliders_.begin();
	for (; itA != colliders_.end(); ++itA) {
		BaseCol* colA = *itA;
		if (colA->GetColType() == WALL) {
			for (int i = 1; i < 50; i++) {
				Vector3 tempPos = pos + (way * (float)i);
				if (CheckHitBox(colA->GetCol(), { tempPos,col })) {
					return false;
				}
			}
		}
	}
	return true;
}

bool ColliderManager::CanMoveToNaviPoint(Vector3 pos1, Vector3 pos2, Vector3 col)
{
	Vector3 way = pos2 - pos1;
	way /= 50;
	way.y = 0;

	forward_list<BaseCol*>::iterator itA = colliders_.begin();
	for (; itA != colliders_.end(); ++itA) {
		BaseCol* colA = *itA;
		if (colA->GetColType() == WALL) {
			for (int i = 1; i < 50; i++) {
				Vector3 tempPos = pos1 + (way * (float)i);
				if (CheckHitBox(colA->GetCol(), { tempPos,col })) {
					return false;
				}
			}
		}
	}

	return true;
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
