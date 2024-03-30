#include "BulletManager.h"

BulletManager* BulletManager::GetInstance()
{
	//インスタンス生成
	static BulletManager instance;

	return &instance;
}
