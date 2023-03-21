#include "Trace.h"
#include "ImageManager.h"

Trace::Trace(const float& rot_, const XMFLOAT3& pos_) {
	Initialize(rot_, pos_);
}

void Trace::Initialize(const float& rot_,const XMFLOAT3& pos_) {
	Object2d* Trace_ = Object2d::Create(ImageManager::kFoot, 
		{ pos_.x, 0.02f,pos_.z },
		{ 0.1f,0.1f,0.1f }, { 1,1,1,1 });
	Trace_->SetRotation({ 90.0f,rot_,0 });
	trace_.reset(Trace_);
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
