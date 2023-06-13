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
#include "FbxLoader.h"
#include "Fbx.h"
#include "PostEffect.h"

class SugiFramework {
public:
	virtual ~SugiFramework() = default;

	virtual void Initialize();

	virtual void Finalize();

	virtual void Update();

	virtual void Draw() = 0;

	void Run();

	bool GetIsEnd();

protected:
	unique_ptr<WinApp> winApp_ = nullptr;
	unique_ptr<DXCommon> dxCom_ = nullptr;
	unique_ptr <Matrix4> matrix4_ = nullptr;
	unique_ptr <PostEffect> postEffect = nullptr;
};