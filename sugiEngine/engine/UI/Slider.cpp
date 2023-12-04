#include "Slider.h"

void Slider::Initialize(Vector2 pos, float min, float max, float size)
{
	sliderLine_.Initialize(pos, { size,30 }, { 1,1,1,1 });
	sliderPoint_.Initialize(pos, { 50,50 }, { 0.8f,0.8f,0.8f,1 });

	pos_ = pos;
	min_ = min;
	max_ = max;
	size_ = size;
}

void Slider::Update(float now)
{
	float par = (now - min_) / (max_ - min_);

	float temp = pos_.x - size_ / 2 + size_ * par;

	sliderPoint_.SetPosX(temp);
	sliderPoint_.Update();
}

void Slider::Draw()
{
	sliderLine_.Draw();
	sliderPoint_.Draw();
}

void Slider::SetPointColor(Vector3 col)
{
	sliderPoint_.SetColor(col);
}
