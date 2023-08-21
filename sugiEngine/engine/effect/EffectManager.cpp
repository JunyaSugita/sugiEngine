#include "EffectManager.h"

EffectManager* EffectManager::GetInstance()
{
	static EffectManager instance;

	return &instance;
}

void EffectManager::Initialize()
{
	
}

void EffectManager::Update()
{
	//飛び散りエフェクト
	burst_.remove_if([](std::unique_ptr<Burst>& burst) {return burst->IsDead(); });
	bolt_.remove_if([](std::unique_ptr<Bolt>& bolt) {return bolt->IsDead(); });
	for (std::unique_ptr<Burst>& burst : burst_) {
		burst->Update();
	}
	for (std::unique_ptr<Bolt>& bolt : bolt_) {
		bolt->Update();
	}
}

void EffectManager::Draw()
{
	for (std::unique_ptr<Burst>& burst : burst_) {
		burst->Draw();
	}
	for (std::unique_ptr<Bolt>& bolt : bolt_) {
		bolt->Draw();
	}
}

//飛び散りエフェクト
void EffectManager::BurstGenerate(const Vector3& pos, int32_t num,const Vector4& color, float range, float pow)
{
	for (int32_t i = 0; i < num; i++) {
		std::unique_ptr<Burst> newBurst = std::make_unique<Burst>();
		newBurst->Initialize(pos, color, range, pow);
		burst_.push_back(std::move(newBurst));
	}
}

void EffectManager::BoltGenerate(const Vector3& posS, const Vector3& posE, const Vector3& rot, const Vector4& color)
{
	std::unique_ptr<Bolt> newBolt = std::make_unique<Bolt>();
	newBolt->Initialize(posS, posE,rot, color);
	bolt_.push_back(std::move(newBolt));
}
