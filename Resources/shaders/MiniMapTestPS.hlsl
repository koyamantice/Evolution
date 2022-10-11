#include"MiniMapTest.hlsli"

Texture2D<float4> tex:register(t0);
sampler smp:register(s0);

float4 main(VSOutput input) :SV_TARGET
{
	float4 texcolor = tex.Sample(smp,input.uv);
	//明度
	//return float4(texcolor.rgbf,1);
	//反転
	//return float4(1 - texcolor.rgb, 1);
	//普通
	return float4(texcolor.rgb, 1);
	//オフセットずらし
	//float2 offset = (0.5,0.5);
	//float4 texcolor = tex.Sample(smp, input.uv+ offset);
	//return float4(texcolor.rgb,1);
	//平均ぼかし
	//float4 color = float4(1.0f, 1.0f, 1.0f, 1.0f);

	//float2 movePixel = 1.0f / 0.2f;
	//movePixel *= 0.0001;

	// ぼかし処理をかける
	//color = tex.Sample(smp, input.uv);  // 自分自身のピクセル
	// 正方向(右、上)の隣のピクセル/
	//color += tex.Sample(smp, input.uv + movePixel * 1.0f);
	//color += tex.Sample(smp, input.uv + movePixel * 2.0f);
	//color += tex.Sample(smp, input.uv + movePixel * 3.0f);
	//color += tex.Sample(smp, input.uv + movePixel * 4.0f);
	// 負方向(左、下)の隣のピクセル
	//color += tex.Sample(smp, input.uv - movePixel * 1.0f);
	//color += tex.Sample(smp, input.uv - movePixel * 2.0f);
	//color += tex.Sample(smp, input.uv - movePixel * 3.0f);
	//color += tex.Sample(smp, input.uv - movePixel * 4.0f);
	// 足し合わせたピクセルの平均を求める
	//color /= 9.0f;

	//return float4(color.rgb,1);
}