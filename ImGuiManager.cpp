#include "ImGuiManager.h"
#include <imgui_impl_win32.h>


void ImGuiManager::Initialie(WinApp* winApp)
{
	//ImGuiのコンテキストを生成
	ImGui::CreateContext();
	//ImGuiのスタイル
	ImGui::StyleColorsDark();

	ImGui_ImplWin32_Init(winApp->GetHWND());
}
