#include "Enemy.h"
#include "Player.h"
#include "ImGuiManager.h"
#include "EffectManager.h"

std::unique_ptr<Model> Enemy::sEyeModel_;
std::unique_ptr<Model> Enemy::sColModel_;

void Enemy::OneTimeInitialize()
{
	sEyeModel_ = move(Model::LoadFromObj("sphere", true));
	sColModel_ = move(Model::LoadFromObj("box"));
}

void Enemy::Initialize(Vector3 pos)
{
	model_ = move(Model::LoadFromObj("player"));
	obj_ = move(Object3d::Create());
	obj_->SetModel(model_.get());

	eyeObj_ = move(Object3d::Create());
	eyeObj_->SetModel(sEyeModel_.get());

	colObj_ = move(Object3d::Create());
	colObj_->SetModel(sColModel_.get());

	pos_ = pos;
	rot_ = { 0,90,0 };
	scale_ = { 1,1,1 };

	boxCol_.pos = pos;
	boxCol_.height = 2.2f;
	boxCol_.width = 1.0f;

	isDead_ = false;

	eyeWorldTrans_.parent_ = &worldTrans_;
	eyePos_ = { 0.3f,4.1f,0 };
	eyeRot_ = { 0,0,0 };
	eyeScale_ = { 0.3f,0.3f,0.3f };

	WorldTransUpdate();

	isHit_ = false;
	life_ = MAX_HP;
}

void Enemy::Update()
{
	//プレイヤー情報の取得
	GetPlayer();

	//デバフの適応
	UpdateDebuff();

	//動けるかどうか
	if (isCanMove()) {
		//移動
		Move();

		//プレイヤーの方へゆっくり回転
		SetAngleToPlayer();

	}
	//当たり判定移動
	SetCol();

	//移動を適応
	WorldTransUpdate();
}

void Enemy::Draw()
{
	obj_->Draw();
	eyeObj_->Draw();
	if (ColliderManager::GetInstance()->GetIsShowHitBox()) {
		colObj_->Draw();
	}
}

void Enemy::SetIsHit(int32_t subLife, int32_t effectNum)
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

void Enemy::SetDebuff(uint8_t debuff, uint32_t time)
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
	default:
		break;
	}


	if (debuff_.isFire) {
		obj_->SetColor({ 1, 0, 0, 1 });
	}
	else if (debuff_.isThunder) {
		obj_->SetColor({ 1, 0, 1, 1 });
	}
	else {
		obj_->SetColor({ 1, 1, 1, 1 });
	}
}

bool Enemy::isDebuff()
{
	if (debuff_.isFire || debuff_.isThunder) {
		return true;
	}

	return false;
}

bool Enemy::isCanMove()
{
	if (debuff_.isThunder) {
		return false;
	}

	return true;
}

void Enemy::SetCol()
{
	boxCol_.pos = pos_;
	boxCol_.pos.y += 2.3f;
}

void Enemy::WorldTransUpdate()
{
	worldTrans_.SetPos(pos_);
	worldTrans_.SetRot(rot_);
	worldTrans_.SetScale(scale_);

	eyeWorldTrans_.SetPos(eyePos_);
	eyeWorldTrans_.SetRot(eyeRot_);
	eyeWorldTrans_.SetScale(eyeScale_);

	colWorldTrans_.SetPos(boxCol_.pos);
	colWorldTrans_.SetScale({ boxCol_.width,boxCol_.height,boxCol_.width });

	SetWorldTrans();
}

void Enemy::SetWorldTrans()
{
	obj_->SetWorldTransform(worldTrans_);
	obj_->Update();
	eyeObj_->SetWorldTransform(eyeWorldTrans_);
	eyeObj_->Update();
	colObj_->SetWorldTransform(colWorldTrans_);
	colObj_->Update();
}

void Enemy::SetAngleToPlayer()
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
		rot_.y += SPEED_ANGLE;
		if (rot_.y - angle_ > 0) {
			rot_.y = angle_;
		}
	}
	else {
		rot_.y -= SPEED_ANGLE;
		if (rot_.y - angle_ < 0) {
			rot_.y = angle_;
		}
	}
}

void Enemy::GetPlayer()
{
	//インスタンス取得
	Player* player = Player::GetInstance()->GetInstance();

	toPlayer = Vector2((player->GetPos() - pos_).x, (player->GetPos() - pos_).z);

	//プレイヤーに近づいたら死ぬ
	if (toPlayer.length() <= 1.5f) {
		isDead_ = true;
		player->SubLife();
	}
}

void Enemy::Move()
{
	toPlayer.normalize();

	pos_.x += toPlayer.x * SPEED_MOVE;
	pos_.z += toPlayer.y * SPEED_MOVE;
}

void Enemy::SubLife(int32_t subLife, int32_t effectNum)
{
	life_ -= subLife;
	EffectManager::GetInstance()->BurstGenerate({ pos_.x,pos_.y + 4,pos_.z }, effectNum, { 1,0,0,1 });
	if (life_ <= 0) {
		isDead_ = true;
	}
}

void Enemy::UpdateDebuff()
{
	if (isDebuff()) {
		if (debuff_.isFire) {
			if (debuff_.fireTime % (3 * 60) == 0) {
				life_ -= 5;
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
	}
	else {
		obj_->SetColor({1,1,1,1});
	}

	if (life_ <= 0) {
		isDead_ = true;
	}
}
