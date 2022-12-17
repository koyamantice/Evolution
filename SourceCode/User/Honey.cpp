#include "Honey.h"

void Honey::OnInit() {
	obj->SetScale({3,3,3});
	//•K—vl”
	stock = 0;

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
		WaitUpda();
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
	if (stock >= 5) {
		int a = 0;
		a++;
	}
	if (pos.y > -2.0f) {
		pos.y -= 0.1f;
	} else {
		pos.y = -2.0f;
	}

	obj->SetPosition(pos);
}

void Honey::WaitUpda() {
	XMFLOAT3 pos = obj->GetPosition();
	if (pos.y < 0) {
		pos.y += 0.3f;
	} else {
		pos.y = 0.0f;
	}
	if (stock >= 5) {
		pos.y = 5.0f;
	}

	obj->SetPosition(pos);


}
