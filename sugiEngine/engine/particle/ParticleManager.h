#pragma once
#include "Particle.h"
#include <array>

enum ParticleName {
	P_FIRE,
	P_FIRE2,
	P_FIRE_BALL_EXPLODE,
	P_TORCH,
	P_TORCH2,
	P_ICE,
	P_ICE2,
	P_MAGIC_MISSILE,
	P_MAGIC_MISSILE2,
	P_WEAPON,
	P_GOAL,
	P_DEBUFF_FIRE,
	P_DEBUFF_FIRE2,
	P_DAMAGE,
	P_DEBUFF_ICE,
	P_DEBUFF_ICE2,
	P_ICE_BREAK,
	P_ICE_BREAK2,
	P_CHARGE_FIRE,
	P_CHARGE_MAX_FIRE,
	P_DOWN,
	P_TEST,

	P_END
};

enum PARTICLE_TEXTURE {
	CIRCLE,
	FIRE1,
	FIRE2,
	FIRE3,
	FIRE4,
	FIRE_ALPHA1,
	FIRE_ALPHA2,
	FIRE_ALPHA3,
	FIRE_ALPHA4,
	EXPLODE,
	ICE,

	PARTICLE_TEXTURE_END
};

class ParticleManager
{
private:
	ParticleManager() = default;
	~ParticleManager() = default;

public:
	//コピーコンストラクタ無効
	ParticleManager(const ParticleManager& obj) = delete;
	//代入演算子を無効
	ParticleManager& operator=(const ParticleManager& obj) = delete;

	static ParticleManager* GetInstance();

public:
	void Initialize();
	void Update();
	void Draw();
	void Finalize();

	void AddCircle(int texture, int life, Vector3 pos, bool isRevers, Vector3 velo, float speed, Vector3 accel, Vector3 gravity, Vector2 checkS, Vector4 scale, Vector4 sColor, float check1, Vector4 check1Color, float check2, Vector4 check2Color, Vector4 eColor, int32_t postEffect);
	void AddFromFile(int32_t num, Vector3 pos, bool isEdit = false);
	void Clear();

	void SetParticleData(int num, EditFile file) {
		particleData_[num] = file;
	}

	void AddFromFileEditScaleAndColor(uint8_t num, Vector3 pos, float scale, Vector4 color, WorldTransform* w = nullptr, bool isEdit = false);
	void AddFromFileEditColor(uint8_t num, Vector3 pos, Vector4 color,WorldTransform* w = nullptr, bool isEdit = false);

	bool GetIsEdit() {
		return particleE_->GetIsEdit();
	}

private:
	EditFile particleData_[100];

	std::array<Particle, PARTICLE_TEXTURE_END> particle_;

	std::unique_ptr<ParticleEditor> particleE_ = nullptr;
};

