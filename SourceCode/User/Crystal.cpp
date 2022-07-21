#include"Crystal.h"

void Crystal::OnInit() {
	obj->SetPosition(XMFLOAT3(5,0,0));
}

void Crystal::OnUpda() {
	obj->SetRotation(XMFLOAT3{ 0,obj->GetRotation().y - 1,0 });

}

void Crystal::OnDraw() {
}

void Crystal::OnFinal() {
}
