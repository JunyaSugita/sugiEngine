
#include "ParticleEditor.h"
#include "ImGuiManager.h"
#include "ParticleManager.h"
#include <random>
#include "Camera.h"
#include "stdio.h"

using namespace ImGui;

void ParticleEditor::Initialize()
{
	isEdit_ = false;
	version_ = 0;
	isTextureRand_ = false;
	texNum_ = 0;
	texNumRand_ = 0;
	num_ = 1;
	life_ = 60;
	lifeRand_ = 0;
	pos_[0] = pos_[1] = pos_[2] = 0.0f;
	posRand_[0] = posRand_[1] = posRand_[2] = 0.0f;
	isRevers_ = false;
	move_[0] = move_[1] = move_[2] = 0.0f;
	moveRand_[0] = moveRand_[1] = moveRand_[2] = 0.0f;
	speed_ = 100.0f;
	acceleration_[0] = acceleration_[1] = acceleration_[2] = 1.0f;
	checkS_[0] = checkS_[1] = 0;
	scale_[0] = scale_[1] = scale_[2] = 1.0f;
	scale_[3] = 0;
	angleSpeed_ = 0;
	angleSpeedRand_ = 0;
	gravity_[0] = gravity_[1] = gravity_[2] = 0;
	sColor_[0] = sColor_[1] = sColor_[2] = sColor_[3] = 1.0f;
	check1_ = 0.3f;
	check1Color_[0] = check1Color_[1] = check1Color_[2] = check1Color_[3] = 1.0f;
	check2_ = 0.6f;
	check2Color_[0] = check2Color_[1] = check2Color_[2] = check2Color_[3] = 1.0f;
	eColor_[0] = eColor_[1] = eColor_[2] = eColor_[3] = 1.0f;
	postEffect_ = 0;
	Write();

	//パーティクルの名前読み込み
	fopen_s(&saveFile_, "Resources/ParticleData/particleName.dat", "rb");
	if (saveFile_ == NULL) {
		return;
	}
	fread(&particleName_, sizeof(particleName_), 1, saveFile_);
	fclose(saveFile_);

	SetParticleData();

	for (int i = 0; i < 100; i++) {
		isPop_[i] = false;
	}
	isP_ = true;
	isOnePop_ = false;
}

void ParticleEditor::Update()
{
#ifdef _DEBUG
	if ((GetIsEdit() && isP_) || isOnePop_) {
		PopParticle();
		isOnePop_ = false;
	}

	Begin("ParticleEditor");
	if (Button("EditMode", { 100,30 })) {
		isEdit_ = (isEdit_ + 1) % 2;
	}
	if (GetIsEdit()) {
		InputText("fileName", text_, sizeof(text_));
		if (Button("Save", { 100,30 })) {
			Save();
		}
		if (Button("Delete", { 100,30 })) {
			Delete();
		}
		Checkbox("pop", &isP_);
		if (!isP_) {
			if (Button("oneTimePop", { 100,30 })) {
				isOnePop_ = true;
			}
		}
		Checkbox("randTex", &isTextureRand_);
		InputInt("texNum", &texNum_);
		if (isTextureRand_) {
			InputInt("texNumRand", &texNumRand_);
		}
		else {
			texNumRand_ = texNum_;
		}
		InputInt("num", &num_);
		InputInt("life", &life_);
		InputInt("lifeRand", &lifeRand_);
		InputFloat3("pos", pos_);
		InputFloat3("posRand", posRand_);
		Checkbox("revers", &isRevers_);
		InputFloat3("move", move_);
		InputFloat3("moveRand", moveRand_);
		InputFloat("speed", &speed_);
		InputFloat3("acceleration", acceleration_);
		SliderFloat2("checkS", checkS_, 0.0f, 1.0f);
		InputFloat4("s_scale->e_scale", scale_);
		SliderFloat("angleSpeed", &angleSpeed_, -10, 10, "%.1f");
		SliderFloat("angleSpeedRand", &angleSpeedRand_, -10, 10, "%.1f");
		InputFloat3("gravity", gravity_);
		ColorEdit4("s_color", sColor_);
		SliderFloat("check1", &check1_, 0.0f, 1.0f);
		ColorEdit4("check1Color", check1Color_);
		SliderFloat("check2", &check2_, 0.0f, 1.0f);
		ColorEdit4("check2Color", check2Color_);
		ColorEdit4("e_color", eColor_);
		if (Button("UseSameColor", { 100,30 })) {
			check1Color_[0] = sColor_[0];
			check1Color_[1] = sColor_[1];
			check1Color_[2] = sColor_[2];
			check1Color_[3] = sColor_[3];
			check2Color_[0] = sColor_[0];
			check2Color_[1] = sColor_[1];
			check2Color_[2] = sColor_[2];
			check2Color_[3] = sColor_[3];
			eColor_[0] = sColor_[0];
			eColor_[1] = sColor_[1];
			eColor_[2] = sColor_[2];
			eColor_[3] = sColor_[3];
		}
		Text("postEffect");
		if (RadioButton("none", postEffect_ == P_NONE)) {
			postEffect_ = P_NONE;
		}
		if (RadioButton("bloom", postEffect_ == P_BLOOM)) {
			postEffect_ = P_BLOOM;
		}
		if (RadioButton("cross", postEffect_ == P_CROSS)) {
			postEffect_ = P_CROSS;
		}

		Begin("ParticleList");
		for (int i = 0; i < 100; i++) {
			std::string tampStr = GetParticleName(i);

			//nullの時は飛ばす
			if (tampStr == "null") {
				break;
			}

			if (Button(GetParticleName(i), { 100,30 })) {
				std::string temp = GetParticleName(i);
				for (int j = 0; j < 16; j++) {
					text_[j] = temp[j];
					if (temp[j] == '\0') {
						break;
					}
				}
				Load();
			}
			char str[16];
			sprintf_s(str, "pop:%d", i);
			Checkbox(str, &isPop_[i]);
			if (isPop_[i]) {
				ParticleManager::GetInstance()->AddFromFile(i, { 0,5,0 }, true);
			}
		}
		End();
	}
	End();
#endif 
}



void ParticleEditor::PopParticle()
{
	//ランダム
	std::random_device seed_gen;
	std::mt19937_64 engine(seed_gen());

	for (int i = 0; i < num_; i++) {
		std::uniform_real_distribution<float> xp(-posRand_[0], posRand_[0]);
		std::uniform_real_distribution<float> yp(-posRand_[1], posRand_[1]);
		std::uniform_real_distribution<float> zp(-posRand_[2], posRand_[2]);

		std::uniform_real_distribution<float> xm(-moveRand_[0], moveRand_[0]);
		std::uniform_real_distribution<float> ym(-moveRand_[1], moveRand_[1]);
		std::uniform_real_distribution<float> zm(-moveRand_[2], moveRand_[2]);

		std::uniform_real_distribution<float> l(-float(lifeRand_ + 0.99f), float(lifeRand_ + 0.99f));

		if (texNum_ >= PARTICLE_TEXTURE_END) {
			texNum_ = PARTICLE_TEXTURE_END - 1;
		}
		if (texNumRand_ >= PARTICLE_TEXTURE_END) {
			texNumRand_ = PARTICLE_TEXTURE_END - 1;
		}
		float min = Min((float)texNum_, (float)texNumRand_);
		float max = Max((float)texNum_, (float)texNumRand_);
		std::uniform_real_distribution<float> tex(min, max + 0.99f);

		ParticleManager::GetInstance()->AddCircle((int)tex(engine), int(life_ + int32_t(l(engine))), { pos_[0] + xp(engine),pos_[1] + 5 + yp(engine),pos_[2] + zp(engine) }, isRevers_, { move_[0] + xm(engine),move_[1] + ym(engine),move_[2] + zm(engine) }, speed_, { acceleration_[0],acceleration_[1],acceleration_[2] }, { gravity_[0],gravity_[1],gravity_[2] }, { checkS_[0],checkS_[1] }, { scale_[0], scale_[1],scale_[2],scale_[3] }, { sColor_[0],sColor_[1],sColor_[2],sColor_[3] }, check1_, { check1Color_[0],check1Color_[1],check1Color_[2],check1Color_[3] }, check2_, { check2Color_[0],check2Color_[1], check2Color_[2], check2Color_[3] }, { eColor_[0],eColor_[1],eColor_[2],eColor_[3] }, postEffect_);
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
		std::string tempStr = particleName_[i];

		if (tempStr == "null" || tempStr == text_) {
			for (int j = 0; j < 16; j++) {
				particleName_[i][j] = text_[j];
				if (text_[j] == '\0') {
					break;
				}
			}
			break;
		}
	}

	fopen_s(&saveFile_, "Resources/ParticleData/particleName.dat", "wb");
	fwrite(&particleName_, sizeof(particleName_), 1, saveFile_);
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
	//現在のバージョンを書く
	editData_.version = 0;

	editData_.isRandTex = isTextureRand_;
	editData_.texNum = texNum_;
	editData_.randTexNum = texNumRand_;
	editData_.num = num_;
	editData_.life = life_;
	editData_.lifeRand = lifeRand_;
	editData_.pos = { pos_[0],pos_[1] ,pos_[2] };
	editData_.posRand = { posRand_[0],posRand_[1] ,posRand_[2] };
	editData_.isRevers = isRevers_;
	editData_.move = { move_[0],move_[1],move_[2] };
	editData_.moveRand = { moveRand_[0],moveRand_[1],moveRand_[2] };
	editData_.speed = speed_;
	editData_.acceleration = { acceleration_[0],acceleration_[1],acceleration_[2] };
	editData_.checkS = { checkS_[0],checkS_[1] };
	editData_.scale = { scale_[0],scale_[1],scale_[2],scale_[3] };
	editData_.angleSpeed = angleSpeed_;
	editData_.angleSpeedRand = angleSpeedRand_;
	editData_.gravity = { gravity_[0],gravity_[1],gravity_[2] };
	editData_.sColor = { sColor_[0],sColor_[1],sColor_[2],sColor_[3] };
	editData_.check1 = check1_;
	editData_.check1Color = { check1Color_[0],check1Color_[1],check1Color_[2],check1Color_[3] };
	editData_.check2 = check2_;
	editData_.check2Color = { check2Color_[0],check2Color_[1],check2Color_[2],check2Color_[3] };
	editData_.eColor = { eColor_[0],eColor_[1],eColor_[2],eColor_[3] };
	editData_.postEffect = postEffect_;
}

void ParticleEditor::Read()
{
	isTextureRand_ = editData_.isRandTex;
	texNum_ = editData_.texNum;
	texNumRand_ = editData_.randTexNum;
	num_ = editData_.num;
	life_ = editData_.life;
	lifeRand_ = editData_.lifeRand;
	pos_[0] = editData_.pos.x;
	pos_[1] = editData_.pos.y;
	pos_[2] = editData_.pos.z;
	posRand_[0] = editData_.posRand.x;
	posRand_[1] = editData_.posRand.y;
	posRand_[2] = editData_.posRand.z;
	isRevers_ = editData_.isRevers;
	move_[0] = editData_.move.x;
	move_[1] = editData_.move.y;
	move_[2] = editData_.move.z;
	moveRand_[0] = editData_.moveRand.x;
	moveRand_[1] = editData_.moveRand.y;
	moveRand_[2] = editData_.moveRand.z;
	speed_ = editData_.speed;
	acceleration_[0] = editData_.acceleration.x;
	acceleration_[1] = editData_.acceleration.y;
	acceleration_[2] = editData_.acceleration.z;
	checkS_[0] = editData_.checkS.x;
	checkS_[1] = editData_.checkS.y;
	scale_[0] = editData_.scale.x;
	scale_[1] = editData_.scale.y;
	scale_[2] = editData_.scale.z;
	scale_[3] = editData_.scale.w;
	angleSpeed_ = editData_.angleSpeed;
	angleSpeedRand_ = editData_.angleSpeedRand;
	gravity_[0] = editData_.gravity.x;
	gravity_[1] = editData_.gravity.y;
	gravity_[2] = editData_.gravity.z;
	sColor_[0] = editData_.sColor.x;
	sColor_[1] = editData_.sColor.y;
	sColor_[2] = editData_.sColor.z;
	sColor_[3] = editData_.sColor.w;
	check1_ = editData_.check1;
	check1Color_[0] = editData_.check1Color.x;
	check1Color_[1] = editData_.check1Color.y;
	check1Color_[2] = editData_.check1Color.z;
	check1Color_[3] = editData_.check1Color.w;
	check2_ = editData_.check2;
	check2Color_[0] = editData_.check2Color.x;
	check2Color_[1] = editData_.check2Color.y;
	check2Color_[2] = editData_.check2Color.z;
	check2Color_[3] = editData_.check2Color.w;
	eColor_[0] = editData_.eColor.x;
	eColor_[1] = editData_.eColor.y;
	eColor_[2] = editData_.eColor.z;
	eColor_[3] = editData_.eColor.w;
	postEffect_ = editData_.postEffect;
}

void ParticleEditor::Delete()
{
	std::string nullStr = "null";

	for (int i = 0; i < 100; i++) {
		std::string temp = particleName_[i];

		if (temp == nullStr) {
			break;
		}

		if (temp == text_) {
			for (int j = i; j < 99; j++) {
				for (int k = 0; k < 16; k++) {
					particleName_[j][k] = particleName_[j + 1][k];
				}
				if (temp == nullStr) {
					break;
				}
			}
			for (int k = 0; k < 16; k++) {
				particleName_[99][k] = nullStr[k];
				if (nullStr[k] == '\0') {
					break;
				}
			}
			break;
		}
	}

	fopen_s(&saveFile_, "Resources/ParticleData/particleName.dat", "wb");
	fwrite(&particleName_, sizeof(particleName_), 1, saveFile_);
	fclose(saveFile_);

	SetParticleData();
}

void ParticleEditor::SetParticleData()
{
	EditFile tempFile;

	for (int i = 0; i < 100; i++) {
		std::string name = particleName_[i];

		//全てのパーティクルを読み込む
		if (name == "null") {
			break;
		}

		std::string temp = "Resources/ParticleData/" + name + ".dat";
		fopen_s(&saveFile_, temp.c_str(), "rb");
		if (saveFile_ == NULL) {
			return;
		}
		fread(&tempFile, sizeof(tempFile), 1, saveFile_);
		fclose(saveFile_);

		ParticleManager::GetInstance()->SetParticleData(i, tempFile);
	}
}
