#include"ActorManager.h"

ActorManager* ActorManager::GetInstance() {
	static ActorManager instance;
	return &instance;
}

void ActorManager::Update() {
	for (std::unique_ptr<Actor>& actor : Actors) {
		actor->Update();
	}
}

void ActorManager::Draw() {
	for (std::unique_ptr<Actor>& actor : Actors) {
		actor->Draw();
	}
}

void ActorManager::Finalize() {
	for (std::unique_ptr<Actor>& actor : Actors) {
		actor->Finalize();
	}
	Actors.clear();
}
