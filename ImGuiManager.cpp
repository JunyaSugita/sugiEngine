#include "ImGuiManager.h"
#include <imgui_impl_win32.h>


void ImGuiManager::Initialie(WinApp* winApp)
{
	//ImGui�̃R���e�L�X�g�𐶐�
	ImGui::CreateContext();
	//ImGui�̃X�^�C��
	ImGui::StyleColorsDark();

	ImGui_ImplWin32_Init(winApp->GetHWND());
}
