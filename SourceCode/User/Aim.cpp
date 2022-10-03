#include "Aim.h"
#include"ImageManager.h"
#include"ActorManager.h"
#include<DirectXMath.h>
using namespace DirectX;

void Aim::Init() {
	Texture* Lock_ = Texture::Create(ImageManager::Lock,{0,0,0}, {0.5f,0.5f,0.5f}, {1,1,1,1});
	Lock_->TextureCreate();
	Lock_->SetRotation({ 90,0,0 });
	Lock_->SetColor({ 1.0f,0.2f,0.2f ,0.6f });
	LockOn.reset(Lock_);

}

void Aim::Upda() {
	LockOn->Update();
	Move();

}

void Aim::Draw() {
	Texture::PreDraw();
	LockOn->Draw();

}

void Aim::Move() {
	if (input->TriggerButton(input->A) || input->TriggerKey(DIK_SPACE)) {
		player->SetStock(player->GetStock()-1);
		bullet = ActorManager::GetInstance()->SearchWaitBullet();
		if (bullet!=nullptr) {
			bullet->SetCommand(Actor::command::Attack,LockOn->GetPosition());
		}
	}
	if (input->TriggerKey(DIK_L)) {
		LockOn->GetPosition();

	}
	if (input->PushKey(DIK_RIGHT) || input->PushKey(DIK_LEFT)|| input->PushKey(DIK_UP) || input->PushKey(DIK_DOWN)) {
		if (input->PushKey(DIK_RIGHT)) {
			angle -= 3;
		}
		if (input->PushKey(DIK_LEFT)) {
			angle += 3;
		}
		if (input->PushKey(DIK_UP)) {
			distance += 1;
		}
		if (input->PushKey(DIK_DOWN)) {
			distance -= 1;
		}
	}
	if (distance < 3.0) {
		distance = 3.0;
	}
	if (distance > 15.0) {
		distance = 15.0;
	}

	player = ActorManager::GetInstance()->SearchActor("Player");
	XMFLOAT3 Lpos = LockOn->GetPosition();
	XMFLOAT3 plapos = player->GetPosition();
	Lpos.x = plapos.x + sinf(angle * (XM_PI / 180)) * distance;
	Lpos.y = 0.18f;
	Lpos.z = plapos.z + cosf(angle * (XM_PI / 180)) * distance;
	LockOn->SetPosition(Lpos);
}
