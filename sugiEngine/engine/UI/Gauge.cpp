#include "Gauge.h"

void Gauge::Set(Vector2 pos, Vector2 size, Vector3 color)
{
	maxTex_ = Sprite::LoadTexture("ChargeGauge","png");
	nowTex_ = Sprite::LoadTexture("white1x1","png");

	maxSp_.Initialize(maxTex_);
	easingSp_.Initialize(nowTex_);
	nowSp_.Initialize(nowTex_);
	HealSp_.Initialize(nowTex_);

	maxSp_.SetAnchorPoint(0, 0.5f);
	easingSp_.SetAnchorPoint(0, 0.5f);
	nowSp_.SetAnchorPoint(0, 0.5f);
	HealSp_.SetAnchorPoint(0, 0.5f);

	maxSp_.SetPos(pos.x - size.x / 2, pos.y - size.y / 2);
	easingSp_.SetPos(pos.x - size.x / 2, pos.y - size.y / 2);
	nowSp_.SetPos(pos.x - size.x / 2, pos.y - size.y / 2);
	HealSp_.SetPos(pos.x - size.x / 2, pos.y - size.y / 2);

	size_ = size;
	showSize_ = size_;
	healSize_ = size_;

	maxSp_.SetSize(size_.x, size_.y);
	easingSp_.SetSize(size_.x, size_.y);
	nowSp_.SetSize(size_.x, size_.y);
	HealSp_.SetSize(size_.x, size_.y);

	nowSp_.SetColor(color.x, color.y, color.z, 1);
	HealSp_.SetColor(0.2f, 0.2f, 0.2f, 1);

	easeTimer_ = 1;
	stopTimer_ = TIME_STOPER;

	oldNow_ = 1000000;
}

void Gauge::Update(float max, float now, float heal)
{
	//ゲージが下限突破しないように
	if (now < 0) {
		now = 0;
	}
	//ゲージが上限突破しないように
	if (now + heal > max) {
		heal = max - now;
	}

	//ゲージ変動が無ければタイマーを減らす
	if (oldNow_ <= now) {
		stopTimer_--;
	}
	else {
		if (stopTimer_ <= 0) {
			showSize_ = nowSize_;
		}
		oldNow_ = now;
		stopTimer_ = TIME_STOPER;
	}
	nowSize_ = size_ * now / max;
	healSize_ = size_ * heal / max;

	easingSp_.SetSizeX(nowSize_.x + EaseOut(easeTimer_, showSize_.x - nowSize_.x));
	nowSp_.SetSizeX(nowSize_.x);
	HealSp_.SetSizeX(nowSize_.x + healSize_.x);

	if (stopTimer_ <= 0) {
		if (nowSize_.x < showSize_.x) {
			if (easeTimer_ > 0) {
				easeTimer_ -= 0.05f;
			}
			else {
				showSize_ = nowSize_;
			}
		}
		else {
			easeTimer_ = 1;
		}
	}
}

void Gauge::Draw()
{
	HealSp_.Draw();
	easingSp_.Draw();
	nowSp_.Draw();
	maxSp_.Draw();
}
