#include "SpellManager.h"
#include "Input.h"
#include "ImGuiManager.h"

using namespace ImGui;

SpellManager::SpellManager()
{
}

SpellManager::~SpellManager()
{
}

SpellManager* SpellManager::GetInstance()
{
	static SpellManager instance;

	return &instance;
}

void SpellManager::Initialize()
{
	FireBall::OneTimeInitialize();
	for (unique_ptr<FireBall>& fireBall : fireBalls_) {
		fireBall->SetIsDead();
	}

}

void SpellManager::Update()
{
#pragma region �C���X�^���X�Ăяo��
	//�C���X�^���X�Ăяo��
	Input* input = Input::GetInstance();
#pragma endregion

#pragma region ���@�̍폜
	//�����t���O�̗��������@�̍폜
	fireBalls_.remove_if([](unique_ptr<FireBall>& fireBall) {
		return fireBall->GetIsDead();
		});
#pragma endregion

#pragma region ���@�̃A�b�v�f�[�g
	//�e���@��Update
	for (unique_ptr<FireBall>& fireBall : fireBalls_) {
		fireBall->Update();
	}
#pragma endregion
}

void SpellManager::Draw()
{
	for (unique_ptr<FireBall>& fireBall : fireBalls_) {
		fireBall->Draw();
	}
}

void SpellManager::ChargeFireBall()
{
	if (chargeTime < TIME_CHARGE_FIREBALL) {
		chargeTime++;
	}

	Begin("FireBallState");
	Text("Charge %f", chargeTime / TIME_CHARGE_FIREBALL);
	End();

	Camera* camera = Camera::GetInstance();

	if (Input::GetInstance()->ReleaseKey(DIK_1)) {
		if (int(chargeTime / TIME_CHARGE_FIREBALL)) {
			unique_ptr<FireBall> newSpell = make_unique<FireBall>();
			newSpell->Initialize(camera->GetEye(), camera->GetTarget() - camera->GetEye());
			newSpell->Fire();

			fireBalls_.push_back(move(newSpell));

		}
		chargeTime = 0;
	}
}
