#include "ParticleEmitter.h"

ParticleEmitter::ParticleEmitter(const UINT& texnumber) {

	//パーティクルの初期化
	particleManager =std::make_unique<ParticleManager>();
	particleManager->Initialize(texnumber);




}

void ParticleEmitter::Add(const int& num, const int& life, const XMFLOAT3& position, const XMFLOAT3& velocity, const XMFLOAT3& accel, const float& start_scale, const float& end_scale, const XMFLOAT4& start_color, const XMFLOAT4& end_color) {









}

void ParticleEmitter::AddCommon(const int& life, const XMFLOAT3& position, const float& average_vel, const float& average_accel, const float& start_scale, const float& end_scale, const XMFLOAT4& start_color, const XMFLOAT4& end_color) {

	float rnd_vel = average_vel;
	XMFLOAT3 vel{};
	vel.x = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
	vel.y = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
	vel.z = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
	float rnd_acc = average_accel;
	XMFLOAT3 acc{};
	acc.x = (float)rand() / RAND_MAX * rnd_acc - rnd_acc / 2.0f;
	acc.y = (float)rand() / RAND_MAX * rnd_acc - rnd_acc / 2.0f;
	acc.z = (float)rand() / RAND_MAX * rnd_acc - rnd_acc / 2.0f;

	particleManager->Add(life, position, vel, acc, start_scale, end_scale, start_color, end_color);

}

void ParticleEmitter::Update() {
	particleManager->Update();
}

void ParticleEmitter::Draw(blendType type) {

	particleManager->Draw(type);
	
}