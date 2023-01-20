#include "Basic.hlsli"

Texture2D<float4> tex : register(t0);  // 0�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
SamplerState smp : register(s0);      // 0�ԃX���b�g�ɐݒ肳�ꂽ�T���v���[

struct PSOutput {
	float4 target0 : SV_TARGET0;
	float4 target1 : SV_TARGET1;
};

PSOutput main(VSOutput input) {
	PSOutput output;
	// �e�N�X�`���}�b�s���O
	float4 texcolor = tex.Sample(smp, input.uv);
	// Lambert����
	float3 light = normalize(float3(1, -1, 1)); // �E�����@�����̃��C�g
	float diffuse = saturate(dot(-light, input.normal));
	float brightness = diffuse + 0.3f;
	float4 shadecolor = float4(brightness, brightness, brightness, 1.0f);
	// �A�e�ƃe�N�X�`���̐F������
	output.target0 = shadecolor * texcolor;
	output.target1 = float4(1 - (shadecolor * texcolor).rgb, 1);


	return output;
}

//�g�D�[���V�F�[�_�[
//float4 main(VSOutput input) : SV_TARGET
//{
//	// �e�N�X�`���}�b�s���O
//	float4 texcolor = tex.Sample(smp, input.uv);
//	const float _ThresholdMin = 0.2;
//	const float _ThresholdMax = 0.8;
//	float3 lightv = normalize(float3(0, 1, -1)); // �E�����@�����̃��C�g
//
//	float3 eyeDir = normalize(cameraPos - input.worldpos.xyz);
//	float3 halfVec = normalize(lightv + eyeDir);
//	float intensity = saturate(dot(normalize(input.normal), halfVec));
//	float Diffuse = pow(intensity, 10);
//	float Dif = smoothstep(_ThresholdMin, _ThresholdMax, Diffuse);
//
//	float light = smoothstep(_ThresholdMin, _ThresholdMax, intensity);
//	float dark = 1 - smoothstep(_ThresholdMin, _ThresholdMax, intensity);
//
//	float4 lightColor = texcolor;
//	float4 darkColor = float4(texcolor.xyz * 0.5, 1.0);
//
//
//	float4 ambient = dark * darkColor * float4(m_ambient.xyz,1.0);
//	float4 diffuse = light * lightColor + dark * darkColor;
//	float4 specular = Dif * float4(0.0f, 0.0f, 0.0f, 1.0f);
//	float4 ads = ambient + diffuse + specular;
//
//	return ads;
//}