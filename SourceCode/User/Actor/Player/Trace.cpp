#include "Trace.h"
#include "ImageManager.h"

Trace::Trace(const ImageFoot& imagefoot_, const float& rot_, const XMFLOAT3& pos_) {
	Initialize(imagefoot_,rot_, pos_);
}

void Trace::Initialize(const ImageFoot& imagefoot_, const float& rot_,const XMFLOAT3& pos_) {
	
	trace_ = Object2d::Create(ImageManager::kLeftFoot + imagefoot_,
		{ pos_.x, 0.02f,pos_.z },
		{ 0.05f,0.05f,0.05f }, { 1,1,1,1 });
	trace_->SetRotation({ 90.0f,rot_,0 });
}

void Trace::Update() {
	life_--;

	alpha_ = (float)(life_) / kLifeMax;
	trace_->SetColor({1,1,1,alpha_});
	trace_->Update();
}

void Trace::Draw() {
	Object2d::PreDraw();
	trace_->Draw();
}
