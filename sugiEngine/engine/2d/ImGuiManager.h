#pragma once
#include "WinApp.h"
#include "DXCommon.h"
#include <imgui.h>

class ImGuiManager final
{
private:
	ImGuiManager() = default;
	~ImGuiManager() = default;

public:
	//�R�s�[�R���X�g���N�^����
	ImGuiManager(const ImGuiManager& obj) = delete;
	//������Z�q�𖳌�
	ImGuiManager& operator=(const ImGuiManager& obj) = delete;

	static ImGuiManager* GetInstance();


public:
	/// <summary>
	/// ������
	/// </summary>
	/// <param name="winApp">WinAppn�̃|�C���^</param>
	void Initialize(WinApp* winApp,DXCommon* dxCom);

	/// <summary>
	/// �I��
	/// </summary>
	void Finalize();

	/// <summary>
	/// ImGui�X�^�[�g
	/// </summary>
	void Begin();

	/// <summary>
	/// ImGui�I���
	/// </summary>
	void End();

	/// <summary>
	/// ImGui�`��
	/// </summary>
	void Draw();

private:
	//SRV�p�f�X�N���v�^�q�[�v
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> srvHeap_;
	DXCommon* dxCom_ = nullptr;
};

