#include "Gauge.h"

void Gauge::Set(Vector2 pos,Vector2 size)
{
	maxTex_ = Sprite::LoadTexture("ChargeGauge.png");
	nowTex_ = Sprite::LoadTexture("white1x1.png");

	maxSp_.Initialize(maxTex_);
	nowSp_.Initialize(nowTex_);

	maxSp_.SetAnchorPoint(0, 0.5f);
	nowSp_.SetAnchorPoint(0, 0.5f);

	maxSp_.SetPos(pos.x - size.x / 2, pos.y - size.y / 2);
	nowSp_.SetPos(pos.x - size.x / 2, pos.y - size.y / 2);

	size_ = size;
	showSize_ = size_;

	maxSp_.SetSize(size_.x, size_.y);
	nowSp_.SetSize(size_.x, size_.y);

	nowSp_.SetColor(0.6f,0,0,1);

	timer_ = 1;
}

void Gauge::Update(float max, float now)
{
	nowSize_ = size_ * now / max;

	nowSp_.SetSizeX(nowSize_.x + EaseOut(timer_, showSize_.x - nowSize_.x));

	if (nowSize_.x < showSize_.x) {
		if (timer_ > 0) {
			timer_ -= 0.05f;
		}
		else {
			showSize_ = nowSize_;
		}
	}
	else {
		timer_ = 1;
	}
}

void Gauge::Draw()
{
	nowSp_.Draw();
	maxSp_.Draw();
}
