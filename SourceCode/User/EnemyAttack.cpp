#include"EnemyAttack.h"
#include"Collision.h"
#include"ImageManager.h"
#include<SourceCode/Common/Easing.h>
#include"ActorManager.h"
EnemyAttack::EnemyAttack() {
}

EnemyAttack::~EnemyAttack() {
}

void EnemyAttack::Init() {
	Object2d* Explo_ = Object2d::Create(ImageManager::Fire, {0,0,0},
	{1,1,1}, { 1,1,1,1 });
	//Explo_->SetIsBillboard(true);
	Explo_->Object2dCreate();
	Explo_->SetPosition({ 0,0.02f,0 });
	Explo_->SetRotation({ 90,0,0 });
	Explo.reset(Explo_);

	Object2d* Predicted_ = Object2d::Create(ImageManager::kpredicted, { 0,0,0 },
		{ 4,4,4 }, { 1,1,1,0.5f});
	//Predicted_->SetIsBillboard(true);
	Predicted_->Object2dCreate();
	Predicted_->SetPosition({ 0,0.02f,0 });
	Predicted_->SetRotation({ 90,0,0 });
	Predicted.reset(Predicted_);


	partMan = new ParticleManager();
	partMan->Initialize(ImageManager::nul);

}

void EnemyAttack::Upda() {
	Explo->Update();
	Predicted->Update();
	partMan->Update();
	if (burning) {
		XMFLOAT3 pos = Explo->GetPosition();
		const float rnd_vel = 5.0f;
		XMFLOAT3 vel{};
		vel.x = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.y = (float)rand() / RAND_MAX * 2.0f;// -rnd_vel / 2.0f;
		vel.z = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		XMFLOAT3 acc{};
		//acc.x = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		acc.y = -(float)vel.y / 10.0f;
		//acc.z = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		partMan->Add(30, pos, vel, acc, 1.5, 0.0f, { 1.0f,1.0f,1.0f,1.0f }, { 1.0f,1.0f,1.0f,1.0f });
		scale = Ease(In, Linear, effectRate, 2.5f, 5.0f);
		ActorManager::GetInstance()->DamageBullet(pos, 10);
		Explo->SetScale({ scale,scale,scale });
		if (effectRate < 1.0f) {
			effectRate += 0.04f;
		} else {
			effectRate = 1.0f;
			Explo->SetScale({ 0,0,0 });
			Explo->SetPosition({ 0,-100,0 });
			burning = false;
		}
	}
}

void EnemyAttack::Draw() {
	if (burning) {
		Object2d::PreDraw();
		Explo->Draw();
		partMan->Draw(addBle);
	}
	if (predict) {
		Object2d::PreDraw();
		Predicted->Draw();
	}
}

bool EnemyAttack::Run() {

	return true;

}

void EnemyAttack::Stamp(XMFLOAT3 pos) {
	if (!burning) {
		burning = true;
		effectRate = 0.0f;
		Explo->SetPosition({ pos.x,0,pos.z });
	}
}

