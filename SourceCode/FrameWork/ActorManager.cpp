#include"ActorManager.h"
#include <algorithm>
#include <SourceCode/FrameWork/collision/Collision.h>

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
	RemoveActor();
	CheckAllCollisions();
}
void ActorManager::DemoUpdate() {
	for (std::unique_ptr<Actor>& actor : Actors) {
		actor->Demo();
	}
}
void ActorManager::Draw() {
	for (auto itrA = Actors.rbegin(); itrA != Actors.rend(); ++itrA) {
		Actor* ActorA = itrA->get();
		ActorA->Draw();
	}
}
void ActorManager::DemoDraw() {
	for (std::unique_ptr<Actor>& actor : Actors) {
		actor->DemoDraw();
	}
}
void ActorManager::Finalize() {
	for (std::unique_ptr<Actor>& actor : Actors) {
		actor->Finalize();
	}
	Actors.clear();
}

void ActorManager::CheckAllCollisions() {
	for (auto itrA = Actors.begin(); itrA != Actors.end(); ++itrA) {
		for (auto itrB = Actors.rbegin(); itrB != Actors.rend(); ++itrB) {
			Actor* actorA = itrA->get();
			Actor* actorB = itrB->get();
			if (Collision::SphereCollision2(actorA->GetPosition(), actorA->GetSize(), actorB->GetPosition(), actorB->GetSize())) {
				if (actorA->GetID()!= actorB->GetID()) {
					actorA->OnCollision(actorB->GetTag());
					actorB->OnCollision(actorA->GetTag());
				}
			}
		}
	}
}

void ActorManager::AttachActor(const std::string& ActorName, ActorComponent* newActorCompornent) {
	assert(actorFactory_);
	
	std::unique_ptr<Actor> newActor;
	
	newActor.reset(actorFactory_->CreateActor(ActorName,newActorCompornent));

	Actors.push_back(std::move(newActor));
}

void ActorManager::RemoveActor() {
	Actors.remove_if([](std::unique_ptr<Actor>& actor){
		return actor->GetIsRemove();
		});
}

const int& ActorManager::SearchNum(const std::string& tag) {
	num = 0;
	for (auto itr = Actors.begin(); itr != Actors.end(); ++itr) {
		Actor* actor = itr->get();
		if (actor->GetTag() == tag) {
			num++;
		}
	}
	return num;
}

Actor* ActorManager::CommandActor(const int& ID) {
	for (auto itr = Actors.begin(); itr != Actors.end(); ++itr) {
		Actor* actor = itr->get();
		if (actor->GetTag() == "Enemy") {
			if (actor->GetID() == ID) {
				return actor;
			}
		}
	}
	return nullptr;
}

Actor* ActorManager::SearchWaitBullet() {
	for (auto itr = Actors.begin(); itr != Actors.end(); ++itr) {
	Actor* actor = itr->get();
		if (actor->GetTag() !="Bullet") { continue; }
		if (actor->GetCommand() == Actor::command::Attack) { continue; }
		return actor;
	}
	return nullptr;
}

Actor* ActorManager::SearchActor(const std::string& tag) {

	for (auto itr = Actors.begin(); itr != Actors.end(); ++itr) {
		Actor* actor = itr->get();
		if (actor->GetTag()== tag) {
			return actor;
		}
	}
	return nullptr;
}
Actor* ActorManager::SearchActorBack(const std::string& tag) {
	for (auto itr = Actors.rbegin(); itr != Actors.rend(); ++itr) {
		Actor* actor = itr->get();
		if (actor->GetTag() == tag) {
			return actor;
		}
	}
	return nullptr;
}