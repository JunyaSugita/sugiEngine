/**
 * @file Wall.h
 * @brief フィールドに置く壁
 */

#pragma once
#include "LightGroup.h"
#include "Model.h"
#include "BaseObj.h"
#include "BaseCol.h"

class Wall : public BaseCol
{
public:
	void Initialize(Vector3 pos, Vector3 scale);
	void Update();
	void Draw();

private:
	std::unique_ptr<Model> wallModel_;
	BaseObj wallObj_;
};