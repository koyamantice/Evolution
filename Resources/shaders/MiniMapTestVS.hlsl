#include"MiniMapTest.hlsli"

VSOutput main( float4 pos : POSITION,float2 uv:TEXCOORD )
{
	VSOutput output;

	//float width = 1280;
	//float hight= 720:
	//float x = 0;
	//float y = 0;
	//x=smothstep(0.0f,width,0.25f);
	//y=smothstep(0.0f,hight,0.25f);
	output.svpos = pos;
	output.uv = uv;
	return output;
}