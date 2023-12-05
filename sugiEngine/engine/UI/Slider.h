#pragma once
#include "GrovalSetting.h"
#include "Box.h"

class Slider
{
public:
	void Initialize(Vector2 pos, float min, float max, float size);
	void Update(float& now);
	void Draw();

	void SetPointColor(Vector3 col);

private:
	Box sliderLine_;
	Box sliderPoint_;

	Vector2 pos_;
	float min_;
	float max_;
	float size_;
};

