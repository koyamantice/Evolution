#include "Helper.h"


XMFLOAT3 Bound(const XMFLOAT3& pos_, const XMFLOAT3& pos2_, const float power_) {
    XMFLOAT3 distance = { pos_.x - pos2_.x,0,pos_.z - pos2_.z };

    XMFLOAT3 itr = {
    pos_.x - sinf(atan2f(distance.x, distance.z)) * power_,
    0,
    pos_.z - cosf(atan2f(distance.x, distance.z)) * power_
    };

    return itr;
}

