#include "Aim.h"
#include "ImageManager.h"
#include "ActorManager.h"
#include <DirectXMath.h>
#include <SourceCode/Common/Easing.h>
using namespace DirectX;

void Aim::Init() {
	Object2d* Lock_ = Object2d::Create(ImageManager::Lock, { 0,0,0 }, { 0.5f,0.5f,0.5f }, { 1,1,1,1 });
	Lock_->SetRotation({ 90,0,0 });
	Lock_->SetColor({ 1.0f,0.2f,0.2f ,0.6f });
	LockOn.reset(Lock_);

	Object2d* Whistle_ = Object2d::Create(ImageManager::Lock, { 0,0,0 }, { 0.5f,0.5f,0.5f }, { 1,1,1,1 });
	Whistle_->SetRotation({ 90,0,0 });
	Whistle_->SetColor({ 1.0f,1.0f,1.0f ,0.5f });
	Whistle.reset(Whistle_);


	for (int i = 0; i < COMMENTMAX;i++) {
		Object2d* UI_ = Object2d::Create(ImageManager::SlowUI + i, { 0,0,0 }, { 0.3f,0.3f,0.3f }, { 1,1,1,1 });
		UI_->SetRotation({ 0,0,0 });
		UI_->SetIsBillboard(true);
		comment_ui_[i].reset(UI_);
		comment_ui_[i]->SetPosition({100,-50,0});

	}


	for (int i = 0; i < GuidNum; i++) {
		Object2d* Guid_ = Object2d::Create(ImageManager::Guid, { 0,0,0 }, { 0.1f,0.1f,0.1f }, { 1,1,1,1 });
		Guid_->SetRotation({ DEGREE_QUARTER,0,0 });
		Guid_->SetColor({ 1.0f,1.0f,1.0f ,0.5f });
		Guid[i].reset(Guid_);
	}

	partMan = new ParticleManager();
	partMan->Initialize(ImageManager::nul);

	audioManager = std::make_unique<AudioManager>();

	audioManager->LoadWave("SE/slow.wav");

	enemy_set = true;
}

void Aim::Upda(float angle) {
	LockOn->Update();
	Whistle->Update();
	partMan->Update();
	for (int i = 0; i < COMMENTMAX; i++) {
		comment_ui_[i]->Update();
	}
	animeframe++;
	if (animeframe > 50) {
		Animation += vel;
		vel *= -1;
		animeframe = 0;
	}

	if (explanation_now_ >= RECOVERY) {
		FirstAlpha *= 0.9f;
		comment_ui_[SHOT]->SetColor({ 1,1,1,FirstAlpha });
	}
	if (explanation_now_ >= COMMENTMAX) {
		SecondAlpha *= 0.9f;
		comment_ui_[RECOVERY]->SetColor({ 1,1,1,SecondAlpha });
		comment_ui_[RECOVERYPUSH]->SetColor({ 1,1,1,SecondAlpha });
	}
	for (int i = 0; i < GuidNum; i++) {
		Guid[i]->Update();
	}
	Move(angle);
	EnemySet();

}

void Aim::Draw() {
	if (!isActive) { return; }
	Object2d::PreDraw();
	LockOn->Draw();
	Whistle->Draw();
	comment_ui_[SHOT]->Draw();
	if (explanation_now_ == RECOVERY) {
		comment_ui_[RECOVERY+Animation]->Draw();
	}
	for (int i = 0; i < GuidNum; i++) {
		Guid[i]->Draw();
	}
	partMan->Draw(addBle);
}



void Aim::FirstSet() {
	player = ActorManager::GetInstance()->SearchActor("Player");
	XMFLOAT3 pos = player->GetPosition();
	LockOn->SetPosition({ pos.x,pos.y + 0.1f,pos.z });

}

void Aim::Move(float angle) {
	//“®‚«Ž‚½‚¹‚é‚½‚ßƒNƒ‹ƒNƒ‹‚³‚¹‚Ä‚Ü‚·B
	XMFLOAT3 Lrot = LockOn->GetRotation();
	Lrot.y += 2.0f;
	LockOn->SetRotation(Lrot);

	if (input->TriggerButton(Input::B) || input->TriggerKey(DIK_SPACE)) {
		if (explanation_now_== SHOT) {
			explanation_now_ = RECOVERY;
		}
		player = ActorManager::GetInstance()->SearchActor("Player");
		player->SetStock(player->GetStock() - 1);
		bullet = ActorManager::GetInstance()->SearchWaitBullet();
		if (bullet != nullptr) {
			audioManager->PlayWave("SE/slow.wav", 0.5f);
			bullet->SetCommand(Bullet::command::Slow, LockOn->GetPosition());
		}
	}

	if (input->PushButton(Input::A) || input->PushKey(DIK_Q)) {
		if (explanation_now_ == RECOVERY) {
			explanation_now_ = COMMENTMAX;
		}
		if (Area < 10.0f) {
			Area += 0.20f;
		}
		XMFLOAT3 base = LockOn->GetPosition();
		for (int i = 0; i < 1; i++) {
			const float rnd_rad = DEGREE_MAX;
			XMFLOAT3 pos{};
			float angle = (float)rand() / RAND_MAX * rnd_rad;
			pos.x = base.x + (Area+0.5f) * sinf(angle);
			pos.z = base.z + (Area+0.5f) * cosf(angle);
			const float rnd_vel = 0.4f;
			XMFLOAT3 vel{};
			vel.y = (float)rand() / RAND_MAX * rnd_vel;

			partMan->Add(45, pos, vel, {}, 0.5f, 0.0f, { 1.0f,1.0f,1.0f,1.0f }, { 1.0f,1.0f,1.0f,0.0f });
		}
		ActorManager::GetInstance()->ChangeBulletCommand(base, Area);
		Whistle->SetPosition({ base.x, base.y+(Area/4), base.z });
		Whistle->SetScale({ Area * 0.3f,Area * 0.3f,Area * 0.3f });
		Whistle->SetRotation({90 ,Lrot.y * 2.0f ,Lrot.z * 2.0f });
		Whistle->SetColor({ 1.0f,1.0f,1.0f,1.1f-(Area/10.0f) });

	} else {
		Area = 0.0f;
		Whistle->SetScale({ Area,Area,Area });
	} 

	XMFLOAT3 Lpos = LockOn->GetPosition();

	if (input->TiltPushStick(Input::L_UP   ,0.0f) ||
		input->TiltPushStick(Input::L_DOWN ,0.0f) ||
		input->TiltPushStick(Input::L_RIGHT,0.0f) ||
		input->TiltPushStick(Input::L_LEFT ,0.0f) ||
		input->PushKey(DIK_W) ||
		input->PushKey(DIK_S) ||
		input->PushKey(DIK_D) ||
		input->PushKey(DIK_A)||
		enemy_set) {
		after_pos = player->GetCameraPos(player->GetRotation().y,10);
		enemy_set = false;
	}
	Lpos.x = Ease(In, Quad, 0.5f, Lpos.x, after_pos.x);
	Lpos.y = Ease(In, Quad, 0.5f, Lpos.y, after_pos.y);
	Lpos.z = Ease(In, Quad, 0.5f, Lpos.z, after_pos.z);
	if (Lpos.x > 48.0f) { Lpos.x = 48.0f; }
	if (Lpos.x < -48.0f) { Lpos.x = -48.0f; }
	if (Lpos.z > 48.0f) { Lpos.z = 48.0f; }
	if (Lpos.z < -48.0f) { Lpos.z = -48.0f; }
	LockOn->SetPosition({ Lpos.x,0.01f,Lpos.z });

	for (int i = 0; i < COMMENTMAX; i++) {
		comment_ui_[i]->SetPosition({ Lpos.x,1.0f * sinf((Lrot.y + 2) * XM_PI / DEGREE_HALF) + 3.5f ,Lpos.z });
	}

	XMFLOAT3 pos = player->GetPosition();
	for (int i = 0; i < GuidNum; i++) {
		GuidPos[i].x = Ease(InOut, Quad, (i + 1) * 0.1f, pos.x, Lpos.x);
		GuidPos[i].y = 0.1f;
		GuidPos[i].z = Ease(InOut, Quad, (i + 1) * 0.1f, pos.z, Lpos.z);
		Guid[i]->SetPosition(GuidPos[i]);
	}
}

void Aim::EnemySet() {
	if (input->TriggerButton(Input::RT)||
		input->TriggerKey(DIK_R)) {
		XMFLOAT3 base = player->GetPosition();
		Actor* enemy = ActorManager::GetInstance()->SearchActorArea(base);
		if (enemy->GetTag()=="Player") { return; }
		XMFLOAT3 boss = enemy->GetPosition();
		float itr = 0;
		XMFLOAT3 position{};
		position.x = (boss.x - base.x);
		position.z = (boss.z - base.z);
		itr = (atan2f(position.x, position.z) * (DEGREE_HALF / XM_PI)) + DEGREE_HALF; //- 90;// *(XM_PI / 180.0f);
		if (itr >= 0) {
			itr = (float)((int)itr % (int)DEGREE_MAX);
		} else {
			itr += DEGREE_MAX;
			itr = (float)((int)itr % (int)DEGREE_MAX);
		}

		player->SetRotation({ 0,itr,0 });
		player->SetPause(true);
		if (!enemy_set) {
			enemy_set = true;
		}
	}
}


DirectX::XMFLOAT3 Aim::MoveVector(XMVECTOR v, float angle) {
	XMMATRIX rot{};
	rot = XMMatrixRotationY(XMConvertToRadians(angle));
	v = XMVector3TransformNormal(v, rot);
	XMFLOAT3 pos = { v.m128_f32[0],v.m128_f32[1] ,v.m128_f32[2] };
	return pos;
}