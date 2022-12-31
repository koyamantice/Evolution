#include "ParticleEmitter.h"

ParticleEmitter::ParticleEmitter(ParticleManager* particleMan) {
	particleMan_ = particleMan;
}
void ParticleEmitter::Add(const int& count, const int& life, const XMFLOAT3& position) {
}

void ParticleEmitter::Update() {
	if (particleMan_ != nullptr) {
		particleMan_->Update();
	}
}

void ParticleEmitter::Draw() {
	//if (particleMan_ != nullptr) {
	//	particleMan_->Draw(cmdList);
	//}
}