#include "Particle.hlsli"

VSOutput main(float4 pos : POSITION)
{
	VSOutput output;	//�s�N�Z���V�F�[�_�ɓn���l
	output.pos = mul(mul(viewproj, world), pos);
	return output;
}