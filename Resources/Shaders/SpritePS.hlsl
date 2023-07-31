#include "Sprite.hlsli"

Texture2D<float4> tex : register(t0);
SamplerState smp : register(s0);

struct PSOutput
{
	float4 target0 : SV_TARGET0;
	float4 target1 : SV_TARGET1;
};

PSOutput main(VSOutput input)
{
	PSOutput output;
	output.target0 = float4(tex.Sample(smp, input.uv)) * color;
	//output.target1 = float4(1 - float3(tex.Sample(smp, input.uv).rgb * color.rgb), float4(tex.Sample(smp, input.uv) * color).a);

	return output;
}