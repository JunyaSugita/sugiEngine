#include "Obj.hlsli"

Texture2D<float4> tex : register(t0);  // 0�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
SamplerState smp : register(s0);      // 0�ԃX���b�g�ɐݒ肳�ꂽ�T���v���[

float4 main(VSOutput input) : SV_TARGET
{
	//�e�N�X�`���}�b�s���O
	float4 texcolor = tex.Sample(smp,input.uv);
	//����x
	const float shininess = 20.0f;
	//���_���王�_�̃x�N�g��
	float3 eyedir = normalize(cameraPos - input.worldpos.xyz);
	//�����ˌ�
	float3 ambient = m_ambient;
	//�V�F�[�f�B���O�ɂ��F
	float4 shadecolor = float4(ambientColor + ambient, m_alpha);

	//���s��
	for (int i = 0; i < DIRLIGHT_NUM; i++) {
		if (dirLights[i].active) {
			float3 iNormal = normalize(input.normal);
			float3 lightNormal = normalize(dirLights[i].lightv);
			float3 dotlightnormal = dot(iNormal, lightNormal);
			float3 reflect = -lightNormal + 2 * iNormal * dotlightnormal;
			float3 diffuse = smoothstep(0.01, 0.05, dotlightnormal) * m_diffuse * dirLights[i].lightcolor;
			float3 specular = smoothstep(0.01, 0.05, pow(saturate(dot(reflect, eyedir)), shininess) * m_specular * dirLights[i].lightcolor);

			float rim = 1 - smoothstep(0.5, 0.6, dot(eyedir, iNormal));
			float4 rimColor = { 0,0,1,1 };


			shadecolor.rgb += (rimColor.rgb * rim) + diffuse + specular;
		}
	}
	//�_����
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
	//�X�|�b�g���C�g
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
	//�ۉe
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

	//�V�F�[�f�B���O�ɂ��F�ŕ`��
	return shadecolor * texcolor;
}