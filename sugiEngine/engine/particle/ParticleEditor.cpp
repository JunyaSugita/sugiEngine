
#include "ParticleEditor.h"
#include "ImGuiManager.h"
#include "ParticleManager.h"
#include <random>
#include "Camera.h"

using namespace ImGui;

void ParticleEditor::Initialize()
{
	for (int i = 0; i < particleNum; i++) {
		isEdit_[i] = false;
		texNum_[i] = 0;
		num_[i] = 1;
		life_[i] = 60;
		lifeRand_[i] = 0;
		pos_[i][0] = pos_[i][1] = pos_[i][2] = 0.0f;
		posRand_[i][0] = posRand_[i][1] = posRand_[i][2] = 0.0f;
		isRevers_[i] = false;
		move_[i][0] = move_[i][1] = move_[i][2] = 0.0f;
		moveRand_[i][0] = moveRand_[i][1] = moveRand_[i][2] = 0.0f;
		speed_[i] = 100.0f;
		acceleration_[i][0] = acceleration_[i][1] = acceleration_[i][2] = 1.0f;
		scale_[i][0] = scale_[i][1] = 1.0f;
		angleSpeed_[i] = 0;
		angleSpeedRand_[i] = 0;
		gravity_[i][0] = gravity_[i][1] = gravity_[i][2] = 0;
		sColor_[i][0] = sColor_[i][1] = sColor_[i][2] = 1.0f;
		eColor_[i][0] = eColor_[i][1] = eColor_[i][2] = 1.0f;
		postEffect_[i] = 0;
	}
	Write();

	for (int i = 0; i < 100; i++) {
		fopen_s(&saveFile_, "Resources/ParticleData/particleName.dat", "rb");
		if (saveFile_ == NULL) {
			return;
		}

		fread(&particleName, sizeof(particleName), 1, saveFile_);

		fclose(saveFile_);
	}

	SetParticleData();
}

void ParticleEditor::Update()
{
#ifdef _DEBUG
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
		InputInt("texNum", &texNum_[0]);
		InputInt("num", &num_[0]);
		InputInt("life", &life_[0]);
		InputInt("lifeRand", &lifeRand_[0]);
		InputFloat3("pos", pos_[0]);
		InputFloat3("posRand", posRand_[0]);
		Checkbox("revers",&isRevers_[0]);
		InputFloat3("move", move_[0]);
		InputFloat3("moveRand", moveRand_[0]);
		InputFloat("speed",&speed_[0]);
		InputFloat3("acceleration", acceleration_[0]);
		InputFloat2("s_scale->e_scale", scale_[0]);
		SliderFloat("angleSpeed", &angleSpeed_[0],-10,10,"%.1f");
		SliderFloat("angleSpeedRand", &angleSpeedRand_[0], -10, 10, "%.1f");
		InputFloat3("gravity", gravity_[0]);
		ColorEdit3("s_color", sColor_[0]);
		ColorEdit3("e_color", eColor_[0]);
		if (Button("UseSameColor", { 100,30 })) {
			eColor_[0][0] = sColor_[0][0];
			eColor_[0][1] = sColor_[0][1];
			eColor_[0][2] = sColor_[0][2];
		}
		Text("postEffect");
		if (RadioButton("none", postEffect_[0] == P_NONE)) {
			postEffect_[0] = P_NONE;
		}
		if (RadioButton("bloom", postEffect_[0] == P_BLOOM)) {
			postEffect_[0] = P_BLOOM;
		}
		if (RadioButton("cross", postEffect_[0] == P_CROSS)) {
			postEffect_[0] = P_CROSS;
		}
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
			Checkbox("revers1", &isRevers_[1]);
			InputFloat3("move1", move_[1]);
			InputFloat3("moveRand1", moveRand_[1]);
			InputFloat("speed1", &speed_[1]);
			InputFloat3("acceleration1", acceleration_[1]);
			InputFloat2("s_scale->e_scale1", scale_[1]);
			SliderFloat("angleSpeed1", &angleSpeed_[1], -10, 10, "%.1f");
			SliderFloat("angleSpeedRand1", &angleSpeedRand_[1], -10, 10, "%.1f");
			InputFloat3("gravity1", gravity_[1]);
			ColorEdit3("s_color1", sColor_[1]);
			ColorEdit3("e_color1", eColor_[1]);
			if (Button("UseSameColor", { 100,30 })) {
				eColor_[1][0] = sColor_[1][0];
				eColor_[1][1] = sColor_[1][1];
				eColor_[1][2] = sColor_[1][2];
			}
			Text("postEffect1");
			if (RadioButton("none1", postEffect_[1] == P_NONE)) {
				postEffect_[1] = P_NONE;
			}
			if (RadioButton("bloom1", postEffect_[1] == P_BLOOM)) {
				postEffect_[1] = P_BLOOM;
			}
			if (RadioButton("cross1", postEffect_[1] == P_CROSS)) {
				postEffect_[1] = P_CROSS;
			}
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
				Checkbox("revers2", &isRevers_[2]);
				InputFloat3("move2", move_[2]);
				InputFloat3("moveRand2", moveRand_[2]);
				InputFloat("speed2", &speed_[2]);
				InputFloat3("acceleration2", acceleration_[2]);
				InputFloat2("s_scale->e_scale2", scale_[2]);
				SliderFloat("angleSpeed2", &angleSpeed_[2], -10, 10, "%.1f");
				SliderFloat("angleSpeedRand2", &angleSpeedRand_[2], -10, 10, "%.1f");
				InputFloat3("gravity2", gravity_[2]);
				ColorEdit3("s_color2", sColor_[2]);
				ColorEdit3("e_color2", eColor_[2]);
				if (Button("UseSameColor", { 100,30 })) {
					eColor_[2][0] = sColor_[2][0];
					eColor_[2][1] = sColor_[2][1];
					eColor_[2][2] = sColor_[2][2];
				}
				Text("postEffect2");
				if (RadioButton("none2", postEffect_[2] == P_NONE)) {
					postEffect_[2] = P_NONE;
				}
				if (RadioButton("bloom2", postEffect_[2] == P_BLOOM)) {
					postEffect_[2] = P_BLOOM;
				}
				if (RadioButton("cross2", postEffect_[2] == P_CROSS)) {
					postEffect_[2] = P_CROSS;
				}
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

	if (GetIsEdit(0)) {
		Begin("ParticleList");

		for (int i = 0; i < 100; i++) {
			//nullの時は飛ばす
			if (GetParticleName(i) == "null") {
				break;
			}

			if (Button(GetParticleName(i).c_str(), { 100,30 })) {
				std::string temp = GetParticleName(i).c_str();
				for (int j = 0; j < 16; j++) {
					text_[j] = temp[j];
					if (temp[j] == '\0') {
						break;
					}
				}
				Load();
			}
		}

		End();
	}

#endif 
}

void ParticleEditor::PopParticle(uint8_t num)
{
	//ランダム
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
			ParticleManager::GetInstance()->AddCircle(int(life_[num] + int32_t(l(engine))), { pos_[num][0] + xp(engine),pos_[num][1] + 5 + yp(engine),pos_[num][2] + zp(engine) }, isRevers_[num], {move_[num][0] + xm(engine),move_[num][1] + ym(engine),move_[num][2] + zm(engine)}, speed_[num], {acceleration_[num][0],acceleration_[num][1],acceleration_[num][2]}, {gravity_[num][0],gravity_[num][1],gravity_[num][2]}, scale_[num][0], scale_[num][1], {sColor_[num][0],sColor_[num][1],sColor_[num][2]}, {eColor_[num][0],eColor_[num][1],eColor_[num][2]},postEffect_[num]);
		}
		if (texNum_[num] == 1) {

		}
	}
}

void ParticleEditor::Save()
{
	Write();

	std::string temp = text_;
	temp = "Resources/ParticleData/" + temp + ".dat";
	fopen_s(&saveFile_, temp.c_str(), "wb");

	fwrite(&editData_, sizeof(editData_), 1, saveFile_);

	fclose(saveFile_);

	for (int i = 0; i < 100; i++) {

		if (particleName[i] == "null" || particleName[i] == text_ || particleName[i] == "a") {
			particleName[i] = text_;
			break;
		}
	}

	fopen_s(&saveFile_,"Resources/ParticleData/particleName.dat","wb");
	fwrite(&particleName,sizeof(particleName),1,saveFile_);
	fclose(saveFile_);

	SetParticleData();
}

void ParticleEditor::Load()
{
	std::string temp = text_;
	temp = "Resources/ParticleData/" + temp + ".dat";
	fopen_s(&saveFile_, temp.c_str(), "rb");
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
	editData_.isRevers = isRevers_[0];
	editData_.move = { move_[0][0],move_[0][1],move_[0][2] };
	editData_.moveRand = { moveRand_[0][0],moveRand_[0][1],moveRand_[0][2] };
	editData_.speed = speed_[0];
	editData_.acceleration = { acceleration_[0][0],acceleration_[0][1],acceleration_[0][2] };
	editData_.scale = { scale_[0][0],scale_[0][1] };
	editData_.angleSpeed = angleSpeed_[0];
	editData_.angleSpeedRand = angleSpeedRand_[0];
	editData_.gravity = { gravity_[0][0],gravity_[0][1],gravity_[0][2] };
	editData_.sColor = { sColor_[0][0],sColor_[0][1],sColor_[0][2] };
	editData_.eColor = { eColor_[0][0],eColor_[0][1],eColor_[0][2] };
	editData_.postEffect = postEffect_[0];
	editData_.add1 = GetIsEdit(1);
	editData_.texNum1 = texNum_[1];
	editData_.num1 = num_[1];
	editData_.life1 = life_[1];
	editData_.lifeRand1 = lifeRand_[1];
	editData_.pos1 = { pos_[1][0],pos_[1][1] ,pos_[1][2] };
	editData_.posRand1 = { posRand_[1][0],posRand_[1][1] ,posRand_[1][2] };
	editData_.isRevers1 = isRevers_[1];
	editData_.move1 = { move_[1][0],move_[1][1],move_[1][2] };
	editData_.moveRand1 = { moveRand_[1][0],moveRand_[1][1],moveRand_[1][2] };
	editData_.speed1 = speed_[1];
	editData_.acceleration1 = { acceleration_[1][0],acceleration_[1][1],acceleration_[1][2] };
	editData_.scale1 = { scale_[1][0],scale_[1][1] };
	editData_.angleSpeed1 = angleSpeed_[1];
	editData_.angleSpeedRand1 = angleSpeedRand_[1];
	editData_.gravity1 = { gravity_[1][0],gravity_[1][1],gravity_[1][2] };
	editData_.sColor1 = { sColor_[1][0],sColor_[1][1],sColor_[1][2] };
	editData_.eColor1 = { eColor_[1][0],eColor_[1][1],eColor_[1][2] };
	editData_.postEffect1 = postEffect_[1];
	editData_.add2 = GetIsEdit(2);
	editData_.texNum2 = texNum_[2];
	editData_.num2 = num_[2];
	editData_.life2 = life_[2];
	editData_.lifeRand2 = lifeRand_[2];
	editData_.pos2 = { pos_[2][0],pos_[2][1] ,pos_[2][2] };
	editData_.posRand2 = { posRand_[2][0],posRand_[2][1] ,posRand_[2][2] };
	editData_.isRevers2 = isRevers_[2];
	editData_.move2 = { move_[2][0],move_[2][1],move_[2][2] };
	editData_.moveRand2 = { moveRand_[2][0],moveRand_[2][1],moveRand_[2][2] };
	editData_.speed2 = speed_[2];
	editData_.acceleration2 = { acceleration_[2][0],acceleration_[2][1],acceleration_[2][2] };
	editData_.scale2 = { scale_[2][0],scale_[2][1] };
	editData_.angleSpeed2 = angleSpeed_[2];
	editData_.angleSpeedRand2 = angleSpeedRand_[2];
	editData_.gravity2 = { gravity_[2][0],gravity_[2][1],gravity_[2][2] };
	editData_.sColor2 = { sColor_[2][0],sColor_[2][1],sColor_[2][2] };
	editData_.eColor2 = { eColor_[2][0],eColor_[2][1],eColor_[2][2] };
	editData_.postEffect2 = postEffect_[2];
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
	isRevers_[0] = editData_.isRevers;
	move_[0][0] = editData_.move.x;
	move_[0][1] = editData_.move.y;
	move_[0][2] = editData_.move.z;
	moveRand_[0][0] = editData_.moveRand.x;
	moveRand_[0][1] = editData_.moveRand.y;
	moveRand_[0][2] = editData_.moveRand.z;
	speed_[0] = editData_.speed;
	acceleration_[0][0] = editData_.acceleration.x;
	acceleration_[0][1] = editData_.acceleration.y;
	acceleration_[0][2] = editData_.acceleration.z;
	scale_[0][0] = editData_.scale.x;
	scale_[0][1] = editData_.scale.y;
	angleSpeed_[0] = editData_.angleSpeed;
	angleSpeedRand_[0] = editData_.angleSpeedRand;
	gravity_[0][0] = editData_.gravity.x;
	gravity_[0][1] = editData_.gravity.y;
	gravity_[0][2] = editData_.gravity.z;
	sColor_[0][0] = editData_.sColor.x;
	sColor_[0][1] = editData_.sColor.y;
	sColor_[0][2] = editData_.sColor.z;
	eColor_[0][0] = editData_.eColor.x;
	eColor_[0][1] = editData_.eColor.y;
	eColor_[0][2] = editData_.eColor.z;
	postEffect_[0] = editData_.postEffect;

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
	isRevers_[1] = editData_.isRevers1;
	move_[1][0] = editData_.move1.x;
	move_[1][1] = editData_.move1.y;
	move_[1][2] = editData_.move1.z;
	moveRand_[1][0] = editData_.moveRand1.x;
	moveRand_[1][1] = editData_.moveRand1.y;
	moveRand_[1][2] = editData_.moveRand1.z;
	speed_[1] = editData_.speed1;
	acceleration_[1][0] = editData_.acceleration1.x;
	acceleration_[1][1] = editData_.acceleration1.y;
	acceleration_[1][2] = editData_.acceleration1.z;
	scale_[1][0] = editData_.scale1.x;
	scale_[1][1] = editData_.scale1.y;
	angleSpeed_[1] = editData_.angleSpeed1;
	angleSpeedRand_[1] = editData_.angleSpeedRand1;
	gravity_[1][0] = editData_.gravity1.x;
	gravity_[1][1] = editData_.gravity1.y;
	gravity_[1][2] = editData_.gravity1.z;
	sColor_[1][0] = editData_.sColor1.x;
	sColor_[1][1] = editData_.sColor1.y;
	sColor_[1][2] = editData_.sColor1.z;
	eColor_[1][0] = editData_.eColor1.x;
	eColor_[1][1] = editData_.eColor1.y;
	eColor_[1][2] = editData_.eColor1.z;
	postEffect_[1] = editData_.postEffect1;

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
	isRevers_[2] = editData_.isRevers2;
	move_[2][0] = editData_.move2.x;
	move_[2][1] = editData_.move2.y;
	move_[2][2] = editData_.move2.z;
	moveRand_[2][0] = editData_.moveRand2.x;
	moveRand_[2][1] = editData_.moveRand2.y;
	moveRand_[2][2] = editData_.moveRand2.z;
	speed_[2] = editData_.speed2;
	acceleration_[2][0] = editData_.acceleration2.x;
	acceleration_[2][1] = editData_.acceleration2.y;
	acceleration_[2][2] = editData_.acceleration2.z;
	scale_[2][0] = editData_.scale2.x;
	scale_[2][1] = editData_.scale2.y;
	angleSpeed_[2] = editData_.angleSpeed2;
	angleSpeedRand_[2] = editData_.angleSpeedRand2;
	gravity_[2][0] = editData_.gravity2.x;
	gravity_[2][1] = editData_.gravity2.y;
	gravity_[2][2] = editData_.gravity2.z;
	sColor_[2][0] = editData_.sColor2.x;
	sColor_[2][1] = editData_.sColor2.y;
	sColor_[2][2] = editData_.sColor2.z;
	eColor_[2][0] = editData_.eColor2.x;
	eColor_[2][1] = editData_.eColor2.y;
	eColor_[2][2] = editData_.eColor2.z;
	postEffect_[2] = editData_.postEffect2;
}

void ParticleEditor::SetParticleData()
{
	FILE* saveFile;
	EditFile tempFile;

	for (int i = 0; i < 100; i++) {
		//全てのパーティクルを読み込む
		if (GetParticleName(i) == "null") {
			break;
		}

		std::string temp = "Resources/ParticleData/" + GetParticleName(i) + ".dat";
		fopen_s(&saveFile, temp.c_str(), "rb");
		if (saveFile == NULL) {
			return;
		}
		fread(&tempFile, sizeof(tempFile), 1, saveFile);
		fclose(saveFile);

		ParticleManager::GetInstance()->SetParticleData(i,tempFile);
	}
}
