#include "Honey.h"
#include <ActorManager.h>
#include <Easing.h>
#include <ModelManager.h>





void (Honey::* Honey::updateFuncTable[])() = {
	&Honey::WaitDriver,//要素0
	&Honey::DispersionDriver,
	&Honey::SetupHoney,
	&Honey::InviteBee,
	&Honey::EatenHoney,
	&Honey::RandSpawn,

};

void Honey::OnInitialize() {

	obj->SetScale(base_sca);

	honey_obj_ = std::make_unique<Object3d>();
	honey_obj_->SetModel(ModelManager::GetInstance()->GetModel(ModelManager::kHoney));
	honey_obj_->SetScale({});
	honey_obj_->Initialize();

	phase_ = E_Phase::kWaitDriver;
	
	collide_size = 9.0f;

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


void Honey::OnUpdate() {
	missionUpdate();
	questionUpdate();
	particleEmitter->Update();

	//関数ポインタで状態管理
	(this->*updateFuncTable[static_cast<size_t>(phase_)])();
}

void Honey::OnDraw(DirectXCommon* dxCommon) {
	Object3d::PreDraw();
	honey_obj_->Draw();
	Object2d::PreDraw();
	if (phase_ == E_Phase::kWaitDriver) {
		missions[0][stock]->Draw();
		missions[1][5]->Draw();
		slash->Draw();
	}
	particleEmitter->Draw();

}

void Honey::OnFinalize() {
}

void Honey::ResultOnUpdate(const float& Timer) {

	obj->Update();

	honey_obj_->Update();

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
	if (phase_!=E_Phase::kSetupHoney) {
		XMFLOAT3 pos = obj->GetPosition();
		honey_obj_->SetPosition(pos);
	}
	honey_obj_->Update();
}

void Honey::WaitDriver() {
	XMFLOAT3 pos = obj->GetPosition();
	if (scale_damaged_frame_ >= 1.0f) {
		first_pos = pos;
		before_pos = pos;
		collide_size = 6.0f;
		phase_ = E_Phase::kSetupHoney;
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
			collide_size = max(collide_size, 3.0f);
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

void Honey::SetupHoney() {
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

		XMFLOAT3 sca_honey = honey_obj_->GetScale();
		sca_honey.x = Ease(Out, Quad, frame, 0, base_sca.x);
		sca_honey.y = Ease(Out, Quad, frame, 0, base_sca.y);
		sca_honey.z = Ease(Out, Quad, frame, 0, base_sca.z);
		honey_obj_->SetScale(sca_honey);
		honey_obj_->SetPosition({ obj->GetPosition().x, height ,obj->GetPosition().z });
	}
}

void Honey::DispersionDriver() {
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

void Honey::InviteBee() {
	if (canMove) {
		phase_ = E_Phase::kEatenHoney;
	}
}

void Honey::EatenHoney() {
	if (frame > 1.0f) {
		frame = 0.0f;
		leave_timer_ = 0;
		stock = 0;
		ride_num = 0;
		scale_damaged_frame_ = 0.0f;
		honey_obj_->SetScale({ 0,0,0 });
		obj->SetScale(base_sca);
		const float rnd_area = 80.0f;
		std::mt19937 mt{ std::random_device{}() };
		std::uniform_real_distribution<float> dist(0.0, 1.0);
		float posX = dist(mt) * rnd_area - rnd_area / 2.0f;
		float posZ = dist(mt) * rnd_area - rnd_area / 2.0f;
		obj->SetPosition({ posX,0.0f,posZ });
		honey_obj_->SetPosition(obj->GetPosition());
		phase_ = E_Phase::kRandSpawn;
		return;
	} else {
		frame += 0.0016f;
	}

	float scale = Ease(In, Quad, frame, base_sca.x, 0);
	honey_obj_->SetScale({ scale ,scale ,scale });
}

void Honey::RandSpawn() {
	XMFLOAT3 pos = obj->GetPosition();
	if (pos.y < 0.0f) {
		pos.y += 0.1f;
	} else {
		phase_ = E_Phase::kWaitDriver;

	}
}

void Honey::IntroOnUpdate(const float& timer) {
	//関数が渡す最初の値
	const float start_timer = 0.5f;
	//
	XMFLOAT3 pos = obj->GetPosition();
	//
	float time = timer - start_timer;
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
	std::mt19937 mt{ std::random_device{}() };
	std::uniform_real_distribution<float> dist(0.0, 1.0);
	float Rand = dist(mt)* rnd_vel - rnd_vel / 2.0f;
	float itr = 0;
	itr = Rand;

	return itr;
}