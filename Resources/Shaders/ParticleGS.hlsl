#include "Particle.hlsli"

static const uint vnum = 4;

static const float4 offset_array[vnum] =
{
    float4(-1.0f,-1.0f, 0, 0),
    float4(-1.0f, 1.0f, 0, 0),
    float4( 1.0f,-1.0f, 0, 0),
    float4( 1.0f, 1.0f, 0, 0)
};

static const float2 uv_array[vnum] =
{
    float2(0, 1),
    float2(0, 0),
    float2(1, 1),
    float2(1, 0)
};

[maxvertexcount(vnum)]
void main(
	point VSOutput input[1] : SV_POSITION,
	inout TriangleStream<GSOutput> output
)
{
    GSOutput element;
    
    for (uint i = 0; i < vnum; i++)
    {
        float4 offset = mul(billboard, offset_array[i] * input[0].scale);
        
        element.svpos = input[0].pos + offset;
    
        element.svpos = mul(mat, element.svpos);
        element.uv = uv_array[i];
        element.color = input[0].color;
        
        output.Append(element);
    }
}