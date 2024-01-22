#include "ParticleManager.h"

using namespace std;

ParticleManager* ParticleManager::GetInstance()
{
	//インスタンス生成
	static ParticleManager instance;

	return &instance;
}

void ParticleManager::Initialize()
{
	particle_[CIRCLE].Initialize("effectCircle.png");
	particle_[FIRE1].Initialize("effectFire1.png");
	particle_[FIRE2].Initialize("effectFire2.png");
	particle_[FIRE3].Initialize("effectFire3.png");
	particle_[FIRE4].Initialize("effectFire4.png");
	particle_[FIRE_ALPHA1].Initialize("effectFireAlpha1.png");
	particle_[FIRE_ALPHA2].Initialize("effectFireAlpha2.png");

	particleE_.release();
	particleE_ = make_unique<ParticleEditor>();
	particleE_->Initialize();
}

void ParticleManager::Update()
{
	for (int i = 0; i < PARTICLE_TEXTURE_END; i++) {
		particle_[i].Update();
	}

	particleE_->Update();
}

void ParticleManager::Draw()
{
	for (int i = 0; i < PARTICLE_TEXTURE_END; i++) {
		particle_[i].Draw();
	}
}

void ParticleManager::AddCircle(int texture, int life, Vector3 pos, bool isRevers, Vector3 velo, float speed, Vector3 accel, Vector3 gravity, float start_scale, float end_scale, Vector3 sColor, Vector3 eColor, int32_t postEffect)
{
	particle_[texture].AddCircle(life, pos, isRevers, velo, speed, accel, gravity, start_scale, end_scale, sColor, eColor, postEffect);
}

void ParticleManager::AddFromFile(uint8_t num, Vector3 pos, bool isEdit)
{
	if (isEdit || !GetIsEdit()) {
		particle_[CIRCLE].Add(pos, particleData_[num]);
	}
}

void ParticleManager::Clear()
{
	for (int i = 0; i < PARTICLE_TEXTURE_END; i++) {
		particle_[i].Clear();
	}
}

void ParticleManager::AddFromFileEditScaleAndColor(uint8_t num, Vector3 pos, float scale, Vector3 color, bool isEdit)
{
	if (isEdit || !GetIsEdit()) {
		particle_[CIRCLE].AddEditScaleAndColor(pos, particleData_[num], scale, color);
	}
}

void ParticleManager::Finalize()
{
	particleE_.release();
}