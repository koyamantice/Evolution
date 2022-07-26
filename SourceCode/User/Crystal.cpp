#include"Crystal.h"
#include <SourceCode/FrameWork/ActorManager.h>
#include <SourceCode/FrameWork/collision/Collision.h>

void Crystal::OnInit() {
	obj->SetPosition(XMFLOAT3(5,3,0));
	player = ActorManager::GetInstance()->SearchActor("Player");
	enemy = ActorManager::GetInstance()->SearchActor("Enemy");

}

void Crystal::OnUpda() {
	obj->SetRotation(XMFLOAT3{ 0,obj->GetRotation().y - 1,0 });

	//if (Collision::SphereCollision2()) {

	//}

}

void Crystal::OnDraw() {
}

void Crystal::OnFinal() {
}
