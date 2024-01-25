#include "ParticleManager.h"
#include <random>

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
	particle_[FIRE_ALPHA1].Initialize("effectFireAlpha1.png", ALPHA);
	particle_[FIRE_ALPHA2].Initialize("effectFireAlpha2.png", ALPHA);
	particle_[FIRE_ALPHA3].Initialize("effectFireAlpha3.png", ALPHA);
	particle_[FIRE_ALPHA4].Initialize("effectFireAlpha4.png", ALPHA);
	particle_[EXPLODE].Initialize("effectExplode.png");

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
	for (int j = 0; j < 2; j++) {
		for (int i = 0; i < PARTICLE_TEXTURE_END; i++) {
			if (particle_[i].GetBlendType() == j) {
				particle_[i].Draw();
			}
		}
	}
}

void ParticleManager::AddCircle(int texture, int life, Vector3 pos, bool isRevers, Vector3 velo, float speed, Vector3 accel, Vector3 gravity, Vector2 checkS,Vector4 scale, Vector4 sColor, float check1, Vector4 check1Color, float check2, Vector4 check2Color, Vector4 eColor, int32_t postEffect)
{
	particle_[texture].AddCircle(life, pos, isRevers, velo, speed, accel, gravity,checkS ,scale, sColor, check1, check1Color, check2, check2Color, eColor, postEffect);
}

void ParticleManager::AddFromFile(int32_t num, Vector3 pos, bool isEdit)
{
	if (isEdit || !GetIsEdit()) {
		//ランダム
		std::random_device seed_gen;
		std::mt19937_64 engine(seed_gen());


		std::uniform_real_distribution<float> tex((float)particleData_[num].texNum, (float)particleData_[num].randTexNum + 0.999f);

		particle_[(int32_t)tex(engine)].Add(pos, particleData_[num]);
	}
}

void ParticleManager::Clear()
{
	for (int i = 0; i < PARTICLE_TEXTURE_END; i++) {
		particle_[i].Clear();
	}
}

void ParticleManager::AddFromFileEditScaleAndColor(uint8_t num, Vector3 pos, float scale, Vector4 color, bool isEdit)
{
	if (isEdit || !GetIsEdit()) {
		particle_[CIRCLE].AddEditScaleAndColor(pos, particleData_[num], scale, color);
	}
}

void ParticleManager::Finalize()
{
	particleE_.release();
}