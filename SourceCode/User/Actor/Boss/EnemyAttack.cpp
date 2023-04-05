#include"EnemyAttack.h"
#include"Collision.h"
#include"ImageManager.h"
#include<SourceCode/Common/Easing.h>
#include"ActorManager.h"
EnemyAttack::EnemyAttack(Actor* _enemy) {
}

EnemyAttack::~EnemyAttack() {
}

void EnemyAttack::Initialize() {
	Explo = Object2d::Create(ImageManager::Fire, { 0,0,0 },
		{ 1,1,1 }, { 1,1,1,1 });
	Explo->SetPosition({ 0,0.02f,0 });
	Explo->SetRotation({ 90,0,0 });

	for (int i = 0; i < PREDICTMAX;i++) {
		
		Predicted[i] = Object2d::Create(ImageManager::kpredicted, { 0,0,0 },
			{ 4,4,4 }, { 1,1,1,0.5f });
		Predicted[i]->SetPosition({ 0,0.02f,0 });
		Predicted[i]->SetRotation({ 90,0,0 });
	}

	partMan = new ParticleManager();
	partMan->Initialize(ImageManager::nul);

}

void EnemyAttack::Updata() {
	Explo->Update();
	for (int i = 0; i < PREDICTMAX; i++) {
		Predicted[i]->Update();
	}
	partMan->Update();
	if (burning) {
		XMFLOAT3 pos = Explo->GetPosition();
		const float rnd_vel = 5.0f;
		XMFLOAT3 vel{};
		vel.x = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.y = (float)rand() / RAND_MAX * 2.0f;
		vel.z = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		XMFLOAT3 acc{};
		acc.y = -(float)vel.y / 10.0f;
		partMan->Add(30, pos, vel, acc, 1.5, 0.0f, { 1.0f,1.0f,1.0f,1.0f }, { 1.0f,1.0f,1.0f,1.0f });
		scale = Ease(In, Linear, effectRate, 2.5f, 5.0f);
		ActorManager::GetInstance()->DamageBullet(pos, 15);
		Explo->SetScale({ scale,scale,scale });
		if (effectRate < 1.0f) {
			effectRate += 0.066f;
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
		for (int i = 0; i < PREDICTMAX; i++) {
			Predicted[i]->Draw();
		}
	}
}

void EnemyAttack::Stamp(const XMFLOAT3& pos) {
	if (!burning) {
		burning = true;
		effectRate = 0.0f;
		Explo->SetPosition({ pos.x,0,pos.z });
	}
}


void EnemyAttack::SetPredict(const bool& _predict, const float& _timer) {
	 this->predict = _predict;
	 
	 float sca = Ease(In,Linear,_timer,0, Predicted[PREDICTED]->GetScale().x);
	 Predicted[PREDICTING]->SetScale({sca,sca,sca});
}

