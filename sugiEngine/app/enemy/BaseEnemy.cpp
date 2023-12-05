//敵のベースクラス

#include "BaseEnemy.h"
#include "SpellManager.h"
#include "Player.h"
#include "EffectManager.h"
#include "ParticleManager.h"
#include "Tutorial.h"
#include "Enemy.h"
#include "ModelManager.h"

#include <random>

bool BaseEnemy::sIsDebugStop_ = true;
LightGroup* BaseEnemy::light_ = nullptr;

void BaseEnemy::Initialize(std::string name, Vector3 pos)
{
	//モデルデータは先に派生クラスで読み込む
	obj_.Initialize(name);
	col_.Initialize();

	//位置
	obj_.pos = pos;
	obj_.rot = { 0,90,0 };
	obj_.scale = { 1,1,1 };
	obj_.obj->SetIsSimple();

	//当たり判定
	col_.col.pos = pos;
	col_.col.size = { 1.0f,height_,1.0f };
	col_.gap = {0,height_,0};

	//フラグ
	isDead_ = false;
	isDown_ = false;
	downTimer_ = TIME_DOWN;
	isHit_ = false;
	isStop_ = false;
	isAttack_ = false;
	attackTimer_ = 0.0f;
	lightNum_ = -1;
	slow_ = 1.0f;
	shakeTime_ = 0;
	isStart_ = false;
}

void BaseEnemy::Update()
{
	//倒れている時の処理
	if (isDown_) {
		Down();
		//移動を適応
		WorldTransUpdate();

		return;
	}

	//1フレ前の座標を保存
	col_.SetOldCol();

	//シェイクを戻す
	ResetShake();

	//デバフの適応
	UpdateDebuff();

	//動けるかどうか
	if (isCanMove()) {
		if (!isAttack_ && !sIsDebugStop_) {
			//移動
			Move();
		}

		//攻撃
		Attack();

		//プレイヤーの方へゆっくり回転
		SetAngleToPlayer();

		//当たり判定移動
		SetCol();
	}
	else
	{
		SetShake();
	}

	if (shakeTime_ > 0) {
		SetShake();
		shakeTime_--;
	}

	//移動を適応
	WorldTransUpdate();
}

void BaseEnemy::Draw()
{
	obj_.Draw();
	col_.Draw();
}

void BaseEnemy::WorldTransUpdate()
{
	obj_.Update();
	col_.Update();
}

void BaseEnemy::DownHitPlayer()
{

}

DownState BaseEnemy::GetDownHitEnemy()
{
	//生き残っている敵に与える影響
	return DownState();
}

void BaseEnemy::SetDownHitEnemy(DownState state)
{
	//死んでいるやつから貰う影響
	SetSlow(state.slow);
	SubLife(state.damage);
}

void BaseEnemy::SetDebuff(int32_t debuff, int32_t time)
{
	switch (debuff)
	{
	case D_FIRE:
		debuff_.isFire = true;
		debuff_.fireTime = time * 60;
		if (lightNum_ == -1) {
			lightNum_ = light_->SetPointLightGetNum();
			light_->SetPointLightAtten(lightNum_, { 0.005f,0.005f,0.005f });
			light_->SetPointLightColor(lightNum_, { 0.5f,0.2f,0 });
			light_->SetPointLightPos(lightNum_, { obj_.pos.x,obj_.pos.y + 1 ,obj_.pos.z });
		}
		
		break;
	case D_STAN:
		debuff_.isThunder = true;
		debuff_.thunderTime = time * 60;
		break;
	case D_SLOW:
		debuff_.isIce = true;
		debuff_.iceTime = time * 60;
		break;
	default:
		break;
	}
}

void BaseEnemy::SetIsHit(int32_t subLife,bool isParticle)
{
	//既に当たっていたら当たらない
	if (isHit_) {
		return;
	}
	else {
		//当たったフラグを立てる
		isHit_ = true;
		//体力を削る
		SubLife(subLife, isParticle);
	}
}

bool BaseEnemy::isDebuff()
{
	//デバフ状態ならtrueを返す
	if (debuff_.isFire || debuff_.isThunder || debuff_.isIce) {
		return true;
	}

	return false;
}

bool BaseEnemy::isCanMove()
{
	//動けなくなる状態ならfalseを返す
	if (debuff_.isThunder) {
		return false;
	}

	return true;
}

void BaseEnemy::ResetShake()
{
	obj_.pos.x = col_.col.pos.x;
	obj_.pos.z = col_.col.pos.z;
}

void BaseEnemy::SetIsAttack()
{
	if (!isAttack_) {
		attackTimer_ = 1.0f;
		Player::GetInstance()->SubLife();
	}
	isAttack_ = true;
}

void BaseEnemy::SetAngleToPlayer()
{
	//インスタンス取得
	Player* player = Player::GetInstance()->GetInstance();

	//プレイヤーの向きを計算して単位化
	Vector2 len = Vector2((player->GetPos() - obj_.pos).x, (player->GetPos() - obj_.pos).z);
	len.normalize();

	//プレイヤーの方にゆっくり向く
	float angle_ = atan2(len.cross(UP), -len.dot(UP)) / PI * -RADIAN - (RADIAN / 2);
	while (obj_.rot.y - angle_ > RADIAN || obj_.rot.y - angle_ < -RADIAN) {
		if (obj_.rot.y - angle_ > RADIAN) {
			obj_.rot.y -= RADIAN * 2;
		}
		if (obj_.rot.y - angle_ < -RADIAN) {
			obj_.rot.y += RADIAN * 2;
		}
	}
	if (obj_.rot.y - angle_ < 0) {
		obj_.rot.y += angleSpeed_ * GetSlow();
		if (obj_.rot.y - angle_ > 0) {
			obj_.rot.y = angle_;
		}
	}
	else {
		obj_.rot.y -= angleSpeed_ * GetSlow();
		if (obj_.rot.y - angle_ < 0) {
			obj_.rot.y = angle_;
		}
	}
}

float BaseEnemy::GetSlow()
{
	if (debuff_.isIce) {
		return slow_ * 0.5f;
	}
	return slow_;
}

void BaseEnemy::SubLife(int32_t subLife, bool isParticle)
{
	life_ -= subLife;
	if (life_ < 0) {
		life_ = 0;
	}
	if (isParticle) {
		ParticleManager::GetInstance()->AddFromFile(P_DAMAGE, { obj_.pos.x,obj_.pos.y,obj_.pos.z });
	}
	if (life_ <= 0) {
		isDown_ = true;
	}
}

void BaseEnemy::UpdateDebuff()
{
	if (isDebuff()) {
		if (debuff_.isFire) {
			PopDebuffFireParticle();
			if (debuff_.fireTime % 40 == 1) {
				SubLife(1);
			}

			//ランダム
			std::random_device seed_gen;
			std::mt19937_64 engine(seed_gen());

			light_->SetPointLightPos(lightNum_, { obj_.pos.x,obj_.pos.y + 1 ,obj_.pos.z});
			std::uniform_real_distribution<float> atten(0.004f, 0.006f);
			light_->SetPointLightAtten(lightNum_, { atten(engine),atten(engine),atten(engine) });

			if (--debuff_.fireTime < 0) {
				debuff_.isFire = false;
				light_->SetPointLightActive(lightNum_, false);
				lightNum_ = -1;
			}
		}
		if (debuff_.isThunder) {
			if (--debuff_.thunderTime < 0) {
				debuff_.isThunder = false;
			}
		}
		if (debuff_.isIce) {
			if (--debuff_.iceTime < 0) {
				debuff_.isIce = false;
			}
		}
	}
}

void BaseEnemy::SetShake()
{
	//ランダム
	std::random_device seed_gen;
	std::mt19937_64 engine(seed_gen());

	//カメラとの距離でシェイクの大きさを変える
	Vector3 len = Camera::GetInstance()->GetEye() - obj_.pos;
	float maxShake = len.length() / 100;
	if (maxShake > 0.5f) {
		maxShake = 0.5f;
	}

	std::uniform_real_distribution<float> x(-maxShake, maxShake);
	std::uniform_real_distribution<float> z(-maxShake, maxShake);

	obj_.pos.x += x(engine);
	obj_.pos.z += z(engine);
}

void BaseEnemy::SetCol()
{
	col_.SetCol(obj_.pos);
}

void BaseEnemy::PopDebuffFireParticle()
{
	ParticleManager::GetInstance()->AddFromFile(P_DEBUFF_FIRE, col_.col.pos);
}

void BaseEnemy::Down()
{
	//デバフの適応
	UpdateDebuff();

	if (debuff_.fireTime == 1) {
		isDead_ = true;
		light_->SetPointLightActive(lightNum_, false);
	}
}
