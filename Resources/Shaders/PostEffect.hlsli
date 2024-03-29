//マテリアル
cbuffer ConstBufferDataMaterial : register(b0) {
	float4 color;
}

//3D変換行列
cbuffer ConstBufferDataTransform : register(b1) {
	matrix mat;
}

//3D変換行列
cbuffer ConstBufferDataEffect : register(b2) {
	int blur;
    int Isigma;
	int invertColor;
	int border;
	int gray;
	int bloom;
	int closs4;
	int closs6;
	int closs8;
	int moveCloss;
}

//頂点シェーダーの出力構造体
struct VSOutput {
	//システム用頂点座標
	float4 svpos:SV_POSITION;
	//uv値
	float2 uv :TEXCOORD;
};