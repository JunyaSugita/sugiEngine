#include "PostEffect.hlsli"

Texture2D<float4> tex0 : register(t0);
Texture2D<float4> tex1 : register(t1);
SamplerState smp : register(s0);

float4 main(VSOutput input) : SV_TARGET
{
	//float pixW = 1.0f / 1280.0f;
	//float pixH = 1.0f / 720.0f;
	//float4 temp = float4(0, 0, 0, 0);
	//const int MAX_PIX = 5;
	//int count = 0;
	//
	//for (int x = -MAX_PIX; x < MAX_PIX; x++) {
	//	for (int y = -MAX_PIX; y < MAX_PIX; y++) {
	//		temp += tex0.Sample(smp, input.uv + float2 (-pixW * x, -pixH * y));
	//		temp += tex0.Sample(smp, input.uv + float2 (0 * x, -pixH * y));
	//		temp += tex0.Sample(smp, input.uv + float2 (pixW * x, -pixH * y));
	//		temp += tex0.Sample(smp, input.uv + float2 (-pixW * x, 0 * y));
	//		temp += tex0.Sample(smp, input.uv + float2 (0 * x, 0 * y));
	//		temp += tex0.Sample(smp, input.uv + float2 (+pixW * x, 0 * y));
	//		temp += tex0.Sample(smp, input.uv + float2 (-pixW * x, +pixH * y));
	//		temp += tex0.Sample(smp, input.uv + float2 (0 * x, +pixH * y));
	//		temp += tex0.Sample(smp, input.uv + float2 (+pixW * x, +pixH * y));

	//		count += 9;
	//	}
	//}

	//temp = temp / count;


	//float4 colortex1 = tex1.Sample(smp, input.uv);

	//float4 color = temp;
	//if (fmod(input.uv.y, 0.1f) < 0.05f) {
	//	color = colortex1;
	//}

	//return float4(color.rgb, 1);

	if (blur) {
		float totalWeight = 0;
		float sigma = 0.005f;
		float stepWidth = 0.001f;
		float4 col = float4(0, 0, 0, 0);
		for (float i = -(sigma * 2); i < sigma * 2; i += stepWidth) {
			for (float j = -(sigma * 2); j < sigma * 2; j += stepWidth) {
				float d = distance(input.uv, input.uv + float2(j,i));
				float weight = exp(-(d * d) / (2 * sigma * sigma));
				col += tex0.Sample(smp, input.uv + float2 (j, i)) * weight;
				totalWeight += weight;
			}
		}
		col = col / totalWeight;

		return float4(col.rgb, 1);
	}
	
	return float4(1, 0, 0, 1);
}