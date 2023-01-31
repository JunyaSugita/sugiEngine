#include "Obj.hlsli"

Texture2D<float4> tex : register(t0);  // 0番スロットに設定されたテクスチャ
SamplerState smp : register(s0);      // 0番スロットに設定されたサンプラー

float4 main(VSOutput input) : SV_TARGET
{
	//テクスチャマッピング
	float4 texcolor = tex.Sample(smp,input.uv);
	//光沢度
	const float shininess = 4.0f;
	//頂点から視点のベクトル
	float3 eyedir = normalize(cameraPos - input.worldpos.xyz);
	//環境反射光
	float3 ambient = m_ambient;
	//シェーディングによる色
	float4 shadecolor = float4(ambientColor * ambient, m_alpha);

	//平行光
	for (int i = 0; i < DIRLIGHT_NUM; i++) {
		if (dirLights[i].active) {
			float3 dotlightnormal = dot(dirLights[i].lightv, input.normal);
			float3 reflect = normalize(-dirLights[i].lightv + 2 * dotlightnormal * input.normal);
			float3 diffuse = dotlightnormal * m_diffuse;
			float3 specular = pow(saturate(dot(reflect, eyedir)), shininess) * m_specular;

			shadecolor.rgb += (diffuse + specular) * dirLights[i].lightcolor;
		}
	}
	//点光源
	for (int i = 0; i < POINTLIGHT_NUM; i++) {
		if (pointLights[i].active) {
			float3 lightv = pointLights[i].lightpos - input.worldpos.xyz;
			float d = length(lightv);
			lightv = normalize(lightv);
			float atten = 1.0f / (pointLights[i].lightatten.x + pointLights[i].lightatten.y * d + pointLights[i].lightatten.z * d * d);
			float3 dotlightnormal = dot(lightv, input.normal);
			float3 reflect = normalize(-lightv + 2 * dotlightnormal * input.normal);
			float3 diffuse = dotlightnormal * m_diffuse;
			float3 specular = pow(saturate(dot(reflect, eyedir)), shininess) * m_specular;
			shadecolor.rgb += atten * (diffuse + specular) * pointLights[i].lightcolor;
		}
	}
	//スポットライト
	for (i = 0; i < SPOTLIGHT_NUM; i++) {
		if (spotLights[i].active) {
			float3 lightv = spotLights[i].lightpos - input.worldpos.xyz;
			float d = length(lightv);
			lightv = normalize(lightv);

			float atten = saturate(1.0f / (spotLights[i].lightatten.x + spotLights[i].lightatten.y * d + spotLights[i].lightatten.z * d * d));

			float cos = dot(lightv, spotLights[i].lightv);

			float angleatten = smoothstep(spotLights[i].lightfactoranglecos.y, spotLights[i].lightfactoranglecos.x, cos);

			atten *= angleatten;

			float3 dotlightnormal = dot(lightv, input.normal);

			float3 reflect = normalize(-lightv + 2 * dotlightnormal * input.normal);
			float3 diffuse = dotlightnormal * m_diffuse;
			float3 specular = pow(saturate(dot(reflect, eyedir)), shininess) * m_specular;

			shadecolor.rgb += atten * (diffuse + specular) * spotLights[i].lightcolor;

		}
	}
	//丸影
	for (i = 0; i < CIRCLESHADOW_NUM; i++) {
		if (circleShadows[i].active) {
			float3 casterv = circleShadows[i].casterPos - input.worldpos.xyz;
			float d = dot(casterv, circleShadows[i].dir);
			float atten = saturate(1.0f / (circleShadows[i].atten.x + circleShadows[i].atten.y * d + circleShadows[i].atten.z * d * d));
			atten *= step(0, d);
			float3 lightpos = circleShadows[i].casterPos + circleShadows[i].dir * circleShadows[i].distanceCasterLight;
			float3 lightv = normalize(lightpos - input.worldpos.xyz);
			float cos = dot(lightv, circleShadows[i].dir);
			float angleatten = smoothstep(circleShadows[i].factorAngleCos.y, circleShadows[i].factorAngleCos.x, cos);
			atten *= angleatten;

			shadecolor.rgb -= atten;
		}
	}


	//シェーディングによる色で描画
	return shadecolor * texcolor;
}