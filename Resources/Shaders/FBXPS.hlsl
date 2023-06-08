#include "FBX.hlsli"

Texture2D<float4> tex : register(t0);

SamplerState smp : register(s0);

float4 main(VSOutput input) : SV_TARGET
{
	float4 texcolor = tex.Sample(smp,input.uv);

	float3 light = normalize(float3(1, -1, 1));
	float diffuse = saturate(dot(-light, input.normal));
	float brightness = diffuse + 0.3f;
	float4 shadecolor = float4(brightness, brightness, brightness, 1.0f);

	return shadecolor * texcolor;
}