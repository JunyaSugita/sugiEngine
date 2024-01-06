#include "Torch.h"
#include <random>
#include "ParticleManager.h"

LightGroup* Torch::lightGroup_ = nullptr;

void Torch::Initialize(Vector3 pos, Vector3 rot,Vector3 scale, float atten)
{
	//モデルを指定して3Dオブジェクトを生成
	torchObj_.Initialize("torch");

	//obj情報
	torchObj_.pos = pos;
	torchObj_.rot = rot + PATCH_ROT;
	torchObj_.scale = scale;
	torchObj_.obj->SetColor(COLOR);

	stageAtten_ = atten;

	useLightNum_ = lightGroup_->SetPointLightGetNum();
	lightGroup_->SetPointLightColor(useLightNum_, COLOR_LIGHT);
	lightGroup_->SetPointLightAtten(useLightNum_, { stageAtten_,stageAtten_,stageAtten_ });
	lightGroup_->SetPointLightPos(useLightNum_, { torchObj_.pos.x, torchObj_.pos.y ,torchObj_.pos.z });
}

void Torch::Update()
{
	//ランダム
	std::random_device seed_gen;
	std::mt19937_64 engine(seed_gen());

	std::uniform_real_distribution<float> atten(stageAtten_ - (stageAtten_ / PATCH_ATTEN), stageAtten_ + (stageAtten_ / PATCH_ATTEN));
	lightGroup_->SetPointLightAtten(useLightNum_, { atten(engine),atten(engine),atten(engine) });

	ParticleManager::GetInstance()->AddFromFile(P_TORCH, torchObj_.pos);

	torchObj_.Update();
}

void Torch::Draw()
{
	torchObj_.Draw();
}
