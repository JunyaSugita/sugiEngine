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
    float4 col1 = float4(0, 0, 0, 1);
	//クロスフィルタ
	{
        float sigma = 0.02f;
        float stepWidth = 0.004f;
        float totalWeight1 = 0;
        float4 col2 = float4(0, 0, 0, 1);
        float totalWeight2 = 0;
        for (float i = -sigma; i < sigma; i += stepWidth)
        {
            float d = distance(input.uv, input.uv + float2(i, i));
            float weight = exp(-(d * d) / (2 * sigma * sigma));
            col1 += tex1.Sample(smp, input.uv + float2(i, i)) * weight;
            totalWeight1 += weight;
        }
        for (i = -sigma; i < sigma; i += stepWidth)
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
        float stepWidth = 0.004f;
        float4 col = float4(0, 0, 0, 1);
        for (float i = -sigma; i < sigma; i += stepWidth)
        {
            for (float j = -sigma; j < sigma; j += stepWidth)
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
    col1 += float4((tex3.Sample(smp, input.uv) * color));
    return col1;
}