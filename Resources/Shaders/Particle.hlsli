//マテリアル
cbuffer ConstBufferDataMaterial : register(b0) {
	float4 color;
}

//3D変換行列
cbuffer ConstBuffB1 : register(b1) {
	matrix viewproj;
	matrix world;
	float3 cameraPos;
}

//頂点シェーダーの出力構造体
struct VSOutput {
	//システム用頂点座標
	float4 svpos:SV_POSITION;
	//uv値
	float2 uv :TEXCOORD;
};