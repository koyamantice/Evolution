#include "Honey.h"

void Honey::OnInit() {
	obj->SetScale({3,3,3});


}


void Honey::OnUpda() {
	switch (command) {
	case Actor::Phase::APPROCH:
		//ApprochUpda();
		break;
	case Actor::Phase::LEAVE:
		LeaveUpda();
		break;
	case Actor::Phase::WAIT:
		//WaitUpda();
		break;
	case Actor::Phase::ATTACK:
		//AttackUpda();
		break;
	default:
		break;
	}
}

void Honey::OnDraw(DirectXCommon* dxCommon) {
}

void Honey::OnFinal() {
}

void Honey::LeaveUpda() {
	XMFLOAT3 pos = obj->GetPosition();
	pos.y -= 0.1f;


	obj->SetPosition(pos);
}

void Honey::WaitUpda() {



}
