#include"Crystal.h"
#include <SourceCode/FrameWork/ActorManager.h>
#include <SourceCode/FrameWork/collision/Collision.h>

void Crystal::OnInit() {
	obj->SetPosition(XMFLOAT3(0,3,-5));
	player = ActorManager::GetInstance()->SearchActor("Player");
}

void Crystal::OnUpda() {
	obj->SetRotation(XMFLOAT3{ 0,obj->GetRotation().y - 1,0 });
	obj->SetPosition(XMFLOAT3{ 0,sinf(angle * 0.05f) * 0.3f+3.5f,35.0f });
	angle++;
}

void Crystal::OnDraw(DirectXCommon* dxCommon) {
}

void Crystal::OnFinal() {
}
