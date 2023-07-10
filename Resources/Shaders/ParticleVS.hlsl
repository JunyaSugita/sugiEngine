#include "Particle.hlsli"

VSOutput main(float4 pos : POSITION, float2 uv : TEXCOORD)
{
	VSOutput output;	//�s�N�Z���V�F�[�_�ɓn���l
	output.svpos = mul(mul(viewproj, world), pos);
	output.uv = uv;
	return output;
}