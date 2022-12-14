#include "Model.h"
using namespace Microsoft::WRL;
using namespace std;

ID3D12Device* Model::device_ = nullptr;
UINT Model::descriptorSize_ = 0;

void Model::StaticInitialize(ID3D12Device* device)
{
	device_ = device;
}

Model* Model::Create()
{
	Model* instance = new Model;

	return instance;
}

void Model::Initialize(const string& filename)
{
}

void Model::Draw(ID3D12GraphicsCommandList* cmdList)
{
}
