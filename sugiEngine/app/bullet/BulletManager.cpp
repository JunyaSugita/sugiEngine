#include "BulletManager.h"

BulletManager* BulletManager::GetInstance()
{
	//インスタンス生成
	static BulletManager instance;

	return &instance;
}

void BulletManager::Initialize()
{
	//コンテナクリア
	bullets_.clear();
}

void BulletManager::Update()
{
	//既に当たっている弾をコンテナから削除
	for (int i = 0; i < bullets_.size(); i++) {
		if (bullets_[i]->GetIsDead()) {
			bullets_.erase(bullets_.begin() + i);
		}
	}

	//全ての弾の更新
	for (int i = 0;i < bullets_.size();i++) {
		bullets_[i]->Update();
	}
}

void BulletManager::Draw()
{
	for (int i = 0; i < bullets_.size(); i++) {
		bullets_[i]->Draw();
	}
}

void BulletManager::PopBullet(Vector3 pos)
{
	//弾の生成
	Bullet* newBullet = new Bullet;
	newBullet->Initialize(pos);
	bullets_.push_back(newBullet);
}
