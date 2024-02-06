//敵のベースクラス

#include "BaseEnemy.h"
#include "SpellManager.h"
#include "Player.h"
#include "EffectManager.h"
#include "ParticleManager.h"
#include "Tutorial.h"
#include "Enemy.h"
#include "ModelManager.h"
#include "ColliderManager.h"
#include "ClearChecker.h"

#include <random>

bool BaseEnemy::sIsDebugStop_ = true;
LightGroup* BaseEnemy::light_ = nullptr;

void BaseEnemy::Initialize(const std::string& name, const Vector3& pos)
{
	//モデルデータは先に派生クラスで読み込む
	obj_.Initialize(name);
	BaseCol::Initialize(obj_.pos, col_.size, ENEMY, gap_);

	//位置
	obj_.pos = pos;
	obj_.rot = START_ROT;
	obj_.obj->SetIsSimple();

	//デバフの氷
	iceObj_.Initialize("iceBlock");
	iceObj_.pos = { 0,2,0 };
	iceObj_.scale = { 2,3,2 };
	iceObj_.obj->SetColor(COLOR_ICE);

	//フラグ
	isDead_ = false;
	isDown_ = false;
	downTimer_ = TIME_DOWN;
	isHit_ = false;
	isStop_ = false;
	isAttack_ = false;
	attackTimer_ = 0.0f;
	lightNum_ = -1;
	slow_ = 1;
	shakeTime_ = 0;
	isStart_ = false;

	//移動適応
	WorldTransUpdate();
	BaseCol::SetCol(obj_.pos, col_.size);
}

void BaseEnemy::Update()
{
	//最初の1フレーム目で敵の体力を記録
	if (!isFirstFrame_) {
		ClearChecker::GetInstance()->AddMaxHp(life_);
		isFirstFrame_ = true;
	}

	//シェイクを戻す
	ResetShake();
	//1フレ前の座標を保存
	SetOldCol();

	//倒れている時の処理
	if (isDown_) {
		Down();
		//弱点の体変更
		WeakBodyColor();
		//移動を適応
		WorldTransUpdate();

		return;
	}

	//デバフの適応
	UpdateDebuff();

	//弱点の体変更
	WeakBodyColor();

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

		BaseCol::Update(obj_.pos, col_.size);
	}
	else
	{
		DontMoveUpdate();
		if (debuff_.isThunder || debuff_.isIce) {
			SetShake();
		}
	}

	if (shakeTime_ > 0) {
		SetShake();
		shakeTime_--;
	}
	ClearChecker::GetInstance()->AddNowHp(life_);

	//移動を適応
	WorldTransUpdate();
}

void BaseEnemy::DontMoveUpdate()
{

}

void BaseEnemy::Draw()
{
	obj_.Draw();
}

void BaseEnemy::DrawTransparent()
{
	if (debuff_.isIce && !isDown_) {
		iceObj_.Draw();
	}
}

void BaseEnemy::WorldTransUpdate()
{
	obj_.Update();
	iceObj_.Update();
}

void BaseEnemy::OnCollision(BaseCol* a)
{
	if (a->GetColType() == PLAYER) {
		if (ColliderManager::GetInstance()->CheckHitCircle(GetCol(), a->GetCol())) {
			if (!GetIsDown()) {
				SetIsStop();
				SetIsAttack();
			}
			else {
				DownHitPlayer();
			}
		}
	}
	else if (a->GetColType() == SPELL) {
		//まだ当たってない呪文と倒れている敵は判定しない
		if (!a->GetIsHit() && GetIsDown()) {
			return;
		}
		if (ColliderManager::GetInstance()->CheckHitBox(a->GetCol(), GetCol())) {
			if (!a->GetIsHit()) {
				//敵に当たった瞬間
				ParticleManager::GetInstance()->AddFromFile(P_DAMAGE, a->GetPos());
				SetShakeTime(TIME_SHAKE);
			}
			SubLife(a->GetDamage());
			SetDebuff(a->GetDebuff(), DEBUFF_TIME);
		}
	}
	else if (a->GetColType() == WALL) {
		if (ColliderManager::GetInstance()->CheckHitBox(GetCol(), a->GetCol())) {
			//X押し戻し
			if (ColliderManager::GetInstance()->CheckHitX(GetCol(), a->GetCol())) {
				if (!ColliderManager::GetInstance()->CheckHitX(GetOldCol(), a->GetOldCol())) {
					SetColX(GetOldCol().pos.x);
					HitChangePos();
				}
			}
			//Y押し戻し
			if (ColliderManager::GetInstance()->CheckHitY(GetCol(), a->GetCol())) {
				if (!ColliderManager::GetInstance()->CheckHitY(GetOldCol(), a->GetOldCol())) {
					SetColY(GetOldCol().pos.y);
					HitChangePos();
				}
			}
			//Z押し戻し
			if (ColliderManager::GetInstance()->CheckHitZ(GetCol(), a->GetCol())) {
				if (!ColliderManager::GetInstance()->CheckHitZ(GetOldCol(), a->GetOldCol())) {
					SetColZ(GetOldCol().pos.z);
					HitChangePos();
				}
			}
		}
	}
	else if (a->GetColType() == ENEMY) {
		if (!GetIsDown() && !a->GetIsDown()) {
			if (ColliderManager::GetInstance()->CheckHitCircle(GetCol(), a->GetCol())) {
				if (GetCol().pos.x <= a->GetCol().pos.x) {
					AddColX(-PUSH_LEN);
				}
				else if (GetCol().pos.x > a->GetCol().pos.x) {
					AddColX(PUSH_LEN);
				}

				if (GetCol().pos.z <= a->GetCol().pos.z) {
					AddColZ(-PUSH_LEN);
				}
				else if (GetCol().pos.z > a->GetCol().pos.z) {
					AddColZ(PUSH_LEN);
				}
			}
		}
		//相手のみが倒れてる
		else if (!GetIsDown() && a->GetIsDown()) {
			if (ColliderManager::GetInstance()->CheckHitCircle(GetCol(), a->GetCol())) {
				SetDownHitEnemy(a->GetDownHitEnemy());
			}
		}
	}
}

void BaseEnemy::DownHitPlayer()
{

}

DownState BaseEnemy::GetDownHitEnemy()
{
	//生き残っている敵に与える影響
	return DownState();
}

void BaseEnemy::SetDownHitEnemy(const DownState& state)
{
	//死んでいるやつから貰う影響
	SetSlow(state.slow);
	SubLife(state.damage);
}

void BaseEnemy::WeakBodyColor()
{

}

void BaseEnemy::SetDebuff(int32_t debuff, int32_t time)
{
	switch (debuff)
	{
	case D_FIRE:
		debuff_.isFire = true;
		debuff_.fireTime = time * MAX_FRAME;
		if (lightNum_ == -1) {
			lightNum_ = light_->SetPointLightGetNum();
			light_->SetPointLightAtten(lightNum_, LIGHT_ATTEN);
			light_->SetPointLightColor(lightNum_, COLOR_FIRE);
			//少し上にライトを置く
			light_->SetPointLightPos(lightNum_, { obj_.pos.x,obj_.pos.y + 1 ,obj_.pos.z });
		}

		break;
	case D_STAN:
		debuff_.isThunder = true;
		debuff_.thunderTime = time * MAX_FRAME;
		break;
	case D_SLOW:
		debuff_.isIce = true;
		debuff_.iceTime = time * MAX_FRAME;
		break;
	default:
		break;
	}
}

void BaseEnemy::SetIsHit(float subLife, bool isParticle)
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
	if (debuff_.isThunder || debuff_.isIce || (sIsDebugStop_ && !Tutorial::GetInstance()->GetIsTutorial())) {
		return false;
	}

	return true;
}

void BaseEnemy::ResetShake()
{
	obj_.pos = col_.pos;
	obj_.pos.y = 0;
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
	return slow_;
}

void BaseEnemy::SubLife(float subLife, bool isParticle)
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
			if (debuff_.fireTime % TIME_FIRE_BURN == 1) {
				SubLife(DAMAGE_FIRE_BURN);
			}

			//ランダム
			std::random_device seed_gen;
			std::mt19937_64 engine(seed_gen());

			light_->SetPointLightPos(lightNum_, { obj_.pos.x,obj_.pos.y + 1 ,obj_.pos.z });
			std::uniform_real_distribution<float> atten(MIN_ATTEN_FIRE, MAX_ATTEN_FIRE);
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
			iceObj_.pos = col_.pos;
			iceObj_.rot = iceObj_.rot;

			if (!isDown_) {
				ParticleManager::GetInstance()->AddFromFile(P_DEBUFF_ICE, obj_.pos);
				ParticleManager::GetInstance()->AddFromFile(P_DEBUFF_ICE2, obj_.pos);
			}
			if (--debuff_.iceTime < 0) {
				if (!isDown_) {
					ParticleManager::GetInstance()->AddFromFile(P_ICE_BREAK, obj_.pos);
					ParticleManager::GetInstance()->AddFromFile(P_ICE_BREAK2, obj_.pos);
				}
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
	//適当な数で割る
	float maxShake = len.length() / CALC_SHAKE;
	if (maxShake > MAX_SHAKE) {
		maxShake = MAX_SHAKE;
	}

	std::uniform_real_distribution<float> x(-maxShake, maxShake);
	std::uniform_real_distribution<float> z(-maxShake, maxShake);

	obj_.pos.x += x(engine);
	obj_.pos.z += z(engine);
	iceObj_.pos.x = obj_.pos.x;
	iceObj_.pos.z = obj_.pos.z;
}

void BaseEnemy::HitChangePos()
{
	ResetShake();
	WorldTransUpdate();
}

void BaseEnemy::Dead()
{
	isDead_ = true;
	ColliderManager::GetInstance()->DeleteCollider(this);
}

void BaseEnemy::SetCol()
{
	SetCol(obj_.pos);
}

void BaseEnemy::PopDebuffFireParticle()
{
	ParticleManager::GetInstance()->AddFromFile(P_DEBUFF_FIRE, col_.pos);
	ParticleManager::GetInstance()->AddFromFile(P_DEBUFF_FIRE2, col_.pos);
}

void BaseEnemy::Down()
{
	//デバフの適応
	UpdateDebuff();

	if (debuff_.fireTime == 1) {
		Dead();
		light_->SetPointLightActive(lightNum_, false);
	}
}
