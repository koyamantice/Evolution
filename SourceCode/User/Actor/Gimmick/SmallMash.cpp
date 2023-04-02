#include "SmallMash.h"
#include <ActorManager.h>
#include <Easing.h>
#include <ModelManager.h>





void (SmallMash::* SmallMash::updateFuncTable[])() = {
	&SmallMash::WaitDriver,//要素0
	&SmallMash::DispersionDriver,
	&SmallMash::SetupSmallMash,
	&SmallMash::InviteBee,
	&SmallMash::EatenSmallMash,
	&SmallMash::RandSpawn,

};

void SmallMash::OnInitialize() {

	obj->SetScale(base_sca);


	phase_ = E_Phase::kWaitDriver;
	
	collide_size = 6.0f;

	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 6; j++) {
			missions[i][j] = Object2d::Create(ImageManager::kMission_0 + j, { 0,0,0 },
				{ 0.2f,0.2f,0.2f }, { 1,1,1,1 });
			missions[i][j]->SetIsBillboard(true);
			missions[i][j]->SetRotation({ 0,0,0 });
		}
	}

	slash = Object2d::Create(ImageManager::kMission_s, { 0,0,0 },
		{ 0.2f,0.2f,0.2f }, { 1,1,1,1 });
	slash->SetIsBillboard(true);
	slash->SetRotation({ 0,0,0 });

	//パーティクルの初期化
	particleEmitter = std::make_unique<ParticleEmitter>(ImageManager::smoke);

}


void SmallMash::OnUpdate() {
	missionUpdate();
	questionUpdate();
	particleEmitter->Update();

	//関数ポインタで状態管理
	(this->*updateFuncTable[static_cast<size_t>(phase_)])();
}

void SmallMash::OnDraw(DirectXCommon* dxCommon) {
	Object2d::PreDraw();
	particleEmitter->Draw();

}

void SmallMash::OnFinalize() {
}

void SmallMash::OnCollision(const std::string& Tag) {


}

void SmallMash::missionUpdate() {
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

void SmallMash::questionUpdate() {
}

void SmallMash::WaitDriver() {
	XMFLOAT3 pos = obj->GetPosition();
	if (scale_damaged_frame_ >= 1.0f) {
		first_pos = pos;
		before_pos = pos;
		collide_size = 6.0f;
		phase_ = E_Phase::kSetupSmallMash;
	}
	if (driver[0] != nullptr) {
		if (stock < 5) {
			scale_damaged_frame_ += scale_damage_;
		} else {
			scale_damaged_frame_ += scale_damage_ * 10.0f;
		}
		if (scale_damaged_frame_ <= 1.0f) {
			XMFLOAT3 sca = obj->GetScale();
			sca.x = Ease(InOut, Circ, scale_damaged_frame_, base_sca.x, 0.0f);
			sca.y = Ease(InOut, Circ, scale_damaged_frame_, base_sca.y, 0.0f);
			sca.z = Ease(InOut, Circ, scale_damaged_frame_, base_sca.z, 0.0f);
			obj->SetScale(sca);
			collide_size = sca.x;
			collide_size = max(collide_size, 1.0f);
		}
		obj->SetPosition({ pos.x + RandHeight(pos.x),pos.y,pos.z + RandHeight(pos.z) });

		//パーティクル処理
		const float rnd_vel = 0.3f;
		const float rnd_height = 0.3f;
		//使用者が多いたび増える
		particle_draw_num_ += 1 * ride_num;
		if (particle_draw_num_ > kDrawTiming) {
			particleEmitter->AddParabo(60, pos, rnd_height, rnd_vel, 1.5f, 0.0f, { 1.0f,1.0f,0.0f,0.8f }, { 1,1,0,0 });
			particle_draw_num_ = 0;
		}
	}
	if (collide) {//(hp/maxhp)*scale
		if (stock > 5 || ride_num > 4) { return; }
		driver[ride_num] = ActorManager::GetInstance()->SetActionBullet(obj->GetPosition(), collide_size);
		if (driver[ride_num] != nullptr) {
			driver[ride_num]->SetsPlayActive(true);
			ride_num++;
			stock++;
		}
		collide = false;
	}

}

void SmallMash::SetupSmallMash() {
	leave_timer_++;
	if (leave_timer_ > kLeaveTimeMax) {
		if (frame > 1.0f) {
			if (pause) { return; }
			frame = 0.0f;
			phase_ = E_Phase::kDispersionDriver;
			leave_timer_ = 0;
			return;
		} else {
			frame += 1 / 45.0f;
		}
		float height = 0.0f;
		if (frame < 0.5f) {
			height = Ease(In, Quad, frame, 0, 10.0f);
		} else {
			height = Ease(Out, Quad, frame, 10.0, 0.0f);
		}
	}
}

void SmallMash::DispersionDriver() {
	for (int i = 0; i < ride_num; i++) {
		if (driver[i] != nullptr) {
			driver[i]->SetsPlayActive(false);
			driver[i]->SetCommand(Bullet::BulletStatus::Wait);
			driver[i] = nullptr;
		}
	}
	ride_num = 0;
	stock = 0;
	canMove = false;
	phase_ = E_Phase::kInviteBee;
}

void SmallMash::InviteBee() {
	if (canMove) {
		phase_ = E_Phase::kEatenSmallMash;
	}
}

void SmallMash::EatenSmallMash() {
	if (frame > 1.0f) {
		frame = 0.0f;
		leave_timer_ = 0;
		stock = 0;
		ride_num = 0;
		scale_damaged_frame_ = 0.0f;
		obj->SetScale(base_sca);
		const float rnd_area = 80.0f;
		float posX = (float)rand() / RAND_MAX * rnd_area - rnd_area / 2.0f;
		float posZ = (float)rand() / RAND_MAX * rnd_area - rnd_area / 2.0f;
		obj->SetPosition({ posX,0.0f,posZ });
		phase_ = E_Phase::kRandSpawn;
		return;
	} else {
		frame += 0.0016f;
	}

	float scale = Ease(In, Quad, frame, base_sca.x, 0);
}

void SmallMash::RandSpawn() {
	XMFLOAT3 pos = obj->GetPosition();
	if (pos.y < 0.0f) {
		pos.y += 0.1f;
	} else {
		phase_ = E_Phase::kWaitDriver;

	}
}


float SmallMash::RandHeight(const float& base) {
	const float rnd_vel = 0.05f * (stock + 1);
	float Rand = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
	float itr = 0;
	itr = Rand;

	return itr;
}