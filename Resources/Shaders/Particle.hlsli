//�}�e���A��
cbuffer ConstBufferDataMaterial : register(b0) {
	float4 color;
}

//3D�ϊ��s��
cbuffer ConstBuffB1 : register(b1) {
	matrix viewproj;
	matrix world;
	float3 cameraPos;
}

//���_�V�F�[�_�[�̏o�͍\����
struct VSOutput {
	//�V�X�e���p���_���W
	float4 svpos:SV_POSITION;
	//uv�l
	float2 uv :TEXCOORD;
};