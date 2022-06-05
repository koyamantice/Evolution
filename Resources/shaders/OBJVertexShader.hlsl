#include "OBJShaderHeader.hlsli"

Output main(float4 pos:POSITION, float3 normal : NORMAL, float2 uv : TEXCOORD)
{
	Output output;
	//output.svpos = pos;
	output.normal = normal;
	//座標に行列を乗算して出力
	output.svpos = mul(mat, pos);
	output.uv = uv;
	return output;
}