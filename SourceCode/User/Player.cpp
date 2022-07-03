#include"Player.h"

void Player::OnInit() {
	obj->SetRotation(XMFLOAT3{0,0,0});
}

void Player::OnUpda() {
	if (input->PushKey(DIK_W)) {
		XMFLOAT3 pos = obj->GetPosition();
		pos.z = obj->GetPosition().z - 0.4f;
		obj->SetPosition(pos);
	}
	if (input->PushKey(DIK_S)) {
		XMFLOAT3 pos = obj->GetPosition();
		pos.z = obj->GetPosition().z + 0.4f;
		obj->SetPosition(pos);
	}
	if (input->PushKey(DIK_D)) {
		XMFLOAT3 pos = obj->GetPosition();
		pos.x = obj->GetPosition().x - 0.4f;
		obj->SetPosition(pos);
	}
	if (input->PushKey(DIK_A)) {
		XMFLOAT3 pos=obj->GetPosition();
		pos.x=obj->GetPosition().x+0.4f;
		obj->SetPosition(pos);
	}
}

void Player::OnDraw() {
}

void Player::OnFinal() {
}
