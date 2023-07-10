#include "Particle.hlsli"

VSOutput main(float4 pos : POSITION, float2 uv : TEXCOORD)
{
	VSOutput output;	//ピクセルシェーダに渡す値
	output.svpos = mul(mul(viewproj, world), pos);
	output.uv = uv;
	return output;
}