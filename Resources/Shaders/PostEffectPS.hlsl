#include "PostEffect.hlsli"

Texture2D<float4> tex0 : register(t0);
Texture2D<float4> tex1 : register(t1);
Texture2D<float4> tex2 : register(t2);
Texture2D<float4> tex3 : register(t3);
Texture2D<float4> tex4 : register(t4);
Texture2D<float4> tex5 : register(t5);
Texture2D<float4> tex6 : register(t6);
Texture2D<float4> tex7 : register(t7);
SamplerState smp : register(s0);

float4 main(VSOutput input) : SV_TARGET
{
	//if (blur) {
	//	float totalWeight = 0;
	//	float sigma = 0.001f * Isigma;
	//	float stepWidth = 0.001f;
	//	float4 col = float4(0, 0, 0, 0);
	//	for (float i = -(sigma * 2); i < sigma * 2; i += stepWidth) {
	//		for (float j = -(sigma * 2); j < sigma * 2; j += stepWidth) {
	//			float d = distance(input.uv, input.uv + float2(j,i));
	//			float weight = exp(-(d * d) / (2 * sigma * sigma));
	//			col += tex0.Sample(smp, input.uv + float2 (j, i)) * weight;
	//			totalWeight += weight;
	//		}
	//	}
	//	col = col / totalWeight;

	//	return float4(col.rgb, 1);
	//}
	//else if (invertColor) {
	//	return float4(float3((tex1.Sample(smp, input.uv) * color).rgb), 1);
	//}
	//else if (border) {
	//	float4 colortex1 = tex0.Sample(smp, input.uv);
	//	float4 color = tex1.Sample(smp, input.uv);
	//	if (fmod(input.uv.y, 0.1f) < 0.05f) {
	//		color = colortex1;
	//	}
	//	return float4(color.rgb, 1);
	//}
	//else if (gray) {
	//	float4 col = float4(float3((tex0.Sample(smp, input.uv) * color).rgb), 1);
	//	float grayScale = col.r * 0.299f + col.g * 0.587f + col.b * 0.114f;
	//	return float4(grayScale, grayScale, grayScale, 1);
	//}
	//else if (bloom) {
	//	float totalWeight = 0;
	//	float sigma = 0.005f;
	//	float stepWidth = 0.001f;
	//	float4 col = float4(0, 0, 0, 1);
	//	for (float i = -(sigma * 2); i < sigma * 2; i += stepWidth) {
	//		for (float j = -(sigma * 2); j < sigma * 2; j += stepWidth) {
	//			float d = distance(input.uv, input.uv + float2(j, i));
	//			float weight = exp(-(d * d) / (2 * sigma * sigma));
	//			col += tex2.Sample(smp, input.uv + float2 (j, i)) * weight;
	//			totalWeight += weight;
	//		}
	//	}
	//	col = col / totalWeight;
	//	col.w = 1;
	//	col.rgb = col.rgb * 5;//ブルーム調整

	//	col.rgb += float3((tex0.Sample(smp, input.uv) * color).rgb);
	//	return col;
	//}
	//else if (closs4) {
	//	float sigma = 0.06f;
	//	float stepWidth = 0.005f;
	//	float4 col1 = float4(0, 0, 0, 1);
	//	float totalWeight1 = 0;
	//	float4 col2 = float4(0, 0, 0, 1);
	//	float totalWeight2 = 0;
	//	for (float i = -(sigma * 2); i < sigma * 2; i += stepWidth) {
	//		float d = distance(input.uv, input.uv + float2(i, i));
	//		float weight = exp(-(d * d) / (2 * sigma * sigma));
	//		col1 += tex2.Sample(smp, input.uv + float2 (i, i)) * weight;
	//		totalWeight1 += weight;
	//	}
	//	for (float i = -(sigma * 2); i < sigma * 2; i += stepWidth) {
	//		float d = distance(input.uv, input.uv + float2(i, -i));
	//		float weight = exp(-(d * d) / (2 * sigma * sigma));
	//		col2 += tex2.Sample(smp, input.uv + float2 (i,-i)) * weight;
	//		totalWeight2 += weight;
	//	}
	//	col1 = col1 / totalWeight1;
	//	col1.rgb = col1.rgb * 5;//ブルーム調整
	//	col2 = col2 / totalWeight2;
	//	col2.rgb = col2.rgb * 5;//ブルーム調整
	//	col1 += col2;

	//	col1.rgb += float3((tex0.Sample(smp, input.uv) * color).rgb);
	//	return col1;
	//}
	//else if (closs6) {
	//	float sigma = 0.06f;
	//	float stepWidth = 0.005f;
	//	float4 col1 = float4(0, 0, 0, 1);
	//	float totalWeight1 = 0;
	//	float4 col2 = float4(0, 0, 0, 1);
	//	float totalWeight2 = 0;
	//	float4 col3 = float4(0, 0, 0, 1);
	//	float totalWeight3 = 0;
	//	for (float i = -(sigma * 2); i < sigma * 2; i += stepWidth) {
	//		float d = distance(input.uv, input.uv + float2(i, i));
	//		float weight = exp(-(d * d) / (2 * sigma * sigma));
	//		col1 += tex2.Sample(smp, input.uv + float2 (i, i)) * weight;
	//		totalWeight1 += weight;
	//	}
	//	for (float i = -(sigma * 2); i < sigma * 2; i += stepWidth) {
	//		float d = distance(input.uv, input.uv + float2(i, -i));
	//		float weight = exp(-(d * d) / (2 * sigma * sigma));
	//		col2 += tex2.Sample(smp, input.uv + float2 (i, -i)) * weight;
	//		totalWeight2 += weight;
	//	}
	//	for (float i = -(sigma * 2); i < sigma * 2; i += stepWidth) {
	//		float d = distance(input.uv, input.uv + float2(i, 0));
	//		float weight = exp(-(d * d) / (2 * sigma * sigma));
	//		col3 += tex2.Sample(smp, input.uv + float2 (i, 0)) * weight;
	//		totalWeight3 += weight;
	//	}
	//	col1 = col1 / totalWeight1;
	//	col1.rgb = col1.rgb * 5;//ブルーム調整
	//	col2 = col2 / totalWeight2;
	//	col2.rgb = col2.rgb * 5;//ブルーム調整
	//	col3 = col3 / totalWeight3;
	//	col3.rgb = col3.rgb * 5;//ブルーム調整
	//	col1 += col2 + col3;

	//	col1.rgb += float3((tex0.Sample(smp, input.uv) * color).rgb);
	//	return col1;
	//}
	//else if (closs8) {
	//	float sigma = 0.06f;
	//	float stepWidth = 0.005f;
	//	float4 col1 = float4(0, 0, 0, 1);
	//	float totalWeight1 = 0;
	//	float4 col2 = float4(0, 0, 0, 1);
	//	float totalWeight2 = 0;
	//	float4 col3 = float4(0, 0, 0, 1);
	//	float totalWeight3 = 0;
	//	float4 col4 = float4(0, 0, 0, 1);
	//	float totalWeight4 = 0;
	//	for (float i = -(sigma * 2); i < sigma * 2; i += stepWidth) {
	//		float d = distance(input.uv, input.uv + float2(i, i));
	//		float weight = exp(-(d * d) / (2 * sigma * sigma));
	//		col1 += tex2.Sample(smp, input.uv + float2 (i, i)) * weight;
	//		totalWeight1 += weight;
	//	}
	//	for (float i = -(sigma * 2); i < sigma * 2; i += stepWidth) {
	//		float d = distance(input.uv, input.uv + float2(i, -i));
	//		float weight = exp(-(d * d) / (2 * sigma * sigma));
	//		col2 += tex2.Sample(smp, input.uv + float2 (i, -i)) * weight;
	//		totalWeight2 += weight;
	//	}
	//	for (float i = -(sigma * 2); i < sigma * 2; i += stepWidth) {
	//		float d = distance(input.uv, input.uv + float2(i, 0));
	//		float weight = exp(-(d * d) / (2 * sigma * sigma));
	//		col3 += tex2.Sample(smp, input.uv + float2 (i, 0)) * weight;
	//		totalWeight3 += weight;
	//	}
	//	for (float i = -(sigma * 2); i < sigma * 2; i += stepWidth) {
	//		float d = distance(input.uv, input.uv + float2(0, i));
	//		float weight = exp(-(d * d) / (2 * sigma * sigma));
	//		col4 += tex2.Sample(smp, input.uv + float2 (0, i)) * weight;
	//		totalWeight4 += weight;
	//	}
	//	col1 = col1 / totalWeight1;
	//	col1.rgb = col1.rgb * 5;//ブルーム調整
	//	col2 = col2 / totalWeight2;
	//	col2.rgb = col2.rgb * 5;//ブルーム調整
	//	col3 = col3 / totalWeight3;
	//	col3.rgb = col3.rgb * 5;//ブルーム調整
	//	col4 = col4 / totalWeight4;
	//	col4.rgb = col4.rgb * 5;//ブルーム調整
	//	col1 += col2 + col3 + col4;

	//	col1.rgb += float3((tex0.Sample(smp, input.uv) * color).rgb);
	//	return col1;
	//}

    float4 col1 = float4(0, 0, 0, 1);
	//クロスフィルタ
	{
        float sigma = 0.01f;
        float stepWidth = 0.002f;
        float totalWeight1 = 0;
        float4 col2 = float4(0, 0, 0, 1);
        float totalWeight2 = 0;
        for (float i = -(sigma * 2); i < sigma * 2; i += stepWidth)
        {
            float d = distance(input.uv, input.uv + float2(i, i));
            float weight = exp(-(d * d) / (2 * sigma * sigma));
            col1 += tex1.Sample(smp, input.uv + float2(i, i)) * weight;
            totalWeight1 += weight;
        }
        for (i = -(sigma * 2); i < sigma * 2; i += stepWidth)
        {
            float d = distance(input.uv, input.uv + float2(i, -i));
            float weight = exp(-(d * d) / (2 * sigma * sigma));
            col2 += tex1.Sample(smp, input.uv + float2(i, -i)) * weight;
            totalWeight2 += weight;
        }
        col1 += col2;
    }
	//ブルーム
	{
        float totalWeight = 0;
        float sigma = 0.02f;
        float stepWidth = 0.008f;
        float4 col = float4(0, 0, 0, 1);
        for (float i = -(sigma * 2); i < sigma * 2; i += stepWidth)
        {
            for (float j = -(sigma * 2); j < sigma * 2; j += stepWidth)
            {
                float d = distance(input.uv, input.uv + float2(j, i));
                float weight = exp(-(d * d) / (2 * sigma * sigma));
                col += tex2.Sample(smp, input.uv + float2(j, i)) * weight;
                totalWeight += weight;
            }
        }
        col = col / totalWeight;
        col.w = 1;
        col.rgb = col.rgb * 5; //ブルーム調整
		
        col1 += col;
    }

    col1 += float4((tex0.Sample(smp, input.uv) * color));
    return col1;
}