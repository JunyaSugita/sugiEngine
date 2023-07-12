#include "ParticleEditer.h"
#include "ImGuiManager.h"
#include "ParticleManager.h"

using namespace ImGui;

void ParticleEditer::Initialize()
{
	isEdit = false;
	life_ = 60;
	scale_[0] = scale_[1] = 1.0f;
	color_[0] = color_[1] = color_[2] = color_[3] = 1.0f;
	pos_[0] = pos_[1] = pos_[2] = 0.0f;
	move_[0] = move_[1] = move_[2] = 0.0f;
}

void ParticleEditer::Update()
{
#ifdef _DEBUG

	ImGuiManager* imgui = ImGuiManager::GetInstance();
	ParticleManager* parM = ParticleManager::GetInstance();

	if (isEdit) {
		parM->Add(int(life_),{pos_[0],pos_[1],pos_[2]}, {move_[0],move_[1],move_[2]}, {gravity_[0],gravity_[1],gravity_[2]}, scale_[0], scale_[1], {color_[0],color_[1],color_[2],color_[3]});
	}

	Begin("ParticleEditer");
	if (Button("EditMode", { 100,30 })) {
		isEdit = (isEdit + 1) % 2;
	}
	InputInt("life", &life_);
	InputFloat3("pos",pos_);
	InputFloat3("move", move_);
	InputFloat2("s_scale", scale_);
	InputFloat3("gravity",gravity_);
	ColorEdit4("color", color_);

	End(); 

#endif 
}
