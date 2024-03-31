#pragma once
#include <vector>
#include "Bullet.h"

class BulletManager
{
private:
	BulletManager() = default;
	~BulletManager() = default;

public:
	//コピーコンストラクタ無効
	BulletManager(const BulletManager& obj) = delete;
	//代入演算子を無効
	BulletManager& operator=(const BulletManager& obj) = delete;

	static BulletManager* GetInstance();

public:
	void Initialize();
	void Update();
	void Draw();

	void PopBullet(Vector3 pos);

private:
	std::vector<Bullet*> bullets_;
};

