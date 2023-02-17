#include "FBX.hlsli"

Texture2D<float4> tex : register(t0);  // 0番スロットに設定されたテクスチャ
SamplerState smp : register(s0);      // 0番スロットに設定されたサンプラー

struct PSOutput {
	float4 target0 : SV_TARGET0;
	float4 target1 : SV_TARGET1;
};

float4 main(VSOutput input) : SV_TARGET
{
	// テクスチャマッピング
	float4 texcolor = tex.Sample(smp, input.uv);

	float3 P = input.svpos.xyz;
	float3 N = normalize(input.normal);
	float Ka = 0.8;
	float Kd = 0.7;

	//　Ambient Color
	float3 ambient = float3(1,1,1) * Ka;

	//　Diffuse Color
	float3 L = normalize(float3(1, 1, 1) - P);	//　ローカル座標系のでのライトベクトル
	float diffuseLight = max(dot(L, N), 0) * 0.5 + 0.5;
	float3 diffuse = Kd * float3(1,1,1) * (diffuseLight * diffuseLight);

	return float4(ambient + diffuse, 1.0f) * texcolor;

}