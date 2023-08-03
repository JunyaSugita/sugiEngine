#pragma once
#include "BaseEnemy.h"

class Enemy : public BaseEnemy{
public:
	static void StaticInitialize();
	void Initialize(Vector3 pos) override;
	void Draw() override;
	void WorldTransUpdate() override;

private:
	void SetWorldTrans() override;
	void Move() override;
	void Attack() override;
	
private:
	const float SPEED_MOVE = 0.15f;
	const float SPEED_ANGLE = 5;
	const uint32_t MAX_HP = 25;
	const float HEIGHT_COL = 2.2f;

private:
	static std::unique_ptr<Model> sEyeModel_;

	//–Ú
	WorldTransform eyeWorldTrans_;
	Vector3 eyePos_;
	Vector3 eyeRot_;
	Vector3 eyeScale_;

	//–Ú‚Ìƒ‚ƒfƒ‹ŠÖŒW
	std::unique_ptr<Object3d> eyeObj_;

	//˜r
	WorldTransform armRWorldTrans_;
	WorldTransform armLWorldTrans_;
	Vector3 armRPos_;
	Vector3 armLPos_;
	Vector3 armRot_;
	Vector3 armScale_;
	//˜rƒ‚ƒfƒ‹
	std::unique_ptr<Object3d> armRObj_;
	std::unique_ptr<Object3d> armLObj_;
};