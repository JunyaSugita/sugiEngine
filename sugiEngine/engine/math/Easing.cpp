#include "Easing.h"
#include <DirectXMath.h>

const float PI = 3.14159265f;

float EaseIn(float time, float move)
{
	return float(1 - cosf((time * PI) / 2)) * move;
}

float EaseOut(float time, float move)
{
	return float(sinf((time * PI) / 2)) * move;
}

float EaseInOut(float time, float move)
{
	return float(-(cosf(PI * time) - 1) / 2) * move;
}

float EaseOutBounce(float time, float move)
{
	const float n1 = 7.5625f;
	const float d1 = 2.75f;

	if (time < 1 / d1) {
		return float(n1 * time * time) * move;
	}
	else if (time < 2 / d1) {
		return float(n1 * (time - 1.5f / d1) * time + 0.75) * move;
	}
	else if (time < 2.5f / d1) {
		return float(n1 * (time - 2.25 / d1) * time + 0.9375) * move;
	}
	else{
		return float(n1 * (time - 2.625 / d1) * time + 0.984375) * move;
	}
}
