#include "BaseEnemy.h"
#include "SpellManager.h"
#include "Player.h"
#include "EffectManager.h"
#include "ParticleManager.h"
#include "Tutorial.h"
#include "Enemy.h"

#include <random>

std::unique_ptr<Model> BaseEnemy::sColModel_;
std::unique_ptr<Model> BaseEnemy::sModel_;
bool BaseEnemy::sIsDebugStop_ = true;

void BaseEnemy::StaticInitialize()
{
	sColModel_ = move(Model::LoadFromObj("box"));
}

void BaseEnemy::Initialize(Vector3 pos)
{
	//モデルデータは先に派生クラスで読み込む
	obj_ = move(Object3d::Create());
	obj_->SetModel(sModel_.get());
	colObj_ = move(Object3d::Create());
	colObj_->SetModel(sColModel_.get());

	//位置
	pos_ = pos;
	rot_ = { 0,90,0 };
	scale_ = { 1,1,1 };

	//当たり判定
	boxCol_.pos = pos;
	boxCol_.size = { 1.0f,height_,1.0f };
	oldBoxCol_.pos = pos;
	oldBoxCol_.size = { 1.0f,height_,1.0f };

	//フラグ
	isDead_ = false;
	isHit_ = false;
	isStop_ = false;
	isAttack_ = false;
	attackTimer_ = 0.0f;

	//チュートリアル
	if (Tutorial::GetInstance()->GetIsTutorial()) {
		sIsDebugStop_ = true;
	}
	else {
		sIsDebugStop_ = false;
	}
}

void BaseEnemy::Update()
{
	//1フレ前の座標を保存
	oldBoxCol_ = boxCol_;

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

	//移動を適応
	WorldTransUpdate();
}

void BaseEnemy::Draw()
{
	obj_->Draw();
	if (ColliderManager::GetInstance()->GetIsShowHitBox()) {
		colObj_->Draw();
	}
}

void BaseEnemy::WorldTransUpdate()
{
	worldTrans_.SetPos(pos_);
	worldTrans_.SetRot(rot_);
	worldTrans_.SetScale(scale_);

	colWorldTrans_.SetPos(boxCol_.pos);
	colWorldTrans_.SetScale(boxCol_.size);

	SetWorldTrans();
}

void BaseEnemy::SetDebuff(uint8_t debuff, uint32_t time)
{
	switch (debuff)
	{
	case FIRE:
		debuff_.isFire = true;
		debuff_.fireTime = time * 60;
		break;
	case THUNDER:
		debuff_.isThunder = true;
		debuff_.thunderTime = time * 60;
		break;
	case ICE:
		debuff_.isIce = true;
		debuff_.iceTime = time * 60;
		break;
	default:
		break;
	}


	if (debuff_.isFire) {
		obj_->SetColor({ 1, 0, 0, 1 });
	}
	else if (debuff_.isThunder) {
		obj_->SetColor({ 1, 0, 1, 1 });
	}
	else if (debuff_.isIce) {
		obj_->SetColor({ 0, 0.3f, 1, 1 });
	}
	else {
		obj_->SetColor({ 1, 1, 1, 1 });
	}
}

void BaseEnemy::SetIsHit(int32_t subLife, int32_t effectNum)
{
	//既に当たっていたら当たらない
	if (isHit_) {
		return;
	}
	else {
		//当たったフラグを立てる
		isHit_ = true;
		//体力を削る
		SubLife(subLife, effectNum);
	}
}

bool BaseEnemy::isDebuff()
{
	if (debuff_.isFire || debuff_.isThunder || debuff_.isIce) {
		return true;
	}

	return false;
}

bool BaseEnemy::isCanMove()
{
	if (debuff_.isThunder) {
		return false;
	}

	return true;
}

void BaseEnemy::ResetShake()
{
	pos_.x = boxCol_.pos.x;
	pos_.z = boxCol_.pos.z;
}

void BaseEnemy::SetIsAttack()
{
	if (!isAttack_) {
		attackTimer_ = 1.0f;
		Player::GetInstance()->SubLife();
	}
	isAttack_ = true;
}

void BaseEnemy::SetWorldTrans()
{
	obj_->SetWorldTransform(worldTrans_);
	obj_->Update();
	colObj_->SetWorldTransform(colWorldTrans_);
	colObj_->Update();
}

void BaseEnemy::SetAngleToPlayer()
{
	//インスタンス取得
	Player* player = Player::GetInstance()->GetInstance();

	//プレイヤーの向きを計算して単位化
	Vector2 len = Vector2((player->GetPos() - pos_).x, (player->GetPos() - pos_).z);
	len.normalize();

	//プレイヤーの方にゆっくり向く
	float angle_ = atan2(len.cross(UP), -len.dot(UP)) / PI * -RADIAN - (RADIAN / 2);
	while (rot_.y - angle_ > RADIAN || rot_.y - angle_ < -RADIAN) {
		if (rot_.y - angle_ > RADIAN) {
			rot_.y -= RADIAN * 2;
		}
		if (rot_.y - angle_ < -RADIAN) {
			rot_.y += RADIAN * 2;
		}
	}
	if (rot_.y - angle_ < 0) {
		rot_.y += angleSpeed_ * GetSlow();
		if (rot_.y - angle_ > 0) {
			rot_.y = angle_;
		}
	}
	else {
		rot_.y -= angleSpeed_ * GetSlow();
		if (rot_.y - angle_ < 0) {
			rot_.y = angle_;
		}
	}
}

float BaseEnemy::GetSlow()
{
	if (debuff_.isIce) {
		return 0.5;
	}
	return 1.0f;
}

void BaseEnemy::SubLife(int32_t subLife, int32_t effectNum)
{
	life_ -= subLife;
	EffectManager::GetInstance()->BurstGenerate({ pos_.x,pos_.y + 4,pos_.z }, effectNum, { 1,0,0,1 });
	if (life_ <= 0) {
		isDead_ = true;
	}
}

void BaseEnemy::UpdateDebuff()
{
	if (isDebuff()) {
		if (debuff_.isFire) {
			ParticleManager::GetInstance()->AddFromFile(P_DEBUFF_FIRE, colObj_->GetPos());
			if (debuff_.fireTime % 40 == 1) {
				SubLife(1, 0);
			}


			if (--debuff_.fireTime < 0) {
				debuff_.isFire = false;
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
	else {
		obj_->SetColor({ 1,1,1,1 });
	}
}

void BaseEnemy::SetShake()
{
	//ランダム
	std::random_device seed_gen;
	std::mt19937_64 engine(seed_gen());

	//カメラとの距離でシェイクの大きさを変える
	Vector3 len = Camera::GetInstance()->GetEye() - pos_;
	float maxShake = len.length() / 100;
	if (maxShake > 0.5f) {
		maxShake = 0.5f;
	}

	std::uniform_real_distribution<float> x(-maxShake, maxShake);
	std::uniform_real_distribution<float> z(-maxShake, maxShake);

	pos_.x += x(engine);
	pos_.z += z(engine);
}

void BaseEnemy::SetCol()
{
	boxCol_.pos = pos_;
	boxCol_.pos.y += height_;
}
