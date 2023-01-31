#include "Obj.hlsli"

Texture2D<float4> tex : register(t0);	//0�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
SamplerState smp : register(s0);		//0�ԃX���b�g�ɐݒ肳�ꂽ�T���v���[

float4 main(VSOutput input) : SV_TARGET
{
	//float3 light = normalize(float3(1,-1,1));				// �E���������̃��C�g
	//float light_diffuse = saturate(dot(-light, input.normal));	//diffuse��[0,1]�͈̔͂�Clamp����
	//float3 shade_color;
	//shade_color = m_ambient;	//�A���r�G���g��
	//shade_color += m_diffuse * light_diffuse;
	//float4 texcolor = tex.Sample(smp, input.uv) * m_color;
	//return float4(texcolor.rgb * shade_color, texcolor.a * m_alpha);	//�P�x��RGB�ɑ�����ďo��

	//�e�N�X�`���}�b�s���O
	float4 texcolor = tex.Sample(smp,input.uv);
	//�V�F�[�f�B���O
	return input.color * texcolor;

}