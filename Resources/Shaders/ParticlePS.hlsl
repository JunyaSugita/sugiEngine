#include "Particle.hlsli"

Texture2D<float4> tex : register(t0);
SamplerState smp : register(s0);

struct PSOutput
{
	float4 target0 : SV_TARGET0;
	float4 target1 : SV_TARGET1;
    float4 target2 : SV_TARGET2;
    float4 target3 : SV_TARGET3;
};

PSOutput main(GSOutput input)
{
	PSOutput output;
    output.target0 = float4(tex.Sample(smp, input.uv)) * input.color;

	return output;
}