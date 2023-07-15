#include "ParticleEditor.h"
#include "ImGuiManager.h"
#include "CircleParticle.h"
#include <random>

using namespace ImGui;

void ParticleEditor::Initialize()
{
	for (int i = 0; i < particleNum; i++) {
		isEdit_[i] = false;
		texNum_[i] = 0;
		num_[i] = 1;
		life_[i] = 60;
		lifeRand_[i] = 0;
		scale_[i][0] = scale_[i][1] = 1.0f;
		color_[i][0] = color_[i][1] = color_[i][2] = color_[i][3] = 1.0f;
		pos_[i][0] = pos_[i][1] = pos_[i][2] = 0.0f;
		posRand_[i][0] = posRand_[i][1] = posRand_[i][2] = 0.0f;
		move_[i][0] = move_[i][1] = move_[i][2] = 0.0f;
		moveRand_[i][0] = moveRand_[i][1] = moveRand_[i][2] = 0.0f;
		acceleration_[i][0] = acceleration_[i][1] = acceleration_[i][2] = 1.0f;
	}
	Write();
}

void ParticleEditor::Update()
{
#ifdef _DEBUG

	ImGuiManager* imgui = ImGuiManager::GetInstance();
	CircleParticle* parM = CircleParticle::GetInstance();

	if (GetIsEdit(0)) {
		PopParticle(0);
		if (GetIsEdit(1)) {
			PopParticle(1);
			if (GetIsEdit(2)) {
				PopParticle(2);
			}
		}
	}

	Begin("ParticleEditor");
	if (Button("EditMode", { 100,30 })) {
		isEdit_[0] = (isEdit_[0] + 1) % 2;
	}
	if (GetIsEdit(0)) {
		InputText("fileName", text_, sizeof(text_));
		if (Button("Save", { 100,30 })) {
			Save();
		}
		if (Button("Load", { 100,30 })) {
			Load();
		}
		InputInt("texNum",&texNum_[0]);
		InputInt("num", &num_[0]);
		InputInt("life", &life_[0]);
		InputInt("lifeRand", &lifeRand_[0]);
		InputFloat3("pos", pos_[0]);
		InputFloat3("posRand", posRand_[0]);
		InputFloat3("move", move_[0]);
		InputFloat3("moveRand", moveRand_[0]);
		InputFloat3("acceleration", acceleration_[0]);
		InputFloat2("s_scale->e_scale", scale_[0]);
		InputFloat3("gravity", gravity_[0]);
		ColorEdit4("color", color_[0]);
		if (Button("AddParticle1", { 100,30 })) {
			isEdit_[1] = (isEdit_[1] + 1) % 2;
		}
		if (GetIsEdit(1)) {
			InputInt("texNum1", &texNum_[1]);
			InputInt("num1", &num_[1]);
			InputInt("life1", &life_[1]);
			InputInt("lifeRand1", &lifeRand_[1]);
			InputFloat3("pos1", pos_[1]);
			InputFloat3("posRand1", posRand_[1]);
			InputFloat3("move1", move_[1]);
			InputFloat3("moveRand1", moveRand_[1]);
			InputFloat3("acceleration1", acceleration_[1]);
			InputFloat2("s_scale->e_scale1", scale_[1]);
			InputFloat3("gravity1", gravity_[1]);
			ColorEdit4("color1", color_[1]);
			if (Button("AddParticle2", { 100,30 })) {
				isEdit_[2] = (isEdit_[2] + 1) % 2;
			}
			if (GetIsEdit(2)) {
				InputInt("texNum2", &texNum_[2]);
				InputInt("num2", &num_[2]);
				InputInt("life2", &life_[2]);
				InputInt("lifeRand2", &lifeRand_[2]);
				InputFloat3("pos2", pos_[2]);
				InputFloat3("posRand2", posRand_[2]);
				InputFloat3("move2", move_[2]);
				InputFloat3("moveRand2", moveRand_[2]);
				InputFloat3("acceleration2", acceleration_[2]);
				InputFloat2("s_scale->e_scale2", scale_[2]);
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

void ParticleEditor::PopParticle(uint8_t num)
{
	//ƒ‰ƒ“ƒ_ƒ€
	std::random_device seed_gen;
	std::mt19937_64 engine(seed_gen());
	for (int i = 0; i < num_[num]; i++) {
		std::uniform_real_distribution<float> xp(-posRand_[num][0], posRand_[num][0]);
		std::uniform_real_distribution<float> yp(-posRand_[num][1], posRand_[num][1]);
		std::uniform_real_distribution<float> zp(-posRand_[num][2], posRand_[num][2]);

		std::uniform_real_distribution<float> xm(-moveRand_[num][0], moveRand_[num][0]);
		std::uniform_real_distribution<float> ym(-moveRand_[num][1], moveRand_[num][1]);
		std::uniform_real_distribution<float> zm(-moveRand_[num][2], moveRand_[num][2]);

		std::uniform_real_distribution<float> l(-float(lifeRand_[num] + 0.99f), float(lifeRand_[num] + 0.99f));

		if (texNum_[num] == 0) {
			CircleParticle::GetInstance()->AddCircle(int(life_[num] + int32_t(l(engine))), { pos_[num][0] + xp(engine),pos_[num][1] + 5 + yp(engine),pos_[num][2] + zp(engine) }, { move_[num][0] + xm(engine),move_[num][1] + ym(engine),move_[num][2] + zm(engine) }, { acceleration_[num][0],acceleration_[num][1],acceleration_[num][2] }, { gravity_[num][0],gravity_[num][1],gravity_[num][2] }, scale_[num][0], scale_[num][1], { color_[num][0],color_[num][1],color_[num][2],color_[num][3] });
		}
		if (texNum_[num] == 1) {
			CircleParticle::GetInstance()->AddIce(int(life_[num] + int32_t(l(engine))), { pos_[num][0] + xp(engine),pos_[num][1] + 5 + yp(engine),pos_[num][2] + zp(engine) }, { move_[num][0] + xm(engine),move_[num][1] + ym(engine),move_[num][2] + zm(engine) }, { acceleration_[num][0],acceleration_[num][1],acceleration_[num][2] }, { gravity_[num][0],gravity_[num][1],gravity_[num][2] }, scale_[num][0], scale_[num][1], { color_[num][0],color_[num][1],color_[num][2],color_[num][3] });
		}
	}
}

void ParticleEditor::Save()
{
	Write();
	
	fopen_s(&saveFile_, text_, "wb");

	fwrite(&editData_, sizeof(editData_), 1, saveFile_);

	fclose(saveFile_);
}

void ParticleEditor::Load()
{
	fopen_s(&saveFile_, text_, "rb");

	if (saveFile_ == NULL) {
		return;
	}

	fread(&editData_, sizeof(editData_), 1, saveFile_);

	fclose(saveFile_);

	Read();
}

void ParticleEditor::Write()
{
	editData_.texNum = texNum_[0];
	editData_.num = num_[0];
	editData_.life = life_[0];
	editData_.lifeRand = lifeRand_[0];
	editData_.pos = { pos_[0][0],pos_[0][1] ,pos_[0][2] };
	editData_.posRand = { posRand_[0][0],posRand_[0][1] ,posRand_[0][2] };
	editData_.scale = { scale_[0][0],scale_[0][1] };
	editData_.move = { move_[0][0],move_[0][1],move_[0][2] };
	editData_.moveRand = { moveRand_[0][0],moveRand_[0][1],moveRand_[0][2] };
	editData_.acceleration = { acceleration_[0][0],acceleration_[0][1],acceleration_[0][2] };
	editData_.gravity = { gravity_[0][0],gravity_[0][1],gravity_[0][2] };
	editData_.color = { color_[0][0],color_[0][1],color_[0][2],color_[0][3] };
	editData_.add1 = GetIsEdit(1);
	editData_.texNum1 = texNum_[1];
	editData_.num1 = num_[1];
	editData_.life1 = life_[1];
	editData_.lifeRand1 = lifeRand_[1];
	editData_.pos1 = { pos_[1][0],pos_[1][1] ,pos_[1][2] };
	editData_.posRand1 = { posRand_[1][0],posRand_[1][1] ,posRand_[1][2] };
	editData_.scale1 = { scale_[1][0],scale_[1][1] };
	editData_.move1 = { move_[1][0],move_[1][1],move_[1][2] };
	editData_.moveRand1 = { moveRand_[1][0],moveRand_[1][1],moveRand_[1][2] };
	editData_.acceleration1 = { acceleration_[1][0],acceleration_[1][1],acceleration_[1][2] };
	editData_.gravity1 = { gravity_[1][0],gravity_[1][1],gravity_[1][2] };
	editData_.color1 = { color_[1][0],color_[1][1],color_[1][2],color_[1][3] };
	editData_.add2 = GetIsEdit(2);
	editData_.texNum2 = texNum_[2];
	editData_.num2 = num_[2];
	editData_.life2 = life_[2];
	editData_.lifeRand2 = lifeRand_[2];
	editData_.pos2 = { pos_[2][0],pos_[2][1] ,pos_[2][2] };
	editData_.posRand2 = { posRand_[2][0],posRand_[2][1] ,posRand_[2][2] };
	editData_.scale2 = { scale_[2][0],scale_[2][1] };
	editData_.move2 = { move_[2][0],move_[2][1],move_[2][2] };
	editData_.moveRand2 = { moveRand_[2][0],moveRand_[2][1],moveRand_[2][2] };
	editData_.acceleration2 = { acceleration_[2][0],acceleration_[2][1],acceleration_[2][2] };
	editData_.gravity2 = { gravity_[2][0],gravity_[2][1],gravity_[2][2] };
	editData_.color2 = { color_[2][0],color_[2][1],color_[2][2],color_[2][3] };

}

void ParticleEditor::Read()
{
	texNum_[0] = editData_.texNum;
	num_[0] = editData_.num;
	life_[0] = editData_.life;
	lifeRand_[0] = editData_.lifeRand;
	pos_[0][0] = editData_.pos.x;
	pos_[0][1] = editData_.pos.y;
	pos_[0][2] = editData_.pos.z;
	posRand_[0][0] = editData_.posRand.x;
	posRand_[0][1] = editData_.posRand.y;
	posRand_[0][2] = editData_.posRand.z;
	scale_[0][0] = editData_.scale.x;
	scale_[0][1] = editData_.scale.y;
	move_[0][0] = editData_.move.x;
	move_[0][1] = editData_.move.y;
	move_[0][2] = editData_.move.z;
	moveRand_[0][0] = editData_.moveRand.x;
	moveRand_[0][1] = editData_.moveRand.y;
	moveRand_[0][2] = editData_.moveRand.z;
	acceleration_[0][0] = editData_.acceleration.x;
	acceleration_[0][1] = editData_.acceleration.y;
	acceleration_[0][2] = editData_.acceleration.z;
	gravity_[0][0] = editData_.gravity.x;
	gravity_[0][1] = editData_.gravity.y;
	gravity_[0][2] = editData_.gravity.z;
	color_[0][0] = editData_.color.x;
	color_[0][1] = editData_.color.y;
	color_[0][2] = editData_.color.z;
	color_[0][3] = editData_.color.w;

	isEdit_[1] = editData_.add1;
	texNum_[1] = editData_.texNum1;
	num_[1] = editData_.num1;
	life_[1] = editData_.life1;
	lifeRand_[1] = editData_.lifeRand1;
	pos_[1][0] = editData_.pos1.x;
	pos_[1][1] = editData_.pos1.y;
	pos_[1][2] = editData_.pos1.z;
	posRand_[1][0] = editData_.posRand1.x;
	posRand_[1][1] = editData_.posRand1.y;
	posRand_[1][2] = editData_.posRand1.z;
	scale_[1][0] = editData_.scale1.x;
	scale_[1][1] = editData_.scale1.y;
	move_[1][0] = editData_.move1.x;
	move_[1][1] = editData_.move1.y;
	move_[1][2] = editData_.move1.z;
	moveRand_[1][0] = editData_.moveRand1.x;
	moveRand_[1][1] = editData_.moveRand1.y;
	moveRand_[1][2] = editData_.moveRand1.z;
	acceleration_[1][0] = editData_.acceleration1.x;
	acceleration_[1][1] = editData_.acceleration1.y;
	acceleration_[1][2] = editData_.acceleration1.z;
	gravity_[1][0] = editData_.gravity1.x;
	gravity_[1][1] = editData_.gravity1.y;
	gravity_[1][2] = editData_.gravity1.z;
	color_[1][0] = editData_.color1.x;
	color_[1][1] = editData_.color1.y;
	color_[1][2] = editData_.color1.z;
	color_[1][3] = editData_.color1.w;

	isEdit_[2] = editData_.add2;
	texNum_[2] = editData_.texNum2;
	num_[2] = editData_.num2;
	life_[2] = editData_.life2;
	lifeRand_[2] = editData_.lifeRand2;
	pos_[2][0] = editData_.pos2.x;
	pos_[2][1] = editData_.pos2.y;
	pos_[2][2] = editData_.pos2.z;
	posRand_[2][0] = editData_.posRand2.x;
	posRand_[2][1] = editData_.posRand2.y;
	posRand_[2][2] = editData_.posRand2.z;
	scale_[2][0] = editData_.scale2.x;
	scale_[2][1] = editData_.scale2.y;
	move_[2][0] = editData_.move2.x;
	move_[2][1] = editData_.move2.y;
	move_[2][2] = editData_.move2.z;
	moveRand_[2][0] = editData_.moveRand2.x;
	moveRand_[2][1] = editData_.moveRand2.y;
	moveRand_[2][2] = editData_.moveRand2.z;
	acceleration_[2][0] = editData_.acceleration2.x;
	acceleration_[2][1] = editData_.acceleration2.y;
	acceleration_[2][2] = editData_.acceleration2.z;
	gravity_[2][0] = editData_.gravity2.x;
	gravity_[2][1] = editData_.gravity2.y;
	gravity_[2][2] = editData_.gravity2.z;
	color_[2][0] = editData_.color2.x;
	color_[2][1] = editData_.color2.y;
	color_[2][2] = editData_.color2.z;
	color_[2][3] = editData_.color2.w;
}
