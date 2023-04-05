#pragma once
#include <DirectXMath.h>
#include <algorithm>
#include <vector>
using namespace DirectX;
template<typename T>

T clamp(T num_, T max_, T min_) {
    auto itr = num_;
    itr = min(max(num_, min_), max_);
    return itr;
}
template <typename T>
T GetParabolicHeight(T pos_, T vel, T hightest, T timest) {
    auto itr_ = pos_;
    itr_ += vel; //+
    vel -= hightest / (timest / 2.0f);//
    return itr_;
}
XMFLOAT3 Bound(const XMFLOAT3& pos_, const XMFLOAT3& pos2_,const float& power_);

