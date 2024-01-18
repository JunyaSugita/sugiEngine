/**
 * @file IScene.h
 * @brief シーンのベースクラス
 */

#pragma once

class IScene
{
public:
	virtual ~IScene() = 0 {}
public:
	virtual void Initialize() = 0;
	virtual void Update() = 0;
	virtual void BackSpriteDraw() = 0;
	virtual void Draw() = 0;
	virtual void ObjDraw() = 0;
	virtual void ObjDraw2() = 0;
	virtual void ParticleDraw() = 0;
	virtual void SpriteDraw() = 0;
	virtual void Finalize() = 0;

};
