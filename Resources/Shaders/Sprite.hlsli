//マテリアル
cbuffer ConstBufferDataMaterial : register(b0) {
	float4 color;
}

//3D変換行列
cbuffer ConstBufferDataTransform : register(b1) {
	matrix mat;
}

//頂点シェーダーの出力構造体
struct VSOutput {
	//システム用頂点座標
	float4 svpos:SV_POSITION;
	//uv値
	float2 uv :TEXCOORD;
};