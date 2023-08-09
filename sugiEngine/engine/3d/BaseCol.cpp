#include "BaseCol.h"

void BaseCol::Initialize()
{
	colObj.Initialize("Box");
}

void BaseCol::Update()
{
	colObj.Update();
}

void BaseCol::Draw()
{
	colObj.Draw();
}

void BaseCol::SetCol(Vector3 pos)
{
	col.pos = pos + gap;
}

void BaseCol::SetOldCol()
{
	oldCol = col;
}
