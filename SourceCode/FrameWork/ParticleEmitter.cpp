#include "ParticleEmitter.h"

#define DEGREE_MAX 360.0f

ParticleEmitter::ParticleEmitter(const UINT& texnumber) {

	//パーティクルの初期化
	particleManager = std::make_unique<ParticleManager>();
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

void ParticleEmitter::AddInNest(const int& life, const XMFLOAT3& position, const float& average_margin, const float& average_vel, const float& start_scale, const float& end_scale, const XMFLOAT4& start_color, const XMFLOAT4& end_color) {

	XMFLOAT3 margin{};
	margin.x = (float)rand() / RAND_MAX * average_margin - average_margin / 2.0f;
	margin.y = (float)rand() / RAND_MAX * average_margin - average_margin / 2.0f;
	margin.z = (float)rand() / RAND_MAX * average_margin - average_margin / 2.0f;

	XMFLOAT3 vel{};
	vel.y = (float)rand() / RAND_MAX * average_vel;

	particleManager->Add(life, { position.x + margin.x, position.y + 2.5f + margin.y,position.z + margin.z }, vel, {}, start_scale, end_scale, start_color, end_color);

}

void ParticleEmitter::AddContraction(const int& _life, const XMFLOAT3& _position, const float& _area, const float& _average_vel, const float& _start_scale, const float& _end_scale, const XMFLOAT4& _start_color, const XMFLOAT4& _end_color) {

	float angle = (float)rand() / RAND_MAX * DEGREE_MAX;

	XMFLOAT3 pos{};
	pos.x = _position.x + _area * sinf(angle);
	pos.z = _position.z + _area * cosf(angle);
	XMFLOAT3 vel{};
	vel.y = (float)rand() / RAND_MAX * _average_vel;
	particleManager->Add(45, pos, vel, {}, 1.5f, 0.0f, { 0.5f,1.0f,0.5f,1.0f }, { 0.5f,1.0f,0.5f,1.0f });


}

void ParticleEmitter::AddCrystalMove(const int& _life, const XMFLOAT3& _position, const float& _area, const float& _average_vel, const float& _start_scale, const float& _end_scale, const XMFLOAT4& _start_color, const XMFLOAT4& _end_color) {
	float angle = (float)rand() / RAND_MAX * DEGREE_MAX;

	float area = (float)rand() / RAND_MAX * _area;

	XMFLOAT3 pos{};
	pos.x = _position.x + area * sinf(angle);
	pos.z = _position.z + area * cosf(angle);

	XMFLOAT3 vel{};
	vel.y = (float)rand() / RAND_MAX * _average_vel;

	particleManager->Add(_life, pos, vel, {}, _start_scale, _end_scale, _start_color, _end_color);

}

void ParticleEmitter::AddParabo(const int& _life, const XMFLOAT3& _position, const float& _downHeight, const float& _average_vel, const float& _start_scale, const float& _end_scale, const XMFLOAT4& _start_color, const XMFLOAT4& _end_color) {

	const float rnd_pos = 8.0f;
	XMFLOAT3 pos{};
	pos.x = _position.x + (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;
	pos.y = _position.y;
	pos.z = _position.z + (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;
	XMFLOAT3 vel{};
	vel.x = (float)rand() / RAND_MAX * _average_vel - _average_vel / 2.0f;
	vel.y = _downHeight;
	vel.z = (float)rand() / RAND_MAX * _average_vel - _average_vel / 2.0f;
	XMFLOAT3 acc{};
	acc.y = -(float)vel.y / (_life / 2.0f);
	particleManager->Add(_life, pos, vel, acc, _start_scale, _end_scale, _start_color, _end_color);

}


void ParticleEmitter::Update() {
	particleManager->Update();
}

void ParticleEmitter::Draw(blendType type) {

	particleManager->Draw(type);

}