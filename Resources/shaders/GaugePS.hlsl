#include"Gauge.hlsli"

Texture2D<float4> tex:register(t0);
sampler smp:register(s0);

float4 main(VSOutput input) :SV_TARGET
{
   const float PI = 3.14159f;
    //��������Ȃ񂩂���������������܂���
    float2 Center = {0.5f,0.5f};
    float2 Up = { 0.5f,1.0f };
    float2 Vector1 = {0.0f,-1.0f};
    float2 Vector2 = input.uv + Center;
    Vector1 = normalize(Vector1);
    Vector2 = normalize(Vector2);
    float Deg = acos(dot(Vector1, Vector2));
    Deg = abs(Deg);
    //���S����s�N�Z���̍��W�ւ̃x�N�g���ƒ��S���������ւ̃x�N�g���̓��ς�����
    //�t�R�T�C�����Ƃ�A�ݒ肵���p�x��菬������Ε\���A�傫����΃s�N�Z����j������
    if (Vector2.x < Vector1.x) {
        Deg = PI + (PI - Deg);
    }
    if (Deg >= angle) {
        //�s�N�Z����j������
        clip(-1);
    }
    float4 Color = tex.Sample(smp, input.uv) * color;
    return Color;
}