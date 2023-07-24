cbuffer cbuff0 : register(b0)
{
	matrix viewproj;
	matrix world;
	float3 cameraPos;
    bool isEffectCross;
};

cbuffer cbuff1 : register(b1)
{
	float3 m_ambient  : packoffset(c0); // �A���r�G���g�W��
	float3 m_diffuse  : packoffset(c1); // �f�B�t���[�Y�W��
	float3 m_specular : packoffset(c2); // �X�y�L�����[�W��
	float m_alpha : packoffset(c2.w);	// �A���t�@
    float4 m_color : packoffset(c3);
};

//���s��
static const int DIRLIGHT_NUM = 3;
struct DirLight {
	float3 lightv;
	float3 lightcolor;
	uint active;
};

//�_����
static const int POINTLIGHT_NUM = 3;
struct PointLight {
	float3 lightpos;
	float3 lightcolor;
	float3 lightatten;
	uint active;
};

//�X�|�b�g���C�g
static const int SPOTLIGHT_NUM = 3;
struct SpotLight {
	float3 lightv;
	float3 lightpos;
	float3 lightcolor;
	float3 lightatten;
	float2 lightfactoranglecos;
	uint active;
};

//�ۉe
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


// ���_�V�F�[�_�[����s�N�Z���V�F�[�_�[�ւ̂����Ɏg�p����\����
struct VSOutput
{
	float4 svpos : SV_POSITION;	// �V�X�e���p���_���W
	float4 worldpos : POSITION; // ���[���h���W
	float3 normal :NORMAL; // �@���x�N�g��
	//float4 color : COLOR;

	float2 uv  :TEXCOORD; // uv�l
};
