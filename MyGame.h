#pragma once
#include <Windows.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <cassert>
#include <vector>
#include <string>
#include <memory>
#include <DirectXMath.h>
#include <d3dcompiler.h>
#include <DirectXTex.h>
#include "Vector2.h"
#include "Vector3.h"
#include "Matrix4.h"
#include "WorldTransform.h"
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

#include "GrovalSetting.h"
#include "WinApp.h"
#include "DXCommon.h"
#include"Input.h"
#include "Object3d.h"
#include "Sprite.h"
#include "Model.h"
#include "GameManager.h"
#include "LightGroup.h"
#include "ImGuiManager.h"

class MyGame
{
public:

	void Initialize();

	void Finalize();

	void Update();

	void Draw();

	bool GetIsEnd();

private:
	std::unique_ptr<WinApp> winApp = nullptr;
	std::unique_ptr<DXCommon> dxCom = nullptr;
	std::unique_ptr<Input> input = nullptr;
	std::unique_ptr <Matrix4> matrix4 = nullptr;
	std::unique_ptr <ImGuiManager> imGuiManager = nullptr;
	std::unique_ptr <GameManager> gameM = nullptr;

};

