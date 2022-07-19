#include"Actor.h"
#include <SourceCode/FrameWork/collision/Collision.h>
#include"ActorComponent.h"
#include <memory>
#include <SourceCode/FrameWork/ActorManager.h>

int Actor::ID = 0;

bool Actor::Collide(Actor* actor) {
	//セットポジションとのコリジョン
	if (Collision::SphereCollision2(obj->GetPosition(), 1.0f, actor->obj->GetPosition(), 1.0f)) {
		if (actor->GetTag() == std::string("enemy")) {//タグcheck
			return true;
		} else {
			return false;
		}
	} else {
		return false;
	}
}

void Actor::Initialize(Model* model, const std::string& tag, ActorComponent* compornent) {
	this->compornent = compornent;
	this->tag = tag;
	if (this->compornent) { this->compornent->Initialize(); }
	Object3d* obj_ = new Object3d();
	obj_->SetModel(model);
	obj_->Initialize();
	obj.reset(obj_);
	//ActorManager::GetInstance()->AttachList(this);
	OnInit();
}

void Actor::Update() {
	if (compornent) { compornent->Update(); }
	obj->Update();
	OnUpda();
}

void Actor::Draw() {
	if (compornent) { compornent->Draw(); }
	Object3d::PreDraw();
	obj->Draw();
	OnDraw();
}

void Actor::Finalize() {
	OnFinal();
}
