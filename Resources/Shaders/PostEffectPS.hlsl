#include "PostEffect.hlsli"

Texture2D<float4> tex : register(t0);
SamplerState smp : register(s0);

float4 main(VSOutput input) : SV_TARGET
{
	float pixW = 1.0f / 1280.0f;
	float pixH = 1.0f / 720.0f;
	float4 temp = float4(0, 0, 0, 0);
	
	temp += tex.Sample(smp,input.uv + float2 (-pixW,-pixH));
	temp += tex.Sample(smp, input.uv + float2 (0, -pixH));
	temp += tex.Sample(smp, input.uv + float2 (pixW, -pixH));
	temp += tex.Sample(smp, input.uv + float2 (-pixW, 0));
	temp += tex.Sample(smp, input.uv + float2 (0, 0));
	temp += tex.Sample(smp, input.uv + float2 (+pixW, 0));
	temp += tex.Sample(smp, input.uv + float2 (-pixW, +pixH));
	temp += tex.Sample(smp, input.uv + float2 (0, +pixH));
	temp += tex.Sample(smp, input.uv + float2 (+pixW, +pixH));

	return float4(temp.rgb / 9,1);
}