#include "Honey.h"
#include <ActorManager.h>
#include <Easing.h>

void Honey::OnInit() {
	obj->SetScale({3,3,3});
	//ïKóvêlêî
	stock = 0;

	for (int i = 0; i < 2;i++) {
		for (int j = 0; j < 6;j++) {
			Object2d* mission_ = Object2d::Create(ImageManager::kMission_0 + j, { 0,0,0 },
				{ 0.2f,0.2f,0.2f }, { 1,1,1,1 });
			mission_->SetIsBillboard(true);
			mission_->Object2dCreate();
			mission_->SetRotation({ 0,0,0 });
			missions[i][j].reset(mission_);
		}
	}
	Object2d* slash_ = Object2d::Create(ImageManager::kMission_s, { 0,0,0 },
		{ 0.2f,0.2f,0.2f }, { 1,1,1,1 });
	slash_->SetIsBillboard(true);
	slash_->Object2dCreate();
	slash_->SetRotation({ 0,0,0 });
	slash.reset(slash_);
	command = WAIT;
}


void Honey::OnUpda() {
	XMFLOAT3 pos = obj->GetPosition();
		for (int j = 0; j < 6; j++) {
			missions[0][j]->Update();
			missions[0][j]->SetPosition({pos.x,pos.y+6.5f,pos.z});
			missions[1][j]->Update();
			missions[1][j]->SetPosition({ pos.x,pos.y + 3.5f,pos.z });

		}
	slash->Update();
	slash->SetPosition({ pos.x,pos.y + 5.0f,pos.z });

	switch (command) {
	case Actor::Phase::APPROCH:
		ApprochUpda();
		break;
	case Actor::Phase::LEAVE:
		LeaveUpda();
		break;
	case Actor::Phase::WAIT:
		WaitUpda();
		break;
	case Actor::Phase::DEAD:
		DeadUpda();
	default:
		break;
	}
}

void Honey::OnDraw(DirectXCommon* dxCommon) {
	Object2d::PreDraw();
	if (command == LEAVE || command == WAIT) {
		missions[0][stock]->Draw();
		missions[1][5]->Draw();
		slash->Draw();
	}
}

void Honey::OnFinal() {
}

void Honey::OnCollision(const std::string& Tag) {


}

void Honey::ApprochUpda() {


}

void Honey::LeaveUpda() {
	XMFLOAT3 pos = obj->GetPosition();
	if (frame > 1.0f) {
		frame = 1.0f;
		if (unload_frame>1.0f) {
			frame = 0.0f;
			unload_frame = 0.0f;
			command = APPROCH;
			before_pos = pos;
			stock = 0;
			old_stock = stock;
			ride_num = 0;
			for (int i = 0; i < 5; i++) {
				if (driver[i] != nullptr) {
					driver[i]->Navigation(first_pos);
					driver[i]->SetRotation({ 0,driver[i]->DirRotation(first_pos),0 });
					driver[i] = nullptr;
				}
			}
			return;
		} else {
			unload_frame += 0.01f;
		}
	} else {
		frame += 0.001f;
	}

	after_pos = { 0, 0 ,0 };
	pos.x = Ease(In, Linear, frame, before_pos.x, after_pos.x);
	pos.y = Ease(In, Linear, unload_frame, before_pos.y, after_pos.y);
	pos.z = Ease(In, Linear, frame, before_pos.z, after_pos.z);

	for (int i = 0; i < 5; i++) {
		if (driver[i] == nullptr) { continue; }
		driver[i]->SetPosition(	{ pos.x + (sinf(((i + 1) * 72) * XM_PI / 180) * 2.0f),0,pos.z + (cosf(((i + 1) * 72) * XM_PI / 180) * 2.0f) });
		driver[i]->SetRotation({0,driver[i]->DirRotation(after_pos),0});
	}
	obj->SetPosition(pos);
}

void Honey::WaitUpda() {
	XMFLOAT3 pos = obj->GetPosition();
	if (collide) {
		driver[ride_num] = ActorManager::GetInstance()->SetActionBullet(obj->GetPosition());
		if (driver[ride_num] != nullptr) {
			driver[ride_num]->SetsPlayActive(true);
			ride_num++;
			stock++;
		}
		collide = false;
	}
	
	for (int i = 0; i < 5;i++) {
		if (driver[i] == nullptr) { continue; }
		driver[i]->SetPosition({ pos.x+(sinf(((i+1)*72)*XM_PI/180)*2.0f),0,pos.z + (cosf(((i + 1) * 72) * XM_PI / 180) * 2.0f) });
	}

	if (stock >= 5&&!pause) {
		first_pos = pos;
		pos.y = 3.0f;
		before_pos = pos;
		command =LEAVE;
	}

	obj->SetPosition(pos);
}

void Honey::DeadUpda() {

	if (frame > 1.0f) {
		frame = 0.0f;
		obj->SetScale({3,3,3 });
		const float rnd_area = 80.0f;
		
		float posX= (float)rand() / RAND_MAX * rnd_area - rnd_area / 2.0f;
		float posZ= (float)rand() / RAND_MAX * rnd_area - rnd_area / 2.0f;
		obj->SetPosition({posX,0,posZ });
		command = WAIT;
		return;
	} else {
		frame += 0.0066f;
	}

	float scale = Ease(In,Quad,frame,3,0);
	obj->SetScale({ scale ,scale ,scale });

}

