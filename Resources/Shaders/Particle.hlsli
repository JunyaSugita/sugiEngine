//�}�e���A��
cbuffer ConstBufferDataMaterial : register(b0) {
	float4 color;
}

//3D�ϊ��s��
cbuffer ConstBufferDataTransform : register(b1) {
	matrix mat;
}

//���_�V�F�[�_�[�̏o�͍\����
struct VSOutput {
	//�V�X�e���p���_���W
	float4 svpos:SV_POSITION;
	//uv�l
	float2 uv :TEXCOORD;
};