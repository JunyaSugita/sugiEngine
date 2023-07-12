#include "ParticleEditer.h"
#include "ImGuiManager.h"
#include "ParticleManager.h"
#include <random>

using namespace ImGui;

void ParticleEditer::Initialize()
{
	for (int i = 0; i < particleNum; i++) {
		isEdit_[i] = false;
		life_[i] = 60;
		scale_[i][0] = scale_[i][1] = 1.0f;
		color_[i][0] = color_[i][1] = color_[i][2] = color_[i][3] = 1.0f;
		pos_[i][0] = pos_[i][1] = pos_[i][2] = 0.0f;
		move_[i][0] = move_[i][1] = move_[i][2] = 0.0f;
		moveRand_[i][0] = moveRand_[i][1] = moveRand_[i][2] = 0.0f;
	}
}

void ParticleEditer::Update()
{
#ifdef _DEBUG

	ImGuiManager* imgui = ImGuiManager::GetInstance();
	ParticleManager* parM = ParticleManager::GetInstance();

	if (GetIsEdit(0)) {
		PopParticle(0);
		if (GetIsEdit(1)) {
			PopParticle(1);
			if (GetIsEdit(2)) {
				PopParticle(2);
			}
		}
	}

	Begin("ParticleEditer");
	if (Button("EditMode", { 100,30 })) {
		isEdit_[0] = (isEdit_[0] + 1) % 2;
	}
	if (GetIsEdit(0)) {
		InputInt("life", &life_[0]);
		InputFloat3("pos", pos_[0]);
		InputFloat3("move", move_[0]);
		InputFloat3("moveRand", moveRand_[0]);
		InputFloat2("s_scale", scale_[0]);
		InputFloat3("gravity", gravity_[0]);
		ColorEdit4("color", color_[0]);
		if (Button("AddParticle1", { 100,30 })) {
			isEdit_[1] = (isEdit_[1] + 1) % 2;
		}
		if (GetIsEdit(1)) {
			InputInt("life1", &life_[1]);
			InputFloat3("pos1", pos_[1]);
			InputFloat3("move1", move_[1]);
			InputFloat3("moveRand1", moveRand_[1]);
			InputFloat2("s_scale1", scale_[1]);
			InputFloat3("gravity1", gravity_[1]);
			ColorEdit4("color1", color_[1]);
			if (Button("AddParticle2", { 100,30 })) {
				isEdit_[2] = (isEdit_[2] + 1) % 2;
			}
			if (GetIsEdit(2)) {
				InputInt("life2", &life_[2]);
				InputFloat3("pos2", pos_[2]);
				InputFloat3("move2", move_[2]);
				InputFloat3("moveRand2", moveRand_[2]);
				InputFloat2("s_scale2", scale_[2]);
				InputFloat3("gravity2", gravity_[2]);
				ColorEdit4("color2", color_[2]);
			}
		}
		else {
			isEdit_[2] = false;
		}
	}
	else {
		isEdit_[1] = false;
		isEdit_[2] = false;
	}
	End(); 

#endif 
}

void ParticleEditer::PopParticle(uint8_t num)
{
	//ƒ‰ƒ“ƒ_ƒ€
	std::random_device seed_gen;
	std::mt19937_64 engine(seed_gen());

	std::uniform_real_distribution<float> x(-moveRand_[num][0], moveRand_[num][0]);
	std::uniform_real_distribution<float> y(-moveRand_[num][1], moveRand_[num][1]);
	std::uniform_real_distribution<float> z(-moveRand_[num][2], moveRand_[num][2]);


	ParticleManager::GetInstance()->Add(int(life_[num]), {pos_[num][0],pos_[num][1],pos_[num][2]}, {move_[num][0] + x(engine),move_[num][1] + y(engine),move_[num][2] + z(engine)}, {gravity_[num][0],gravity_[num][1],gravity_[num][2]}, scale_[num][0], scale_[num][1], {color_[num][0],color_[num][1],color_[num][2],color_[num][3]});
}
