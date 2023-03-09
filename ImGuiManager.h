#pragma once
#include "WinApp.h"
#include "DXCommon.h"
#include <imgui.h>

class ImGuiManager
{
public:
	/// <summary>
	/// ������
	/// </summary>
	/// <param name="winApp">WinAppn�̃|�C���^</param>
	void Initialie(WinApp* winApp,DXCommon* dxCom);

	void Text(const char* text,...);

private:
	//SRV�p�f�X�N���v�^�q�[�v
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> srvHeap_;

};

