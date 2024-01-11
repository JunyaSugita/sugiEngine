#include "Slime.h"
#include "NaviPointManager.h"
#include "Player.h"
#include "ParticleManager.h"
#include <random>

void Slime::Initialize(std::string name, Vector3 pos)
{
	life_ = MAX_HP;
	height_ = HEIGHT_COL;

	BaseEnemy::Initialize(name, pos);
	obj_.obj->SetColor(COLOR_BODY);
	WorldTransUpdate();

}

void Slime::Draw()
{
}

void Slime::DrawTransparent()
{
	//半透明描画
	BaseEnemy::Draw();
	BaseEnemy::DrawTransparent();
}

void Slime::WorldTransUpdate()
{
	cols_.col.size = { obj_.scale.x,obj_.scale.y ,obj_.scale.x };
	BaseEnemy::WorldTransUpdate();
}

void Slime::Move()
{
	ColliderManager* colM = ColliderManager::GetInstance();
	NaviPointManager* navePointM = NaviPointManager::GetInstance();

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
			int32_t point = colM->CanMoveNaviPointVec(obj_.pos);
			//ナビポイントが見つからなければ移動しない
			if (point == -1) {
				return;
			}
			//isStartがfalseなら止まる
			if (!isStart_) {
				return;
			}

			temp.x = navePointM->GetNaviPoint(point).pos.x;
			temp.y = navePointM->GetNaviPoint(point).pos.z;
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
	slow_ = 1;
}

void Slime::Attack()
{
	if (isAttack_) {
		attackTimer_ -= SUB_ATTACK_TIMER;

		timer_ += SPEED_ANIME_TIMER;
		obj_.scale.x = 1 + sinf(timer_) * SAVE_ANIME;
		obj_.scale.y = 1 + cosf(timer_) * SAVE_ANIME;
		obj_.scale.z = 1 + sinf(timer_) * SAVE_ANIME;

		if (attackTimer_ <= 0) {
			isAttack_ = false;
		}
	}

	//スライムの動き
	timer_ += SPEED_ANIME_TIMER;
	obj_.scale.x = 1 + sinf(timer_) * SAVE_ANIME;
	obj_.scale.y = 1 + cosf(timer_) * SAVE_ANIME;
	obj_.scale.z = 1 + sinf(timer_) * SAVE_ANIME;

	WorldTransUpdate();
}

void Slime::Down()
{
	if (obj_.scale.y > MAX_HEIGHT_DOWN) {
		obj_.scale.y -= SPEED_HEIGHT_DOWN;
	}
	if (obj_.scale.x < MAX_WIDTH_DOWN) {
		obj_.scale.x += SAVE_ANIME;
		obj_.scale.z += SAVE_ANIME;
	}

	//最後
	BaseEnemy::Down();
}

void Slime::DownHitPlayer()
{
	Player::GetInstance()->SetSlow(SPEED_DOWN_DEBUFF_TO_PLAYER);
	if (debuff_.isFire) {
		Player::GetInstance()->SubLife(DAMAGE_DOWN_FIRE_TO_PLAYER);
	}
}

DownState Slime::GetDownHitEnemy()
{
	DownState temp;

	temp.slow = SPEED_DOWN_DEBUFF_TO_ENEMY;

	if (debuff_.isFire) {
		temp.damage = DAMAGE_DOWN_FIRE_TO_ENEMY;
	}

	return temp;
}

void Slime::PopDebuffFireParticle()
{
	if (isDown_) {
		//ランダム
		std::random_device seed_gen;
		std::mt19937_64 engine(seed_gen());
		std::uniform_real_distribution<float> x(-RENGE_DOWN_FIELD, RENGE_DOWN_FIELD);
		std::uniform_real_distribution<float> z(-RENGE_DOWN_FIELD, RENGE_DOWN_FIELD);

		ParticleManager::GetInstance()->AddFromFile(P_DEBUFF_FIRE, { cols_.col.pos.x + x(engine), cols_.col.pos.y ,cols_.col.pos.z + z(engine) });
	}
	else {
		ParticleManager::GetInstance()->AddFromFile(P_DEBUFF_FIRE, cols_.col.pos);
	}
}

void Slime::WeakBodyColor()
{
	if (Player::GetInstance()->GetPresetSpell() == MAGIC::FIRE_BALL) {
		obj_.obj->SetColor(COLOR_WEAK_BODY);
		obj_.obj->SetIsBloom();
	}
	else {
		obj_.obj->SetColor(COLOR_BODY);
		obj_.obj->SetIsBloom(false);
	}

	if (isDown_) {
		obj_.obj->SetColor(COLOR_BODY);
		obj_.obj->SetIsBloom(false);
	}
}
