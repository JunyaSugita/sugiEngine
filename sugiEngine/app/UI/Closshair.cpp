#include "Closshair.h"

Closshair::Closshair()
{
}

Closshair::~Closshair()
{
}

Closshair* Closshair::GetInstance()
{
	static Closshair instance;

	return &instance;
}

void Closshair::Initialize()
{
	clossTex_ = Sprite::LoadTexture("closshair.png");
	clossSprite_.Initialize(clossTex_);
	clossSprite_.SetAnchorPoint(0.5f,0.5f);
	clossSprite_.SetPos(WIN_WIDTH / 2, WIN_HEIGHT / 2);
}

void Closshair::Update()
{
}

void Closshair::Draw()
{
	clossSprite_.Draw();
}
