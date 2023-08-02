#include "Closshair.h"
#include "Input.h"

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
	Input* input = Input::GetInstance();

	if (input->GetLTrigger() > 50) {
		clossSprite_.SetPos(WIN_WIDTH / 2 + input->GetRStickX() / 500.0f, WIN_HEIGHT / 2 + (input->GetRStickY() / 500.0f) * -1);
	}
	else {
		clossSprite_.SetPos(WIN_WIDTH / 2, WIN_HEIGHT / 2);
	}
}

void Closshair::Draw()
{
	clossSprite_.Draw();
}
