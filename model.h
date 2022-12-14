#pragma once
#include <Windows.h>
#include "WorldTransform.h"
#include "DXCommon.h"
#include "GrovalSetting.h"
#include <wrl.h>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

class Model
{
public:
	//エイリアステンプレート
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

private:
	//デバイス
	static ID3D12Device* device_;
	static UINT descriptorSize_;

public:
	static void StaticInitialize(ID3D12Device* device);

	static Model* Create();

public:
	void Initialize(const string& filename);

	void Draw(ID3D12GraphicsCommandList* cmdList);

};

