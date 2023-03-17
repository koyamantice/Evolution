#include"BulletGreen.h"
#include "Easing.h"
#include"ActorManager.h"
#include <SourceCode/FrameWork/collision/Collision.h>
#include"ImageManager.h"
#include"ModelManager.h"
using namespace DirectX;



BulletGreen::BulletGreen() {
}

void BulletGreen::OnInit() {
	ID = ActorManager::GetInstance()->SearchNum("BulletGreen");
	command = Wait;
	player = ActorManager::GetInstance()->SearchActor("Player");
	enemy = ActorManager::GetInstance()->SearchActorBack("Enemy");
	landing = player->GetLockPos();
	Color = DeathColor::Green;

	Object2d* Lock_ = Object2d::Create(ImageManager::Battle, { fbxObj->GetPosition().x,fbxObj->GetPosition().y + 1.0f,fbxObj->GetPosition().z
		}, { 0.1f,0.1f,0.1f }, { 1,1,1,1 });
	Lock_->SetIsBillboard(true);
	Lock_->Object2dCreate();
	Lock_->SetRotation({ 0,0,0 });
	Status.reset(Lock_);


	Object2d* CharaDead_ = Object2d::Create(Color, { fbxObj->GetPosition().x,fbxObj->GetPosition().y,fbxObj->GetPosition().z },
		{ 0.3f,0.3f,0.3f }, { 1,1,1,1 });
	CharaDead_->SetIsBillboard(true);
	CharaDead_->Object2dCreate();
	CharaDead_->SetRotation({ 0,0,0 });
	CharaDead.reset(CharaDead_);
}

void BulletGreen::OnUpda() {

}

void BulletGreen::OnDraw(DirectXCommon* dxCommon) {

}
void BulletGreen::OnFinal() {
}


void BulletGreen::BulletCollision(const XMFLOAT3& pos, const int& Id) {
	if (collide) { return; }
	if (command == Dead|| command ==Smash ) { return; }
	if (isPlayActive) { return; }
	if (ID < Id) { return; }
	collide = true;
	XMFLOAT3 pos2 = fbxObj->GetPosition();

	float dir = ((pos.x * pos2.z) - (pos2.x * pos.z));

	if (dir <= 0) {
		pos2.x += sin(atan2f((pos2.x - pos.x), (pos2.z - pos.z))) * 0.1f;
		pos2.z += cos(atan2f((pos2.x - pos.x), (pos2.z - pos.z))) * 0.1f;
	} else {
		pos2.x -= sin(atan2f((pos2.x - pos.x), (pos2.z - pos.z))) * 0.1f;
		pos2.z -= cos(atan2f((pos2.x - pos.x), (pos2.z - pos.z))) * 0.1f;

	}
	fbxObj->SetPosition(pos2);
}

void BulletGreen::ResultOnUpdate(const float& Timer) {
	XMFLOAT3 rot = fbxObj->GetRotation();
	XMFLOAT3 pos = fbxObj->GetPosition();
	XMFLOAT3 p_pos = player->GetPosition();
	if(clear_ease){
		if (clear_frame < 1.0f) {
			float seed_ = 0.01f * (ID / 30.0f);
			clear_frame += 0.01f + seed_;
		} else {
			clear_ease = false;
		}
		if (pos.y > 0) {
			float gra = 0.3f;
			pos.y -= gra;
		} else {
			pos.y = max(0, pos.y);
		}
		clear_e_pos = {
			p_pos.x + sinf(((int)ID * 40) * (XM_PI / 180)) * 5.0f,
			pos.y,
			p_pos.z + cosf(((int)ID * 40) * (XM_PI / 180)) * 5.0f,
		};
		pos.x=Ease(In,Linear,clear_frame,clear_s_pos.x,clear_e_pos.x);
		pos.z=Ease(In,Linear,clear_frame,clear_s_pos.z,clear_e_pos.z);
		rot.y = DirRotation(clear_e_pos);

	}else {
		pos.y += vel; //+
		float rnd_vel = 0.04f;
		float margin = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel -= rnd_vel + margin;//
		pos.y = max(0, pos.y);
		//��]
		float delay = 0.3f;
		if (frame + delay <= 1.0f) {
			rot.x = Ease(In, Quad, frame + delay, 0, -DEGREE_MAX);
		}
		if (frame < 1.0f) {
			frame += 0.02f;
		} else {
			frame = 0.0f;
			vel = 0.8f;
			rot.x = 0.0f;
			pos.y = 0.0f;
		}
	}
	fbxObj->SetPosition(pos);
	fbxObj->SetRotation(rot);
	fbxObj->Update();
}

