cbuffer cbuff0 : register(b0)
{
	matrix viewproj;
	matrix world;
	float3 cameraPos;
};

cbuffer cbuff1 : register(b1)
{
	float3 m_ambient  : packoffset(c0); // アンビエント係数
	float3 m_diffuse  : packoffset(c1); // ディフューズ係数
	float3 m_specular : packoffset(c2); // スペキュラー係数
	float m_alpha : packoffset(c2.w);	// アルファ
};

//平行光
static const int DIRLIGHT_NUM = 3;
struct DirLight {
	float3 lightv;
	float3 lightcolor;
	uint active;
};

//点光源
static const int POINTLIGHT_NUM = 3;
struct PointLight {
	float3 lightpos;
	float3 lightcolor;
	float3 lightatten;
	uint active;
};

//スポットライト
static const int SPOTLIGHT_NUM = 3;
struct SpotLight {
	float3 lightv;
	float3 lightpos;
	float3 lightcolor;
	float3 lightatten;
	float2 lightfactoranglecos;
	uint active;
};

//丸影
static const int CIRCLESHADOW_NUM = 1;
struct CircleShadow {
	float3 dir;
	float3 casterPos;
	float distanceCasterLight;
	float3 atten;
	float2 factorAngleCos;
	uint active;
};

cbuffer cbuff2 : register(b2) {
	float3 ambientColor;
	DirLight dirLights[DIRLIGHT_NUM];
	PointLight pointLights[POINTLIGHT_NUM];
	SpotLight spotLights[SPOTLIGHT_NUM];
	CircleShadow circleShadows[CIRCLESHADOW_NUM];
};

struct VSInput
{
	float4 pos : POSITION;
	float3 normal : NORMAL;
	float2 uv : TEXCOORD;
	uint4 boneIndices : BONEINDICES;
	float4 boneWeights : BONEWEIGHTS;
};

struct VSOutput
{
	float4 svpos : SV_POSITION;
	float4 worldpos : POSITION; // ワールド座標
	float3 normal : NORMAL;
	float2 uv : TEXCOORD;
};

static const int MAX_BONES = 32;

cbuffer skinning : register(b3) {
	matrix matSkinning[MAX_BONES];
}