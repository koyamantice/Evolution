#include"ActorManager.h"

ActorManager* ActorManager::GetInstance() {
	static ActorManager instance;
	return &instance;
}

void ActorManager::Initialize() {

	ActorFactory* newFactory = new ActorFactory();
	actorFactory_ = newFactory;

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

void ActorManager::AttachActor(const std::string& ActorName) {
	assert(actorFactory_);
	
	std::unique_ptr<Actor> newActor;
	
	newActor.reset(actorFactory_->CreateActor(ActorName));

	Actors.push_back(std::move(newActor));
}

Actor* ActorManager::SearchActor(const std::string& tag) {

	for (auto itr = Actors.begin(); itr != Actors.end(); ++itr) {
		Actor* actor = itr->get();
		if (actor->GetTag()==tag) {
			return actor;
		}
	}
	return nullptr;
}
