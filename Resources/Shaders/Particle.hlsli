//3D変換行列
cbuffer ConstBuffB1 : register(b1)
{
    matrix mat;
    matrix billboard;
}

//頂点シェーダーの出力構造体
struct VSOutput
{
	//システム用頂点座標
    float4 pos : POSITION;
    float scale : TEXCOORD;
    float4 color : COLOR;
};

//ジオメトリシェーダーの出力構造体
struct GSOutput
{
	//システム用頂点座標
    float4 svpos : SV_POSITION;
	//uv値
    float2 uv : TEXCOORD;
    
    float4 color : COLOR;
};