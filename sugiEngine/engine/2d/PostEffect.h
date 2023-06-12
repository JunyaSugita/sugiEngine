#pragma once
#include "Sprite.h"

class PostEffect : public Sprite
{
public:
	PostEffect();

	void Initialize();
	void Draw(ID3D12GraphicsCommandList* cmdList);

private:
	ComPtr<ID3D12Resource> texBuff;
	ComPtr<ID3D12DescriptorHeap>descHeapSRV;
};

