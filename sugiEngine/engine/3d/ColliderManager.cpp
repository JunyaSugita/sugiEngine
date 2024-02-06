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

			colA->OnCollision(colB);
			colB->OnCollision(colA);
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

bool ColliderManager::CanMoveToPlayer(const Vector3& pos)
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

bool ColliderManager::CanMoveToNaviPoint(const Vector3& pos1, const Vector3& pos2)
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

Vector3 ColliderManager::CanMoveEnemyToNaviPoint(const Vector3& pos)
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

bool ColliderManager::CheckHitEnemyToRay(const Vector3& pos, const Vector3& ray)
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

Vector3 ColliderManager::CheckHitPosEnemyToRay(const Vector3& pos, const Vector3& ray)
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

void ColliderManager::LightningEnemyToEnemy(const Vector3& pos)
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

bool ColliderManager::CheckHitLineToBox(const Vector3& posS, const Vector3& posE, Col a)
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

Vector3 ColliderManager::CheckHitPosLineToBox(const Vector3& posS, const Vector3& posE, Col a)
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
