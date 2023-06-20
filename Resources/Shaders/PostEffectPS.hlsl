#include "PostEffect.hlsli"

Texture2D<float4> tex : register(t0);
SamplerState smp : register(s0);

float4 main(VSOutput input) : SV_TARGET
{
	float pixW = 1.0f / 1280.0f;
	float pixH = 1.0f / 720.0f;
	float4 temp = float4(0, 0, 0, 0);
	const int MAX_PIX = 5;
	int count = 0;
	
	for (int x = -MAX_PIX; x < MAX_PIX; x++) {
		for (int y = -MAX_PIX; y < MAX_PIX; y++) {
			temp += tex.Sample(smp, input.uv + float2 (-pixW * x, -pixH * y));
			temp += tex.Sample(smp, input.uv + float2 (0 * x, -pixH * y));
			temp += tex.Sample(smp, input.uv + float2 (pixW * x, -pixH * y));
			temp += tex.Sample(smp, input.uv + float2 (-pixW * x, 0 * y));
			temp += tex.Sample(smp, input.uv + float2 (0 * x, 0 * y));
			temp += tex.Sample(smp, input.uv + float2 (+pixW * x, 0 * y));
			temp += tex.Sample(smp, input.uv + float2 (-pixW * x, +pixH * y));
			temp += tex.Sample(smp, input.uv + float2 (0 * x, +pixH * y));
			temp += tex.Sample(smp, input.uv + float2 (+pixW * x, +pixH * y));

			count += 9;
		}
	}

	return float4(temp.rgb / count, 1);
}