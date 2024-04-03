#include "Bullet.h"
#include "Player.h"
#include "ColliderManager.h"
#include "ParticleManager.h"

void Bullet::Initialize(Vector3 pos)
{
	//モデルデータは先に派生クラスで読み込む
	obj_.Initialize("sphere");
	//位置を初期化
	obj_.pos = pos;
	//当たり判定登録
	BaseCol::Initialize(obj_.pos, col_.size, BULLET, gap_);

	//プレイヤー方向に飛ばすためのベクトル計算
	vec_ = Player::GetInstance()->GetPos() - Vector3(pos.x,pos.y + 1,pos.z);
	//ベクトルの単位化
	vec_.normalize();

	//ヒット判定を初期化
	isHit_ = false;
}

void Bullet::Update()
{
	//場所をSPEED分進める
	obj_.pos += vec_ * SPEED;
	obj_.Update();

	//当たり判定の更新
	SetCol(obj_.pos, col_.size);
	colObj_.Update();

	//パーティクル放出
	ParticleManager::GetInstance()->AddFromFile(P_MAGIC_MISSILE, obj_.pos);
	ParticleManager::GetInstance()->AddFromFile(P_MAGIC_MISSILE2, obj_.pos);
}

void Bullet::Draw()
{
	obj_.Draw();
}