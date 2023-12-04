#include "Box.h"

void Box::Initialize(Vector2 pos, Vector2 size,Vector4 color)
{
	box_.Initialize(Sprite::LoadTexture("white1x1.png"));
	box_.SetAnchorPoint(0.5f,0.5f);

	pos_ = pos;
	box_.SetPos(pos_);
	box_.SetSize(size);

	box_.SetColor(color);
}

void Box::Update()
{
	box_.SetPos(pos_);
}

void Box::Draw()
{
	box_.Draw();
}
