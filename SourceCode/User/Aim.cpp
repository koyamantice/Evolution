#include "Aim.h"
#include"ImageManager.h"
#include"ActorManager.h"
#include<DirectXMath.h>
#include <SourceCode/Common/Easing.h>
using namespace DirectX;

void Aim::Init() {
	Texture* Lock_ = Texture::Create(ImageManager::Lock, { 0,0,0 }, { 0.5f,0.5f,0.5f }, { 1,1,1,1 });
	Lock_->TextureCreate();
	Lock_->SetRotation({ 90,0,0 });
	Lock_->SetColor({ 1.0f,0.2f,0.2f ,0.6f });
	LockOn.reset(Lock_);
	LockOn->SetPosition({ 100,-50,0 });

	Texture* Whistle_ = Texture::Create(ImageManager::Lock, { 0,0,0 }, { 0.5f,0.5f,0.5f }, { 1,1,1,1 });
	Whistle_->TextureCreate();
	Whistle_->SetRotation({ 90,0,0 });
	Whistle_->SetColor({ 1.0f,0.2f,0.2f ,0.6f });
	Whistle.reset(Whistle_);
	Whistle->SetPosition({ 100,-50,0 });



	for (int i = 0; i < GuidNum; i++) {
		Texture* Guid_ = Texture::Create(ImageManager::Guid, { 0,0,0 }, { 0.1f,0.1f,0.1f }, { 1,1,1,1 });
		Guid_->TextureCreate();
		Guid_->SetRotation({ 90,0,0 });
		Guid_->SetColor({ 1.0f,1.0f,1.0f ,0.6f });
		Guid[i].reset(Guid_);
	}
}

void Aim::Upda(float angle) {
	LockOn->Update();
	Whistle->Update();
	for (int i = 0; i < GuidNum; i++) {
		Guid[i]->Update();
	}
	Move(angle);
	EnemySet();

}

void Aim::Draw() {
//ImGui::Begin("test");
//for (int i = 0; i < GuidNum; i++) {
//	ImGui::SliderFloat("@:@", &GuidPos[i].y, 0, 360);
//}
//////ImGui::Unindent();
//ImGui::End();

	Texture::PreDraw();
	LockOn->Draw();
	Whistle->Draw();
	for (int i = 0; i < GuidNum; i++) {
		Guid[i]->Draw();
	}
}



void Aim::FirstSet() {
	player = ActorManager::GetInstance()->SearchActor("Player");
	XMFLOAT3 pos = player->GetPosition();
	LockOn->SetPosition({ pos.x,pos.y + 0.1f,pos.z });

}

void Aim::Move(float angle) {
	if (input->TriggerButton(Input::B) || input->TriggerKey(DIK_SPACE)) {
		player = ActorManager::GetInstance()->SearchActor("Player");
		player->SetStock(player->GetStock() - 1);
		bullet = ActorManager::GetInstance()->SearchWaitBullet();
		if (bullet != nullptr) {
			bullet->SetCommand(Bullet::command::Slow, LockOn->GetPosition());
		}
	}

	if (input->PushButton(Input::A)) {
		if (Area < 8.0f) {
			Area += 0.08f;
		}
		XMFLOAT3 base = LockOn->GetPosition();
		for (int i = 0; i < 1; i++) {
			const float rnd_rad = 360.0f;
			XMFLOAT3 pos{};
			float angle = (float)rand() / RAND_MAX * rnd_rad;
			pos.x = base.x + Area * sinf(angle);
			pos.z = base.z + Area * cosf(angle);
			const float rnd_vel = 0.4f;
			XMFLOAT3 vel{};
			//vel.x = (float)rand() / RAND_MAX * rnd_vel;// -rnd_vel / 2.0f;
			vel.y = (float)rand() / RAND_MAX * rnd_vel;
			//vel.z = -(float)rand() / RAND_MAX * rnd_vel;// -rnd_vel / 2.0f;
			ParticleManager::GetInstance()->Add(15, pos, vel, XMFLOAT3(), 1.0f, 0.0f);
		}
		ActorManager::GetInstance()->ChangeBulletCommand(base, Area);
	} else {
		Area = 0.0f;
	}

	if (input->TiltPushStick(Input::L_RIGHT, 0.0f) || input->TiltPushStick(Input::L_LEFT, 0.0f) || input->TiltPushStick(Input::L_UP, 0.0f) || input->TiltPushStick(Input::L_DOWN, 0.0f)) {
		XMFLOAT3 Lpos = LockOn->GetPosition();
		float StickX = input->GetLeftControllerX();
		float StickY = input->GetLeftControllerY();
		const float PI = 3.14159f;
		const float STICK_MAX = 32768.0f;
		if (input->PushKey(DIK_W) || input->TiltPushStick(Input::L_UP, 0.0f)) {
			XMFLOAT3 vecvel = MoveVector(XMVECTOR{ 0,0,2,0 }, angle);
			Lpos.x -= vecvel.x * (StickY / STICK_MAX);
			Lpos.z -= vecvel.z * (StickY / STICK_MAX);
		}
		if (input->PushKey(DIK_S) || input->TiltPushStick(Input::L_DOWN, 0.0f)) {
			XMFLOAT3 vecvel = MoveVector(XMVECTOR{ 0,0,-2,0 }, angle);
			Lpos.x += vecvel.x * (StickY / STICK_MAX);
			Lpos.z += vecvel.z * (StickY / STICK_MAX);
		}
		if (input->PushKey(DIK_D) || input->TiltPushStick(Input::L_RIGHT, 0.0f)) {
			XMFLOAT3 vecvel = MoveVector(XMVECTOR{ 2,0,0,0 }, angle);
			Lpos.x -= vecvel.x * (StickX / STICK_MAX);
			Lpos.z -= vecvel.z * (StickX / STICK_MAX);
		}
		if (input->PushKey(DIK_A) || input->TiltPushStick(Input::L_LEFT, 0.0f)) {
			XMFLOAT3 vecvel = MoveVector(XMVECTOR{ -2,0,0,0 }, angle);
			Lpos.x += vecvel.x * (StickX / STICK_MAX);
			Lpos.z += vecvel.z * (StickX / STICK_MAX);
		}
		if (Lpos.x > 47.0f) {
			Lpos.x = 47.0f;
		}
		if (Lpos.x < -47.0f) {
			Lpos.x = -47.0f;
		}
		if (Lpos.z > 47.0f) {
			Lpos.z = 47.0f;
		}
		if (Lpos.z < -47.0f) {
			Lpos.z = -47.0f;
		}
		LockOn->SetPosition(Lpos);

	}

	XMFLOAT3 Lpos = LockOn->GetPosition();
	XMFLOAT3 pos = player->GetPosition();

	for (int i = 0; i < GuidNum; i++) {
		GuidPos[i].x = Ease(InOut, Quad, (i + 1) * 0.1f, pos.x, Lpos.x);
		GuidPos[i].y = 0.1f;
		GuidPos[i].z = Ease(InOut, Quad, (i + 1) * 0.1f, pos.z, Lpos.z);
		Guid[i]->SetPosition(GuidPos[i]);
	}
}

void Aim::EnemySet() {
	if (input->TriggerButton(Input::RT)) {
		Actor* enemy = ActorManager::GetInstance()->SearchActorArea(player->GetPosition());
		//->SearchActor("Enemy");
		XMFLOAT3 base = LockOn->GetPosition();
		base = enemy->GetPosition();
		base.y = 0.18f;
		LockOn->SetPosition(base);
	}
}


DirectX::XMFLOAT3 Aim::MoveVector(XMVECTOR v, float angle) {
	XMMATRIX rot;
	rot = XMMatrixRotationY(XMConvertToRadians(angle));
	v = XMVector3TransformNormal(v, rot);
	XMFLOAT3 pos = { v.m128_f32[0],v.m128_f32[1] ,v.m128_f32[2] };
	return pos;
}