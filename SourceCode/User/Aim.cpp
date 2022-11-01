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
	XMFLOAT3 base = LockOn->GetPosition();
	XMFLOAT3 pos = player->GetPosition();
	if (input->TriggerButton(Input::B) || input->TriggerKey(DIK_SPACE)) {
		player = ActorManager::GetInstance()->SearchActor("Player");
		player->SetStock(player->GetStock() - 1);
		bullet = ActorManager::GetInstance()->SearchWaitBullet();
		if (bullet != nullptr) {
			bullet->SetCommand(Bullet::command::Slow, LockOn->GetPosition());
		}
	}

	if (input->PushButton(Input::A)) {
		collect = true;
		if (Area < 8.0f) {
			Area += 0.08f;
		}
		for (int i = 0; i < 1; i++) {
			const float rnd_rad = 360.0f;
			XMFLOAT3 spherePos{};
			float angle = (float)rand() / RAND_MAX * rnd_rad;
			spherePos.x = base.x + Area * sinf(angle);
			spherePos.z = base.z + Area * cosf(angle);
			const float rnd_vel = 0.4f;
			XMFLOAT3 vel{};
			vel.y = (float)rand() / RAND_MAX * rnd_vel;
			ParticleManager::GetInstance()->Add(15, spherePos, vel, XMFLOAT3(), 1.0f, 0.0f);
		}
		ActorManager::GetInstance()->ChangeBulletCommand(base, Area);
		alpha = 0.3f;
		Whistle->SetColor({1,1,1,alpha});

		whiframe = Area / 8.0f;
		if (whiframe < 1.0f) {
			float scaleArea = Ease(Out, Quad, whiframe, 0.5f, 2.0f);
			Whistle->SetScale({ scaleArea,scaleArea,scaleArea });
			Whistle->SetRotation({90,0,scaleArea * 180 });
		}
		Whistle->SetPosition(base);
	
	} else {
		Area = 0.0f;
		if (alpha>0.01f) {
			alpha *= 0.5f;
			Whistle->SetColor({ 1,1,1,alpha });
		}
		Whistle->SetPosition({0,-100,0});
		Whistle->SetScale({ 0.5f,0.5f,0.5f });
		collect = false;
	}

	if (input->TiltPushStick(Input::L_RIGHT, 0.0f) || input->TiltPushStick(Input::L_LEFT, 0.0f) || input->TiltPushStick(Input::L_UP, 0.0f) || input->TiltPushStick(Input::L_DOWN, 0.0f)) {
		XMFLOAT3 base = LockOn->GetPosition();
		float StickX = input->GetLeftControllerX();
		float StickY = input->GetLeftControllerY();
		const float PI = 3.14159f;
		const float STICK_MAX = 32768.0f;
		if (input->TiltPushStick(Input::L_UP, 0.0f)) {
			XMFLOAT3 vecvel = MoveVector(XMVECTOR{ 0,0,2,0 }, angle);
			base.x -= vecvel.x * (StickY / STICK_MAX);
			base.z -= vecvel.z * (StickY / STICK_MAX);
		}
		if (input->TiltPushStick(Input::L_DOWN, 0.0f)) {
			XMFLOAT3 vecvel = MoveVector(XMVECTOR{ 0,0,-2,0 }, angle);
			base.x += vecvel.x * (StickY / STICK_MAX);
			base.z += vecvel.z * (StickY / STICK_MAX);
		}
		if (input->TiltPushStick(Input::L_RIGHT, 0.0f)) {
			XMFLOAT3 vecvel = MoveVector(XMVECTOR{ 2,0,0,0 }, angle);
			base.x -= vecvel.x * (StickX / STICK_MAX);
			base.z -= vecvel.z * (StickX / STICK_MAX);
		}
		if (input->TiltPushStick(Input::L_LEFT, 0.0f)) {
			XMFLOAT3 vecvel = MoveVector(XMVECTOR{ -2,0,0,0 }, angle);
			base.x += vecvel.x * (StickX / STICK_MAX);
			base.z += vecvel.z * (StickX / STICK_MAX);
		}
		LockOn->SetPosition(base);

	}

	for (int i = 0; i < GuidNum; i++) {
		GuidPos[i].x = Ease(InOut, Quad, (i + 1) * 0.1f, pos.x, base.x);
		GuidPos[i].y = 0.1f;
		GuidPos[i].z = Ease(InOut, Quad, (i + 1) * 0.1f, pos.z, base.z);
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