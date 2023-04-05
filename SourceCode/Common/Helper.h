#pragma once
#include <DirectXMath.h>
#include <algorithm>

using namespace DirectX;




XMFLOAT3 Bound(const XMFLOAT3& pos_, const XMFLOAT3& pos2_,const float& power_);

float GetParabolicHeight(float pos_, float vel,float hightest,float timest);
