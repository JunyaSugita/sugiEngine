#pragma once

class ISceneManager
{
public:
	virtual void Initialize() = 0;
	virtual void Update() = 0;
	virtual void BackSpriteDraw() = 0;
	virtual void Draw() = 0;
	virtual void SpriteDraw() = 0;
	virtual void Delete() = 0;

};

