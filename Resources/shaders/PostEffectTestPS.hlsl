#include"PostEffectTest.hlsli"

Texture2D<float4> tex0:register(t0);
Texture2D<float4> tex1:register(t1);

sampler smp:register(s0);

float4 main(VSOutput input) :SV_TARGET
{
	//float4 texcolor = tex.Sample(smp,input.uv);
	//���x
	//return float4(texcolor.rgb*2.0f,1);
	//���]
	//return float4(1 - texcolor.rgb, 1);float3(1,-1,1)
	//�I�t�Z�b�g���炵
	//float2 offset = (0.5,0.5);
	//float4 texcolor = tex.Sample(smp, input.uv+ offset);
	//return float4(texcolor.rgb,1);
	//���ςڂ���
	float4 colortex0 = tex0.Sample(smp,input.uv);
	float4 colortex1 = tex1.Sample(smp,input.uv);

	float4 color = colortex0;

	float2 movePixel = 1.0f / 0.2f;
	movePixel *= 0.001f;

	if (fmod(input.uv.y,0.1f)<0.05f) {
		// �ڂ���������������
		color = tex1.Sample(smp, input.uv);  // �������g�̃s�N�Z��
		// ������(�E�A��)�ׂ̗̃s�N�Z��/
		color += tex1.Sample(smp, input.uv + movePixel * 1.0f);
		color += tex1.Sample(smp, input.uv + movePixel * 2.0f);
		color += tex1.Sample(smp, input.uv + movePixel * 3.0f);
		color += tex1.Sample(smp, input.uv + movePixel * 4.0f);
		// ������(���A��)�ׂ̗̃s�N�Z��
		color += tex1.Sample(smp, input.uv - movePixel * 1.0f);
		color += tex1.Sample(smp, input.uv - movePixel * 2.0f);
		color += tex1.Sample(smp, input.uv - movePixel * 3.0f);
		color += tex1.Sample(smp, input.uv - movePixel * 4.0f);
		// �������킹���s�N�Z���̕��ς����߂�
		color /= 9.0f;
	}


	return float4(color.rgb,1);
}
