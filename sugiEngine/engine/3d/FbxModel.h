#pragma once
#include <string>
#include <DirectXMath.h>
#include <vector>

struct Node 
{
	std::string name;

	DirectX::XMVECTOR scaling = { 1,1,1,0 };
	DirectX::XMVECTOR rotation = { 0,0,0,0 };
	DirectX::XMVECTOR  translation = { 0,0,0,1 };
	DirectX::XMMATRIX transform;
	DirectX::XMMATRIX globalTransform;

	//親ノード
	Node* parent = nullptr;
};

class FbxModel
{
public:
	friend class FbxLoader;
private:
	std::string name;
	//ノード配列
	std::vector<Node> nodes;
};

