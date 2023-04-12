#include <DirectXMath.h>
#include <Windows.h>
#include <d3d12.h>
#include <d3dx12.h>
#include <vector>
#pragma once

class DirectionalLight
{
private:
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

public:
	struct ConstBufferData {
		XMVECTOR lightv;
		XMFLOAT3 lightcolor;
		unsigned int active;
	};

public:
	//セッター
	inline void SetLightDir(const XMVECTOR& lightdir) {
		lightdir_ = lightdir;
	};
	inline void SetLightColor(const XMFLOAT3& lightcolor) {
		lightcolor_ = lightcolor;
	};

	inline void SetActive(bool active) {
		active_ = active;
	}

	//ゲッター
	inline XMVECTOR GetLightDir() {
		return lightdir_;
	};
	inline XMFLOAT3 GetLightColor() {
		return lightcolor_;
	};

	inline bool GetIsActive() {
		return active_;
	}


private:
	XMVECTOR lightdir_ = { 1,0,0,0 };
	XMFLOAT3 lightcolor_ = { 1,1,1 };

	//有効フラグ
	bool active_ = false;

};

