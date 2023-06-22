#include "PostEffect.hlsli"

Texture2D<float4> tex0 : register(t0);
Texture2D<float4> tex1 : register(t1);
SamplerState smp : register(s0);

float4 main(VSOutput input) : SV_TARGET
{
	if (blur) {
		float totalWeight = 0;
		float sigma = 0.005f;
		float stepWidth = 0.001f;
		float4 col = float4(0, 0, 0, 0);
		for (float i = -(sigma * 2); i < sigma * 2; i += stepWidth) {
			for (float j = -(sigma * 2); j < sigma * 2; j += stepWidth) {
				float d = distance(input.uv, input.uv + float2(j,i));
				float weight = exp(-(d * d) / (2 * sigma * sigma));
				col += tex1.Sample(smp, input.uv + float2 (j, i)) * weight;
				totalWeight += weight;
			}
		}
		col = col / totalWeight;

		return float4(col.rgb, 1);
	}
	else if (border) {
		float4 colortex1 = tex1.Sample(smp, input.uv);
		float4 color = tex0.Sample(smp, input.uv);
		if (fmod(input.uv.y, 0.1f) < 0.05f) {
			color = colortex1;
		}
		return float4(color.rgb, 1);
	}
	else if (gray) {
		float4 col = float4(float3((tex1.Sample(smp, input.uv) * color).rgb), 1);
		float grayScale = col.r * 0.299f + col.g * 0.587f + col.b * 0.114f;
		float extract = smoothstep(0.85f, 0.9f, grayScale);
		return float4(extract, extract, extract, 1);
	}
	else if (bloom) {
		float4 col = float4(float3((tex1.Sample(smp, input.uv) * color).rgb), 1);
		float grayScale = col.r * 0.299f + col.g * 0.587f + col.b * 0.114f;
		float extract = smoothstep(0.85f, 0.9f, grayScale);
		float4 temp = float4(extract, extract, extract, 1);

		float totalWeight = 0;
		float sigma = 0.005f;
		float stepWidth = 0.001f;
		float4 col1 = float4(0, 0, 0, 0);
		for (float i = -(sigma * 2); i < sigma * 2; i += stepWidth) {
			for (float j = -(sigma * 2); j < sigma * 2; j += stepWidth) {
				float d = distance(input.uv, input.uv + float2(j, i));
				float weight = exp(-(d * d) / (2 * sigma * sigma));
				col1 += temp * weight;
				totalWeight += weight;
			}
		}
		col1 = col1 / totalWeight;

		return float4(col1.rgb, 1);
	}

	return float4(float3((tex1.Sample(smp, input.uv) * color).rgb),1);
}