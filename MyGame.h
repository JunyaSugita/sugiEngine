#pragma once
#include "SugiFramework.h"



class MyGame : public SugiFramework
{
public:

	void Initialize() override;

	void Finalize() override;

	void Update() override;

	void Draw() override;

private:
	std::unique_ptr <GameManager> gameM = nullptr;
	std::unique_ptr <ImGuiManager> imGuiManager = nullptr;
};

