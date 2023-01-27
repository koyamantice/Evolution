#include "Honey.h"
#include <ActorManager.h>
#include <Easing.h>
#include <ModelManager.h>

void Honey::OnInit() {
	obj->SetScale(base_sca);
	//必要人数
	stock = 0;
	collide_size = 6.0f;

	honey_obj_ = std::make_unique<Object3d>();
	honey_obj_->SetModel(ModelManager::GetIns()->GetModel(ModelManager::kHoney));
	honey_obj_->Initialize();

	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 6; j++) {
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
	missionUpdate();
	questionUpdate();

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
		break;
	case Actor::Phase::UNGUARD:
		IntroOnUpdate(0);
		break;
	default:
		break;
	}
}

void Honey::OnDraw(DirectXCommon* dxCommon) {
	Object3d::PreDraw();
	honey_obj_->Draw();
	Object2d::PreDraw();
	if (obj->GetPosition().y >= -0.01f) {
		if (command == LEAVE || command == WAIT) {
			if (stock <= 5) {
				missions[0][stock]->Draw();
			}
			missions[1][5]->Draw();
			slash->Draw();
		}
	}
}

void Honey::OnFinal() {
}

void Honey::OnCollision(const std::string& Tag) {


}

void Honey::missionUpdate() {
	XMFLOAT3 pos = obj->GetPosition();
	for (int i = 0; i < 6; i++) {
		missions[0][i]->Update();
		missions[0][i]->SetPosition({ pos.x,pos.y + 6.5f,pos.z });
		missions[1][i]->Update();
		missions[1][i]->SetPosition({ pos.x,pos.y + 3.5f,pos.z });
	}
	slash->Update();
	slash->SetPosition({ pos.x,pos.y + 5.0f,pos.z });
}

void Honey::questionUpdate() {
	XMFLOAT3 pos = obj->GetPosition();
	honey_obj_->SetPosition(pos);
	honey_obj_->Update();
}

void Honey::ApprochUpda() {
	for (int i = 0; i < ride_num; i++) {
		if (driver[i] != nullptr) {
			driver[i]->SetsPlayActive(false);
			driver[i]->SetCommand(Bullet::command::Wait);
			driver[i] = nullptr;
		}
	}
	ride_num = 0;
}

void Honey::LeaveUpda() {
	leave_timer_++;
	if (leave_timer_ > kLeaveTimeMax) {
		if (frame > 1.0f) {
			frame = 0.0f;
			command = APPROCH;
			leave_timer_ = 0;
			return;
		} else {
			frame += 0.055f;
		}

		XMFLOAT3 sca = obj->GetScale();
		sca.x = Ease(In, Linear, frame, base_sca.x, 0);
		sca.y = Ease(In, Linear, frame, base_sca.y, 0);
		sca.z = Ease(In, Linear, frame, base_sca.z, 0);
		obj->SetScale(sca);

		XMFLOAT3 sca_honey = honey_obj_->GetScale();
		sca_honey.x = Ease(In, Linear, frame, 0, base_sca.x);
		sca_honey.y = Ease(In, Linear, frame, 0, base_sca.y);
		sca_honey.z = Ease(In, Linear, frame, 0, base_sca.z);
		honey_obj_->SetScale(sca_honey);
	}
}

void Honey::WaitUpda() {
	XMFLOAT3 pos = obj->GetPosition();
	if (pos.y < 0.0f) {
		pos.y += 0.1f;
	} else {
		if (collide) {
			if (stock > 5) { return; }
			if (ride_num > 4) { return; }
			driver[ride_num] = ActorManager::GetInstance()->SetActionBullet(obj->GetPosition(), collide_size);
			if (driver[ride_num] != nullptr) {
				driver[ride_num]->SetsPlayActive(true);
				ride_num++;
				stock++;
			}
			collide = false;
		}
		if (stock == 5 && !pause) {
			stock = 5;
			first_pos = pos;
			before_pos = pos;
			command = LEAVE;
		}
	}
	if (driver[0] != nullptr) {
		obj->SetPosition({ pos.x + RandHeight(pos.x),pos.y,pos.z + RandHeight(pos.z) });
	}

}

void Honey::DeadUpda() {

	if (frame > 1.0f) {
		frame = 0.0f;
		leave_timer_ = 0;
		stock = 0;
		honey_obj_->SetScale({ 0,0,0 });
		obj->SetScale(base_sca);
		const float rnd_area = 80.0f;
		float posX = (float)rand() / RAND_MAX * rnd_area - rnd_area / 2.0f;
		float posZ = (float)rand() / RAND_MAX * rnd_area - rnd_area / 2.0f;
		obj->SetPosition({ posX,0.0f,posZ });
		honey_obj_->SetPosition(obj->GetPosition());
		command = WAIT;
		return;
	} else {
		frame += 0.0016f;
	}

	float scale = Ease(In, Quad, frame, base_sca.x, 0);
	honey_obj_->SetScale({ scale ,scale ,scale });
}

void Honey::IntroOnUpdate(const float& Timer) {
	//関数が渡す最初の値
	const float start_timer = 0.5f;
	//
	XMFLOAT3 pos = obj->GetPosition();
	//
	float time = Timer - start_timer;
	if (time <= 0.1f) {
		pos.x = Ease(In, Linear, time / 0.1f, 15, 15);
		pos.y = Ease(In, Linear, time / 0.1f, 15, 0);
		pos.z = Ease(In, Linear, time / 0.1f, 10, 10);
	} else {
		int a = 0;
		a++;

	}
}

float Honey::RandHeight(const float& base) {
	const float rnd_vel = 0.05f * (stock + 1);
	float Rand = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
	float itr = 0;
	itr = Rand;

	return itr;
}