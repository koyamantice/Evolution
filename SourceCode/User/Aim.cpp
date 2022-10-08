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
	EnemySet();
}

void Aim::Draw() {
	Texture::PreDraw();
	LockOn->Draw();

}

void Aim::FirstSet() {
	player = ActorManager::GetInstance()->SearchActor("Player");
	LockOn->SetPosition(player->GetPosition());

}

void Aim::Move() {
	if (input->TriggerButton(Input::B) || input->TriggerKey(DIK_SPACE)) {
		player = ActorManager::GetInstance()->SearchActor("Player");
		player->SetStock(player->GetStock()-1);
		bullet = ActorManager::GetInstance()->SearchWaitBullet();
		if (bullet!=nullptr) {
			bullet->SetCommand(Actor::command::Attack,LockOn->GetPosition());
		}
	}
	if (input->PushButton(Input::A)) {
		Area += 0.02f;
		ActorManager::GetInstance()->ChangeBulletCommand(LockOn->GetPosition(),Area);
	} else {
		Area = 0.3f;
	}

	if (input->TiltPushStick(Input::L_RIGHT) || input->TiltPushStick(Input::L_LEFT) || input->TiltPushStick(Input::L_UP) || input->TiltPushStick(Input::L_DOWN)) {
		if (input->TiltPushStick(Input::L_RIGHT, 0.01f)) {
			angle -= abs(angle*0.1f);
			if (angle<=0) {
				angle -= (abs(angle) + 1) * 0.1f;
			}
		}
		if (input->TiltPushStick(Input::L_LEFT, 0.01f)) {
			angle += 0.3f;
		}
		if (input->TiltPushStick(Input::L_UP, 0.01f)) {
			distance -= 0.3f;
		}
		if (input->TiltPushStick(Input::L_DOWN, 0.01f)) {
			distance += 0.3f;
		}
		if (angle > 10.0) {
			angle = 10.0;
		}
		if (distance > 10.0) {
			distance = 10.0;
		}
		if (angle < -10.0) {
			angle = -10.0;
		}
		if (distance < -10.0) {
			distance = -10.0;
		}
		XMFLOAT3 Lpos = LockOn->GetPosition();
		XMFLOAT3 plapos = player->GetPosition();
		Lpos.x = plapos.x + angle;
		Lpos.y = 0.18f;
		Lpos.z = plapos.z +distance;
		LockOn->SetPosition(Lpos);

	}

}

void Aim::EnemySet() {
	if (input->TriggerButton(Input::RT)) {
		Actor* enemy = ActorManager::GetInstance()->SearchActorArea(player->GetPosition());
		//->SearchActor("Enemy");

		XMFLOAT3 Lpos = LockOn->GetPosition();
		Lpos = enemy->GetPosition();
		Lpos.y = 0.18f;
		LockOn->SetPosition(Lpos);
	}
}
