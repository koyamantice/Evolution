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
	Explo_->SetIsBillboard(true);
	Explo_->Object2dCreate();
	Explo_->SetRotation({ 0,0,0 });
	Explo.reset(Explo_);

	partMan = new ParticleManager();
	partMan->Initialize(ImageManager::nul);

}

void EnemyAttack::Upda() {
	Explo->Update();
	partMan->Update();
	if (burning) {
		XMFLOAT3 pos = Explo->GetPosition();
		const float rnd_vel = 3.0f;
		XMFLOAT3 vel{};
		vel.x = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.y = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.z = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		//const float rnd_sca = 2.0f;
		//float sca = (float)rand() / RAND_MAX * rnd_sca;
		partMan->Add(15, pos, vel, {0,-1.0f,0}, 1.5, 0.0f, { 1.0f,1.0f,1.0f,1.0f }, { 1.0f,1.0f,1.0f,1.0f });
		ActorManager::GetInstance()->DamageBullet(pos, 1.5f);
		if (effectRate < 1.0f) {
			effectRate += 0.08f;
		} else {
			effectRate = 0.0f;
			Explo->SetScale({ 0,0,0 });
			Explo->SetPosition({ 0,-100,0 });
			burning = false;
		}
	}
	scale = Ease(In, Quad, effectRate, 0.5f, 1.0f);
	Explo->SetScale({ scale,scale,scale });




}

void EnemyAttack::Draw() {
	if (burning) {
		Explo->Draw();
	}
	partMan->Draw(addBle);
}

bool EnemyAttack::Run() {


	return true;

}

void EnemyAttack::Stamp(XMFLOAT3 pos) {
	if (!burning) {
		burning = true;
		Explo->SetPosition({ pos.x,0,pos.z });
	}
}

