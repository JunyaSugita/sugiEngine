#include "Box.h"

void Box::Initialize(Vector2 pos, Vector2 size,Vector4 color)
{
	box_.Initialize(Sprite::LoadTexture("white1x1"));
	box_.SetAnchorPoint(0.5f,0.5f);

	pos_ = pos;
	box_.SetPos(pos_);
	box_.SetSizeX(size.x);

	box_.SetColor(color);
}

void Box::Draw()
{
	box_.Draw();
}
