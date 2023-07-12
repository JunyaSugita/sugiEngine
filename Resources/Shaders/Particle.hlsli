//3D�ϊ��s��
cbuffer ConstBuffB1 : register(b1)
{
    matrix mat;
    matrix billboard;
}

//���_�V�F�[�_�[�̏o�͍\����
struct VSOutput
{
	//�V�X�e���p���_���W
    float4 pos : POSITION;
    float scale : TEXCOORD;
    float4 color : COLOR;
};

//�W�I���g���V�F�[�_�[�̏o�͍\����
struct GSOutput
{
	//�V�X�e���p���_���W
    float4 svpos : SV_POSITION;
	//uv�l
    float2 uv : TEXCOORD;
    
    float4 color : COLOR;
};