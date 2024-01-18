#include "Wall.h"
#include "ColliderManager.h"

void Wall::Initialize(Vector3 pos, Vector3 scale)
{
	//モデルを指定して3Dオブジェクトを生成
	wallObj_.Initialize("ground");

	//obj情報
	wallObj_.pos = pos;
	wallObj_.scale = scale;
	wallObj_.Update();

	//当たり判定
	BaseCol::Initialize(wallObj_.pos, wallObj_.scale,WALL);

	//タイリング
	float tilY = 0;
	if (scale.x > scale.y) {
		tilY = scale.x;
	}
	else {

		tilY = scale.z;
	}

	wallObj_.obj->SetTiling({ scale.y,tilY });
	BaseCol::Update(pos,scale);
}

void Wall::Update()
{
	wallObj_.Update();
	BaseCol::Update(wallObj_.pos, wallObj_.scale);
}

void Wall::Draw()
{
	wallObj_.Draw();
}
