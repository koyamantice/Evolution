#include "FBX.hlsli"

Texture2D<float4> tex : register(t0);  // 0番スロットに設定されたテクスチャ
SamplerState smp : register(s0);      // 0番スロットに設定されたサンプラー

float4 main(VSOutput input) : SV_TARGET
{
	// テクスチャマッピング
	float4 texcolor = tex.Sample(smp, input.uv);
	const float _ThresholdMin = 0.4;
	const float _ThresholdMax = 0.8;
	float3 lightv = normalize(float3(0, 1, -1)); // 右下奥　向きのライト

	float3 eyeDir = normalize(cameraPos);
	float3 halfVec = normalize(lightv + eyeDir);
	float intensity = saturate(dot(normalize(input.normal), halfVec));
	float Diffuse = pow(intensity, 5);
	float Dif = smoothstep(_ThresholdMin, _ThresholdMax, Diffuse);

	float light = smoothstep(_ThresholdMin, _ThresholdMax, intensity);
	float dark = 1 - smoothstep(_ThresholdMin, _ThresholdMax, intensity);

	float4 lightColor = texcolor;
	float4 darkColor = float4(texcolor.xyz * 0.5, 1.0);


	float4 ambient = dark * darkColor;
	float4 diffuse = light * lightColor + dark * darkColor;
	float4 specular = Dif * float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 ads = ambient + diffuse + specular;

	return ads;
}