#pragma once
#include "GrovalSetting.h"
#include "Sprite.h"

class Box
{
public:
	void Initialize(Vector2 pos,Vector2 size,Vector4 color);
	void Draw();

	void SetPos(Vector2 pos) {
		pos_ = pos;
	}
	void SetPosX(float pos) {
		pos_.x = pos;
	}
	void SetPosY(float pos) {
		pos_.y = pos;
	}

	void AddPosX(float add) {
		pos_.x = add;
	}
	void AddPosY(float add) {
		pos_.y = add;
	}

	void SetSize(Vector2 size) {
		box_.SetSize(size.x,size.y);
	}
	void SetSizeX(float size) {
		box_.SetSizeX(size);
	}
	void SetSizeY(float size) {
		box_.SetSizeY(size);
	}

private:
	Vector2 pos_;
	float size_;
	Sprite box_;
};

