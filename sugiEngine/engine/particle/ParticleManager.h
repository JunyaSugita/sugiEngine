#pragma once
#include "Particle.h"
#include <array>

enum ParticleName {
	P_FIRE_BALL,
	P_FIRE_BALL_EXPLODE,
	P_ICE,
	P_MAGIC_MISSILE,
	P_LIGHTNING,
	P_WEAPON,
	P_WEAPON_FIRE,
	P_DEBUFF_FIRE,
	P_GOAL,
	P_TORCH,
	P_DAMAGE,
	P_CHARGE_FIRE,
	P_CHARGE_MAX_FIRE,
	P_BACKGROUND,
	P_ICE_BREAK,
	P_DEBUFF_ICE,

	P_END
};

enum PARTICLE_TEXTURE {
	CIRCLE,
	FIRE1,
	FIRE2,
	FIRE3,

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

	void AddCircle(int texture, int life, Vector3 pos, bool isRevers, Vector3 velo, float speed, Vector3 accel, Vector3 gravity, float start_scale, float end_scale, Vector3 sColor, Vector3 eColor, int32_t postEffect);
	void AddFromFile(uint8_t num, Vector3 pos, bool isEdit = false);
	void Clear();

	void SetParticleData(int num, EditFile file) {
		particleData_[num] = file;
	}

	void AddFromFileEditScaleAndColor(uint8_t num, Vector3 pos, float scale, Vector3 color, bool isEdit = false);

	bool GetIsEdit() {
		return particleE_->GetIsEdit(0);
	}

private:
	EditFile particleData_[100];

	std::array<Particle, PARTICLE_TEXTURE_END> particle_;

	std::unique_ptr<ParticleEditor> particleE_ = nullptr;
};

