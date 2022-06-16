#include"Actor.h"
#include <SourceCode/FrameWork/collision/Collision.h>

int Actor::ID = 0;


bool Actor::Collide(Actor* actor) {
	//�Z�b�g�|�W�V�����Ƃ̃R���W����
	if (Collision::SphereCollision2(obj->GetPosition(), 1.0f, actor->obj->GetPosition(), 1.0f)) {
		if (actor->GetTag() == std::string("enemy")) {//�^�Ocheck
			return true;
		} else {
			return false;
		}
	} else {
		return false;
	}
}

void Actor::Initialize(Model* model) {
	Object3d* obj_ = new Object3d();
	obj_->SetModel(model);
	obj_->Initialize();
	obj.reset(obj_);
}

void Actor::Update() {
	obj->Update();
}

void Actor::Draw() {
	Object3d::PreDraw();
	obj->Draw();
}
