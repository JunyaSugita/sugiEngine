/**
 * @file SugiFramework.h
 * @brief フレームワーク
 */

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

#include "GlobalSetting.h"
#include "WinApp.h"
#include "DXCommon.h"
#include"Input.h"
#include "Sprite.h"
#include "Model.h"
#include "Object3d.h"
#include "GameManager.h"
#include "LightGroup.h"
#include "ImGuiManager.h"
#include "FbxLoader.h"
#include "Fbx.h"
#include "PostEffect.h"
#include "PostEffectSecond.h"
#include "Particle.h"

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
	std::unique_ptr<WinApp> winApp_ = nullptr;
	std::unique_ptr<DXCommon> dxCom_ = nullptr;
	std::unique_ptr <Matrix4> matrix4_ = nullptr;
	std::unique_ptr <PostEffect> postEffect = nullptr;
	std::unique_ptr <PostEffectSecond> postEffect2 = nullptr;
};
