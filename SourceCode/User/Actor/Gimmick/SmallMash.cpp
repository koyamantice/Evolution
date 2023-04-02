#include "SmallMash.h"
#include <ActorManager.h>
#include <Easing.h>
#include <ModelManager.h>



void (SmallMash::* SmallMash::updateFuncTable[])() = {
	&SmallMash::RandSpawn,//要素0
	&SmallMash::WaitDriver,
	&SmallMash::ParasiteDriver,
	&SmallMash::LapseMash,
};

void SmallMash::OnInitialize() {

	obj->SetScale({1,1,1});
	collide_size = 1.0f;


	phase_ = Phase::kWaitDriver;
	

	//パーティクルの初期化
	particleEmitter_ = std::make_unique<ParticleEmitter>(ImageManager::smoke);

}


void SmallMash::OnUpdate() {
	particleEmitter_->Update();

	//関数ポインタで状態管理
	(this->*updateFuncTable[static_cast<size_t>(phase_)])();
}

void SmallMash::OnDraw(DirectXCommon* dxCommon) {
	Object2d::PreDraw();
	particleEmitter_->Draw();

}

void SmallMash::OnFinalize() {
}

void SmallMash::OnCollision(const std::string& Tag) {

}

void SmallMash::RandSpawn() {

}

void SmallMash::WaitDriver() {
}

void SmallMash::ParasiteDriver() {
}

void SmallMash::LapseMash() {
}


float SmallMash::RandHeight(const float& base) {
	const float rnd_vel = 0.05f * (stock + 1);
	float Rand = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
	float itr = 0;
	itr = Rand;

	return itr;
}