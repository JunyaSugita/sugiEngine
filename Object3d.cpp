#include "Object3d.h"

void Object3d::Initialize(DXCommon* dxCom)
{
	this->dxCom = dxCom;
	HRESULT result;

	//ヒープ設定
	D3D12_HEAP_PROPERTIES cbHeapProp{};
	cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;		//GPUへの転送用
	//リソース設定
	D3D12_RESOURCE_DESC cbResourceDesc{};
	cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	cbResourceDesc.Width = (sizeof(ConstBufferDataTransform) + 0xff) & ~0xff;	//256バイトアライメント
	cbResourceDesc.Height = 1;
	cbResourceDesc.DepthOrArraySize = 1;
	cbResourceDesc.MipLevels = 1;
	cbResourceDesc.SampleDesc.Count = 1;
	cbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//定数バッファの生成
	result = dxCom->device->CreateCommittedResource(
		&cbHeapProp,		//ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&cbResourceDesc,	//リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffTransform)
	);
	assert(SUCCEEDED(result));

	//定数バッファのマッピング
	result = constBuffTransform->Map(0, nullptr, (void**)&constMapTransform);	//マッピング
	assert(SUCCEEDED(result));

	//単位行列を代入
	WorldTransform a;
	//並行投影行列
	XMMATRIX ortho = XMMatrixOrthographicOffCenterLH(
		0.0f, WIN_WIDTH,
		WIN_HEIGHT, 0.0f,
		0.0f, 1.0f
	);

	//透視投影変換行列の計算
	XMMATRIX perspective = XMMatrixPerspectiveFovLH(
		XMConvertToRadians(45.0f),			//上下画角45度
		(float)WIN_WIDTH / WIN_HEIGHT,//アスペクト比
		0.1f, 1000.0f						//前端,奥端
	);
	//matrix4に変換
	matProjecsion = ConvertToMatrix4(perspective);

	//ビュー変換行列
	XMMATRIX xmmatView;
	XMFLOAT3 eye(0, 0, -100);	//視点座標
	XMFLOAT3 target(0, 0, 0);	//注視点座標
	XMFLOAT3 up(0, 1, 0);		//上方向ベクトル
	xmmatView = XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&target), XMLoadFloat3(&up));
	//matrix4に変換
	matView = ConvertToMatrix4(xmmatView);

	//ワールド変換行列
	worldTransform.scale = { 1.0f,1.0f,1.0f };
	worldTransform.rotation = { 0.0f,1.0f,1.0f };
	worldTransform.trans = { 0.0f,0.0f,0.0f };
	worldTransform.SetWorldMat();

	constMapTransform->mat = worldTransform.matWorld * matView * matProjecsion;
}

void Object3d::Update()
{
	worldTransform.SetWorldMat();
	constMapTransform->mat = worldTransform.matWorld * matView * matProjecsion;
}

void Object3d::Scale(float x, float y, float z)
{
	worldTransform.scale = { Vector3(x,y,z) };
}

void Object3d::Rotate(float x, float y, float z)
{
	worldTransform.rotation = { Vector3(x,y,z) };
}

void Object3d::Trans(float x, float y, float z)
{
	worldTransform.trans = { Vector3(x,y,z) };
}

void Object3d::Draw(uint16_t _countofIndices)
{
	//定数バッファビュー(CBV)の設定コマンド
	dxCom->commandList->SetGraphicsRootConstantBufferView(2, constBuffTransform->GetGPUVirtualAddress());

	dxCom->commandList->DrawIndexedInstanced(_countofIndices, 1, 0, 0, 0); // 全ての頂点を使って描画
}
