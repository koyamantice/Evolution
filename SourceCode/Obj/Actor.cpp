#include"Actor.h"
#include <SourceCode/FrameWork/collision/Collision.h>
#include"ActorComponent.h"
#include <memory>
#include <SourceCode/FrameWork/ActorManager.h>

int Actor::id = 0;
int Actor::ID = 0;


void Actor::Initialize(Model* model, const std::string& tag, ActorComponent* compornent) {
	id++;
	ID++;
	if (isActive) {
		this->compornent = compornent;
		this->tag = tag;
		Object3d* obj_ = new Object3d();
		obj_->SetModel(model);
		obj_->Initialize();
		obj.reset(obj_);
		//ActorManager::GetInstance()->AttachList(this);
		OnInit();
		if (this->compornent) { this->compornent->Initialize(); }
	}
}

void Actor::Update() {
	if (isActive) {
		if (compornent) { compornent->Update(); }
		obj->Update();
		OnUpda();
	}
}

void Actor::Demo() {
	if (isActive) {
		obj->Update();
	}
}

void Actor::Draw() {
	if (isActive) {
		if (compornent) { compornent->Draw(); }
		Object3d::PreDraw();
		obj->Draw();
		OnDraw();
	}
}

void Actor::DemoDraw() {
	if (isActive) {
		Object3d::PreDraw();
		obj->Draw();
	}
	DebugUpdate();
}

void Actor::Finalize() {
	OnFinal();
}
