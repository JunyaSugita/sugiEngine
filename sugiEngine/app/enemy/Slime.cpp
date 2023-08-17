#include "Slime.h"
#include "NavePointManager.h"
#include "Player.h"

void Slime::StaticInitialize()
{
}

void Slime::Initialize(std::string name, Vector3 pos)
{
	life_ = MAX_HP;
	height_ = HEIGHT_COL;

	BaseEnemy::Initialize(name, pos);
	obj_.obj->SetColor({ 0,1,1,0.2f });
	WorldTransUpdate();

}

void Slime::Draw()
{
	BaseEnemy::Draw();
}

void Slime::WorldTransUpdate()
{
	col_.col.size = { obj_.scale.x,obj_.scale.y ,obj_.scale.x };
	BaseEnemy::WorldTransUpdate();
}

void Slime::Move()
{
	timer_ += 0.1f;
	obj_.scale.x = 1 + sinf(timer_) * 0.5f;
	obj_.scale.y = 1 + cosf(timer_) * 0.5f;
	obj_.scale.z = 1 + sinf(timer_) * 0.5f;

	ColliderManager* colM = ColliderManager::GetInstance();
	NavePointManager* navePointM = NavePointManager::GetInstance();

	if (!isStop_) {
		Vector2 temp;
		if (colM->CanMovePlayerVec(obj_.pos)) {
			temp.x = Player::GetInstance()->GetBoxCol().pos.x;
			temp.y = Player::GetInstance()->GetBoxCol().pos.z;
		}
		else {
			temp.x = navePointM->GetNavePoint(colM->CanMoveNavePointVec(obj_.pos)).pos.x;
			temp.y = navePointM->GetNavePoint(colM->CanMoveNavePointVec(obj_.pos)).pos.z;
		}

		toPlayer = Vector2(temp.x - obj_.pos.x, temp.y - obj_.pos.z);
		toPlayer.normalize();
		obj_.pos.x += toPlayer.x * SPEED_MOVE * GetSlow();
		obj_.pos.z += toPlayer.y * SPEED_MOVE * GetSlow();
	}
	else {
		isStop_ = false;
	}
}

void Slime::Attack()
{
	if (isAttack_) {
		attackTimer_ -= 0.025f;

		timer_ += 0.1f;
		obj_.scale.x = 1 + sinf(timer_) * 0.5f;
		obj_.scale.y = 1 + cosf(timer_) * 0.5f;
		obj_.scale.z = 1 + sinf(timer_) * 0.5f;

		if (attackTimer_ <= 0) {
			isAttack_ = false;
		}
	}
}

void Slime::Down()
{
	if (obj_.scale.y > 0.1f) {
		obj_.scale.y -= 0.1f;
	}
	if (obj_.scale.x < 10) {
		obj_.scale.x += 0.5f;
		obj_.scale.z += 0.5f;
	}
	obj_.obj->SetColor({1,0,0,1});

	//ç≈å„
	BaseEnemy::Down();
}
