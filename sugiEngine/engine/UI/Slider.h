#pragma once
#include "GrovalSetting.h"
#include "Box.h"

class Slider
{
public:
	void Initialize(Vector2 pos,float min,float max,float size);
	void Update();
	void Draw();

private:
	Box slider_;
};

