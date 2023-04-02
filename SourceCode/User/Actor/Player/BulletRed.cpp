#include"BulletRed.h"
#include"ActorManager.h"
#include"ModelManager.h"
#include <SourceCode/FrameWork/collision/Collision.h>
#include <SourceCode/Common/Easing.h>
using namespace DirectX;



BulletRed::BulletRed() {
}

void BulletRed::OnInitialize() {
	ID = ActorManager::GetInstance()->SearchNum("BulletRed");
	command_ = BulletStatus::Wait;
	player = ActorManager::GetInstance()->SearchActor("Player");
	enemy = ActorManager::GetInstance()->SearchActor("Enemy");
	if (enemy == nullptr) { noBoss = true; };

	landing = player->GetLockPos();
	Color = DeathColor::Red;

	chara_dead_ = Object2d::Create(Color, { fbxobj_->GetPosition().x,fbxobj_->GetPosition().y,fbxobj_->GetPosition().z },
		{ 0.3f,0.3f,0.3f }, { 1,1,1,1 });
	chara_dead_->SetIsBillboard(true);
	chara_dead_->SetRotation({ 0,0,0 });

}


void BulletRed::ResultOnUpdate(const float& timer) {
	XMFLOAT3 rot = fbxobj_->GetRotation();
	XMFLOAT3 pos = fbxobj_->GetPosition();
	XMFLOAT3 p_pos = player->GetPosition();
	if (clear_ease) {
		if (clear_frame < 1.0f) {
			clear_frame += 0.01f;
		} else {
			clear_ease = false;
		}
		clear_e_pos = {
			p_pos.x + sinf(((int)ID * 40) * (XM_PI / 180)) * 8.0f,
			0,
			p_pos.z + cosf(((int)ID * 40) * (XM_PI / 180)) * 8.0f,
		};
		pos.x = Ease(In, Linear, clear_frame, clear_s_pos.x, clear_e_pos.x);
		pos.z = Ease(In, Linear, clear_frame, clear_s_pos.z, clear_e_pos.z);
		rot.y= DirRotation(clear_e_pos);
	} else {
		pos.y += vel_; //+
		float rnd_vel = 0.04f;
		float margin= (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel_ -= 0.04f+ margin;//
		if (frame < 0.7f) {
			rot.x = Ease(In, Quad, frame + 0.3f, 0, -360);
		}
		pos.y = max(0,pos.y);
		if (frame <= 1.0f) {
			frame += 0.02f;
		} else {
			frame = 0.0f;
			vel_ = 0.8f;
			rot.x = 0.0f;
			pos.y = 0.0f;
		}
	}
	fbxobj_->SetPosition(pos);
	fbxobj_->SetRotation(rot);
	fbxobj_->Update();
	shadow_->SetPosition({ fbxobj_->GetPosition().x,0.01f, fbxobj_->GetPosition().z });
	shadow_->Update();
}

void BulletRed::BulletCollision(const XMFLOAT3& pos, const int& Id) {
	if (collide) { return; }
	if (command_ == BulletStatus::Dead || command_ == BulletStatus::Smash) { return; }
	if (isPlayActive) { return; }
	if (ID < Id) { return; }
	collide = true;
	XMFLOAT3 pos2 = fbxobj_->GetPosition();

	float dir = ((pos.x * pos2.z) - (pos2.x * pos.z));

	if (dir <= 0) {
		pos2.x += sin(atan2f((pos2.x - pos.x), (pos2.z - pos.z))) * 0.1f;
		pos2.z += cos(atan2f((pos2.x - pos.x), (pos2.z - pos.z))) * 0.1f;
	} else {
		pos2.x -= sin(atan2f((pos2.x - pos.x), (pos2.z - pos.z))) * 0.1f;
		pos2.z -= cos(atan2f((pos2.x - pos.x), (pos2.z - pos.z))) * 0.1f;

	}
	fbxobj_->SetPosition(pos2);
}



