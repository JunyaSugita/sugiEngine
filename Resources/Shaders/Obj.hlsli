cbuffer ConstBufferDataTransform : register(b0) {
	matrix mat;	//3D変換行列
}

cbuffer ConstBufferDataMaterial : register(b1) {
	float3 m_ambient : packoffset(c0);	//アンビエント
	float3 m_diffuse : packoffset(c1);	//ディフューズ係数
	float3 m_specular : packoffset(c2);	//スペキュラー係数
	float m_alpha : packoffset(c2.w);	//アルファ
	float4 m_color : packoffset(c3);
}

//(頂点シェーダーからピクセルシェーダーへのやり取りに使用)
struct VSOutput
{
	//システム用頂点座標
	float4 svpos : SV_POSITION;
	//法線ベクトル
	//float3 normal : NORMAL;

	float4 color : COLOR;
	//uv値
	float2 uv : TEXCOORD;
};