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
	circleParticle_.Initialize("effectCircle.png");
	iceParticle_.Initialize("effectIce.png");

	particleE_.release();
	particleE_ = make_unique<ParticleEditor>();
	particleE_->Initialize();
}

void ParticleManager::Update()
{
	circleParticle_.Update();
	iceParticle_.Update();

	particleE_->Update();
}

void ParticleManager::Draw()
{
	circleParticle_.Draw();
	iceParticle_.Draw();
}

void ParticleManager::AddCircle(int texture, int life, Vector3 pos, bool isRevers, Vector3 velo, float speed, Vector3 accel, Vector3 gravity, float start_scale, float end_scale, Vector3 sColor, Vector3 eColor, int32_t postEffect)
{
	if (texture == 0) {
		circleParticle_.AddCircle(life, pos, isRevers, velo, speed, accel, gravity, start_scale, end_scale, sColor, eColor, postEffect);
	}
	else if (texture == 1) {
		iceParticle_.AddCircle(life, pos, isRevers, velo, speed, accel, gravity, start_scale, end_scale, sColor, eColor, postEffect);
	}
}

void ParticleManager::AddFromFile(uint8_t num, Vector3 pos, bool isEdit)
{
	if (isEdit || !GetIsEdit()) {
		circleParticle_.Add(pos, particleData_[num]);
	}
}

void ParticleManager::Clear()
{
	circleParticle_.Clear();
	iceParticle_.Clear();
}

void ParticleManager::AddFromFileEditScaleAndColor(uint8_t num, Vector3 pos, float scale, Vector3 color, bool isEdit)
{
	if (isEdit || !GetIsEdit()) {
		circleParticle_.AddEditScaleAndColor(pos, particleData_[num], scale, color);
	}
}

void ParticleManager::Finalize()
{
	particleE_.release();
}