#include"Actor.h"
#include <SourceCode/FrameWork/collision/Collision.h>
#include"ActorComponent.h"
#include <memory>
#include <SourceCode/FrameWork/ActorManager.h>

int Actor::id = 0;

void Actor::Initialize(Model* model, const std::string& tag, ActorComponent* compornent) {
	id++;
	if (isActive) {
		this->tag = tag;
		Object3d* obj_ = new Object3d();
		obj_->SetModel(model);
		obj_->Initialize();
		obj.reset(obj_);
		OnInit();
	}
}

void Actor::Update() {
	if (isActive) {
		if (compornent) { compornent->Update(); }
		old_pos = obj->GetPosition();
		if (isVisible) {
			obj->Update();
		}
		OnUpda();
	}
}

void Actor::IntroUpdate(const float& Timer) {
	if (isVisible) {
		obj->Update();
	}
	IntroOnUpdate(Timer);
}

void Actor::ResultUpdate(const float& Timer) {
	isResult = true;
	if (isVisible) {
		obj->Update();
	}
	ResultOnUpdate(Timer);
}

void Actor::Demo() {
	if (isActive) {
		obj->Update();
	}
}

void Actor::FirstDraw(DirectXCommon* dxCommon) {
	if (isActive) {
		OnFirstDraw(dxCommon);
	}
}

void Actor::Draw(DirectXCommon* dxCommon) {
	if (isActive) {
		if (isVisible) {
			Object3d::PreDraw();
			obj->Draw();
		}
		OnDraw(dxCommon);
	}
}

void Actor::LastDraw(DirectXCommon* dxCommon) {
	if (isActive) {
		OnLastDraw(dxCommon);
	if (compornent) { compornent->Draw(); }
	}
}

void Actor::DemoDraw(DirectXCommon* dxCommon) {
	if (isActive) {
		DebugUpdate();
		if (isVisible) {
			Object3d::PreDraw();
			obj->Draw();
		}
		OnDraw(dxCommon);
	}
}

void Actor::Finalize() {
	OnFinal();
}
