#include"Bullet.h"
#include <SourceCode/User/ModelManager.h>



Bullet::Bullet() {
}

void Bullet::Initialize(FBXModel* model, const std::string& tag, ActorComponent* compornent) {
	FBXObject3d* fbxObj_ = new FBXObject3d();
	fbxObj_->Initialize();
	fbxObj_->SetModel(model);
	fbxObj_->SetScale({ 0.002f,0.002f, 0.002f });
	fbxObj.reset(fbxObj_);
	fbxObj->LoadAnimation();
	fbxObj->PlayAnimation();

	OnInit();
}

void Bullet::Update() {
	if (isActive) {
		fbxObj->Update();
		oldPos = fbxObj->GetPosition();
		OnUpda();
	}

}

void Bullet::Demo() {
}

void Bullet::Draw(DirectXCommon* dxCommon) {
	if (isActive) {
		Object3d::PreDraw();
		fbxObj->Draw(dxCommon->GetCmdList());
		OnDraw(dxCommon);
	}
}

void Bullet::DemoDraw(DirectXCommon* dxCommon) {
	if (isActive) {
		DebugUpdate();
		Object3d::PreDraw();
		fbxObj->Draw(dxCommon->GetCmdList());
		OnDraw(dxCommon);
	}
}

void Bullet::Finalize() {
}



void Bullet::SetCommand(const int& command, XMFLOAT3 pos) {
	this->command = command; AftaerPos = pos;
}