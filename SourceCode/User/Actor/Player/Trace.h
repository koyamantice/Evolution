#pragma once
#include"Trace.h"
#include"Object2d.h"
class Trace{
private:
	using XMFLOAT3 = DirectX::XMFLOAT3;
public:
	enum ImageFoot{
		LeftFoot=0,
		RightFoot
	};
	Trace(const ImageFoot& imagefoot_=LeftFoot,const float& rot_ = 0.0f, const XMFLOAT3& pos_ = {});
	~Trace() {};
	void Initialize(const ImageFoot& imagefoot_, const float& rot_, const XMFLOAT3& pos_);
	void Update();
	void Draw();
	const int& GetLife() { return life_; }
private:

	std::unique_ptr<Object2d> trace_;
	int life_ = 180;
	float alpha_ = 1.0f;
	const float kLifeMax = 180.0f;
};
