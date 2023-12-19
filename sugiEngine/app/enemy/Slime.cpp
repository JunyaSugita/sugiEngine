#include "Slime.h"
#include "NavePointManager.h"
#include "Player.h"
#include "ParticleManager.h"
#include <random>

void Slime::StaticInitialize()
{
}

void Slime::Initialize(std::string name, Vector3 pos)
{
	life_ = MAX_HP;
	height_ = HEIGHT_COL;

	BaseEnemy::Initialize(name, pos);
	obj_.obj->SetColor({ 0,0.1f,0.1f,0.7f });
	WorldTransUpdate();

}

void Slime::Draw()
{
}

void Slime::Draw2()
{
	//半透明描画
	BaseEnemy::Draw();
}

void Slime::WorldTransUpdate()
{
	col_.col.size = { obj_.scale.x,obj_.scale.y ,obj_.scale.x };
	BaseEnemy::WorldTransUpdate();
}

void Slime::Move()
{
	ColliderManager* colM = ColliderManager::GetInstance();
	NavePointManager* navePointM = NavePointManager::GetInstance();

	if (!isStop_) {
		Vector2 temp;
		//プレイヤー方向に壁が無ければプレイヤー方向に移動
		if (colM->CanMovePlayerVec(obj_.pos)) {
			temp.x = Player::GetInstance()->GetBoxCol().pos.x;
			temp.y = Player::GetInstance()->GetBoxCol().pos.z;

			//起動してなければ起動
			isStart_ = true;
		}
		else {
			int32_t point = colM->CanMoveNavePointVec(obj_.pos);
			//ナビポイントが見つからなければ移動しない
			if (point == -1) {
				return;
			}
			//isStartがfalseなら止まる
			if (!isStart_) {
				return;
			}

			temp.x = navePointM->GetNavePoint(point).pos.x;
			temp.y = navePointM->GetNavePoint(point).pos.z;
		}

		toPlayer = Vector2(temp.x - obj_.pos.x, temp.y - obj_.pos.z);
		toPlayer.normalize();

		obj_.pos.x += toPlayer.x * SPEED_MOVE * slow_;
		obj_.pos.z += toPlayer.y * SPEED_MOVE * slow_;
	}
	else {
		isStop_ = false;
	}

	//最後にスピード減少を初期化
	slow_ = 1.0f;
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

	//スライムの動き
	timer_ += 0.1f;
	obj_.scale.x = 1 + sinf(timer_) * 0.5f;
	obj_.scale.y = 1 + cosf(timer_) * 0.5f;
	obj_.scale.z = 1 + sinf(timer_) * 0.5f;

	WorldTransUpdate();
}

void Slime::Down()
{
	if (obj_.scale.y > 0.2f) {
		obj_.scale.y -= 0.1f;
	}
	if (obj_.scale.x < 10) {
		obj_.scale.x += 0.5f;
		obj_.scale.z += 0.5f;
	}

	//最後
	BaseEnemy::Down();
}

void Slime::DownHitPlayer()
{
	Player::GetInstance()->SetSlow(0.4f);
	if (debuff_.isFire) {
		Player::GetInstance()->SubLife(10);
	}
}

DownState Slime::GetDownHitEnemy()
{
	DownState temp;

	temp.slow = 0.5f;

	if (debuff_.isFire) {
		temp.damage = 1;
	}

	return temp;
}

void Slime::PopDebuffFireParticle()
{
	if (isDown_) {
		//ランダム
		std::random_device seed_gen;
		std::mt19937_64 engine(seed_gen());
		std::uniform_real_distribution<float> x(-7.0f, 7.0f);
		std::uniform_real_distribution<float> z(-7.0f, 7.0f);

		ParticleManager::GetInstance()->AddFromFile(P_DEBUFF_FIRE, { col_.col.pos.x + x(engine), col_.col.pos.y ,col_.col.pos.z + z(engine) });
	}
	else {
		ParticleManager::GetInstance()->AddFromFile(P_DEBUFF_FIRE, col_.col.pos);
	}
}
