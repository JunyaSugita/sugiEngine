#include "FBX.hlsli"

Texture2D<float4> tex : register(t0);

SamplerState smp : register(s0);

struct PSOutput
{
	float4 target0 : SV_TARGET0;
	float4 target1 : SV_TARGET1;
	float4 target2 : SV_TARGET2;
	float4 target3 : SV_TARGET3;
	float4 target4 : SV_TARGET4;
	float4 target5 : SV_TARGET5;
	float4 target6 : SV_TARGET6;
	float4 target7 : SV_TARGET7;
};

PSOutput main(VSOutput input)
{
	PSOutput output;
	float4 texcolor = tex.Sample(smp,input.uv);

	float3 light = normalize(float3(1, -1, 1));
	float diffuse = saturate(dot(-light, input.normal));
	float brightness = diffuse + 0.3f;
	float4 shadecolor = float4(brightness, brightness, brightness, 1.0f);

	output.target0 = float4(1 - (shadecolor * texcolor).rgb, 1);
	output.target1 = shadecolor * texcolor;

	float4 col = output.target1;
	float grayScale = col.r * 0.299f + col.g * 0.587f + col.b * 0.114f;
	float extract = smoothstep(0.6f, 0.9f, grayScale);
	output.target2 = float4(col.rgb * extract, 1);

	return output;
}