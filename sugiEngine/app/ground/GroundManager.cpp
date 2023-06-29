#include "GroundManager.h"

using namespace std;

GroundManager::GroundManager()
{
}

GroundManager::~GroundManager()
{
}

GroundManager* GroundManager::GetInstance()
{
	static GroundManager instance;

	return &instance;
}

void GroundManager::Initialize()
{
	Ground::OneTimeInitialize();

	for (int i = 0; i < 25; i++) {
		grounds_[i] = make_unique<Ground>();
	}
	for (int i = -2; i < 3; i++) {
		for (int j = -2; j < 3; j++) {
			int num = ((i + 2) * 5) + (j + 2);
			grounds_[num]->Initialize({i * 20.0f,0,j * 20.0f});
		}
	}
}

void GroundManager::Update()
{
	for (int i = 0; i < 25; i++) {
		grounds_[i]->Update();
	}
}

void GroundManager::Draw()
{
	for (int i = 0; i < 25; i++) {
		grounds_[i]->Draw();
	}
}
