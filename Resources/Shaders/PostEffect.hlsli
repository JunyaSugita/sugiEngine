//�}�e���A��
cbuffer ConstBufferDataMaterial : register(b0) {
	float4 color;
}

//3D�ϊ��s��
cbuffer ConstBufferDataTransform : register(b1) {
	matrix mat;
}

//3D�ϊ��s��
cbuffer ConstBufferDataEffect : register(b2) {
	int blur;
	int invertColor;
	int border;
	int gray;
	int bloom;
}

//���_�V�F�[�_�[�̏o�͍\����
struct VSOutput {
	//�V�X�e���p���_���W
	float4 svpos:SV_POSITION;
	//uv�l
	float2 uv :TEXCOORD;
};