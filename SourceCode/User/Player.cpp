#include"Player.h"
#include"ModelManager.h"

void Player::DebugUpdate() {
}

void Player::OnInit() {
	obj->SetRotation(XMFLOAT3{0,0,0});
}

void Player::OnUpda() {
	Move();
	Shot();
	for (std::unique_ptr<Bullet>& bullet:bullets) {
		bullet->Update();
	}
}

void Player::OnDraw() {
	for (std::unique_ptr<Bullet>& bullet : bullets) {
		bullet->Draw();
	}
}

void Player::OnFinal() {
}

void Player::Move() {
	XMFLOAT3 pos = obj->GetPosition();
	if (input->PushKey(DIK_W)) {
		pos.z = obj->GetPosition().z - 0.4f;
	}
	if (input->PushKey(DIK_S)) {
		pos.z = obj->GetPosition().z + 0.4f;
	}
	if (input->PushKey(DIK_D)) {
		pos.x = obj->GetPosition().x - 0.4f;
	}
	if (input->PushKey(DIK_A)) {
		pos.x = obj->GetPosition().x + 0.4f;
	}

	float StickX= input->GetPosX();
	float StickY = input->GetPosY();
	const float vel = 0.3f;
	if (!(StickX<650 && StickX>-650)) {
		pos.x -= sin(atan2(StickX, StickY)) * vel;
		//rot.y = ((-atan2(StickrotX, StickrotY) * (180.0f / XM_PI))) + 90;
	}
	if (!(StickY<650 && StickY>-650)) {
		pos.z += cos(atan2(StickX, StickY)) * vel;
		//rot.y = ((-atan2(StickrotX, StickrotY) * (180.0f / XM_PI))) + 90;
	}



	obj->SetPosition(pos);

}

void Player::Shot() {
	if (input->TriggerKey(DIK_SPACE)||input->TriggerButton(input->Button_A)) {
		std::unique_ptr<Bullet> newBullet = std::make_unique<Bullet>();
		newBullet->Initialize(ModelManager::GetIns()->GetModel(ModelManager::hole));
		newBullet->SetPosition(obj->GetPosition());
		bullets.push_back(std::move(newBullet));
	}
}
