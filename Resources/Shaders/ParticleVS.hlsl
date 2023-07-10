#include "Particle.hlsli"

VSOutput main(float4 pos : POSITION)
{
	VSOutput output;	//ピクセルシェーダに渡す値
	output.pos = mul(mul(viewproj, world), pos);
	return output;
}