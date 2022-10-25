#include"Bullet.h"
#include <SourceCode/User/ModelManager.h>
#include <SourceCode/FrameWork/ActorManager.h>



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
		Move();
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

void Bullet::BoidAverage() {
	XMFLOAT3 pos = fbxObj->GetPosition();
	player = ActorManager::GetInstance()->SearchActor("Player");
	flocking.average = player->GetPosition();
	flocking.ctrDirX = flocking.average.x - pos.x;
	flocking.ctrDirY = flocking.average.z - pos.z;
}

void Bullet::Move() {
	XMFLOAT3 pos = fbxObj->GetPosition();
	BoidAverage();
	float kX = 0.7f * flocking.ctrDirX + 0.2f * flocking.vel.x + 0.8f * flocking.contX;
	float kY = 0.7f * flocking.ctrDirY + 0.2f * flocking.vel.x + 0.8f * flocking.contY;

	float tempVel = sqrtf(kX * kX + kY * kY);
	if (tempVel > 0.2f) {
		kX = 0.2f * kX / tempVel;
		kY = 0.2f * kY / tempVel;
	}

	dx += (kX - dx) * 0.02f;
	dy += (kY - dy) * 0.02f;


	pos.x += dx;
	pos.z += dy;
	fbxObj->SetPosition(pos);
}



void Bullet::SetCommand(const int& command, XMFLOAT3 pos) {
	this->command = command; AftaerPos = pos;
}