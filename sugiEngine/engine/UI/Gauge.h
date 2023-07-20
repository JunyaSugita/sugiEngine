#pragma once
#include "GrovalSetting.h"
#include "Sprite.h"

class Gauge {
public:

	void Set(Vector2 pos, Vector2 size);
	void Update(float max, float now);
	void Draw();

private:
	Vector2 size_;

	Vector2 nowSize_;
	Vector2 showSize_;

	Sprite maxSp_;
	Sprite nowSp_;

	int32_t maxTex_;
	int32_t nowTex_;

	float timer_;
};
