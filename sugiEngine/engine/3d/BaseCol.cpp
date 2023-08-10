#include "BaseCol.h"

void BaseCol::Initialize()
{
	colObj_.Initialize("box");

	col.pos = {0,0,0};
	col.size = { 1,1,1 };

	oldCol = col;

	gap = { 0,0,0 };
}

void BaseCol::Update()
{
	colObj_.Update();
}

void BaseCol::Draw()
{
	colObj_.Draw();
}

void BaseCol::SetCol(Vector3 pos)
{
	col.pos = pos + gap;

	colObj_.pos = col.pos;
	colObj_.scale = col.size;
}

void BaseCol::SetOldCol()
{
	oldCol = col;
}
