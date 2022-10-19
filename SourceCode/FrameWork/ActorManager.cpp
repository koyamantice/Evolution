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
	CheckAllCollisions();
	RemoveActor();
}
void ActorManager::DemoUpdate() {
	for (std::unique_ptr<Actor>& actor : Actors) {
		actor->Demo();
	}
}
void ActorManager::Draw(DirectXCommon* dxCommon) {
	for (auto itrA = Actors.rbegin(); itrA != Actors.rend(); ++itrA) {
		Actor* ActorA = itrA->get();
		ActorA->Draw(dxCommon);
	}
}
void ActorManager::DemoDraw(DirectXCommon* dxCommon) {
	for (std::unique_ptr<Actor>& actor : Actors) {
		actor->DemoDraw(dxCommon);
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
			if (actorA->GetTag() == "Crystal" && actorB->GetTag() == "Player") {
				if (Collision::CircleCollision(actorA->GetPosition().x, actorA->GetPosition().z, 1.5f, actorB->GetPosition().x, actorB->GetPosition().z, 1.5f)) {
						actorA->OnCollision(actorB->GetTag());
						actorB->OnCollision(actorA->GetTag());
				}
				continue;
			}
			if (Collision::SphereCollision2(actorA->GetPosition(), 1.5f, actorB->GetPosition(), 1.5f)) {
				if (actorA->GetTag()!= actorB->GetTag()) {
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

void ActorManager::DamageBullet(XMFLOAT3 pos,float radius) {
	for (auto itr = Actors.begin(); itr != Actors.end(); ++itr) {
		Actor* actor = itr->get();
		if (actor->GetTag() != "Bullet") { continue; }
		if (actor->GetCommand() == Actor::command::Wait) { continue; }
		XMFLOAT3 itrPos = actor->GetPosition();
		if (itrPos.y > 0.1f) { continue; }
		if(Collision::CircleCollision(pos.x,pos.z,1.0f,itrPos.x,itrPos.z,radius)){
			actor->SetDeadFlag(true);
		}
	}
}

XMFLOAT3 ActorManager::Dist(XMFLOAT3 pos, XMFLOAT3 pos2) {
	XMFLOAT3 itr;
	itr.x = sqrtf(powf((pos2.x - pos.x),2));
	itr.y = sqrtf(powf((pos2.y - pos.y),2));
	itr.z = sqrtf(powf((pos2.z - pos.z),2));
	return itr;
}

float ActorManager::Length(XMFLOAT3 pos, XMFLOAT3 pos2) {
	float itr;

	itr = sqrtf(powf((pos2.x - pos.x), 2)+powf((pos2.y - pos.y), 2)+powf((pos2.z - pos.z), 2));

	return itr;
}

void ActorManager::ChangeBulletCommand(XMFLOAT3 pos, float scale) {
	for (auto itr = Actors.begin(); itr != Actors.end(); ++itr) {
		Actor* actor = itr->get();
		if (actor->GetTag() != "Bullet") { continue; }
		if(Collision::CircleCollision(pos.x, pos.z, scale, actor->GetPosition().x, actor->GetPosition().z, 1.0f)) {

			actor->SetCommand(Actor::command::Wait);
		}
	}
}

Actor* ActorManager::SearchActorArea(XMFLOAT3 pos) {
	Actor* itrActor=SearchActor("Player");
	const float limit = 30.0f;
	float check = limit;
	for (auto itr = Actors.begin(); itr != Actors.end(); ++itr) {
		Actor* actor = itr->get();
		if (actor->GetTag() == "Player"||actor->GetTag() == "Bullet") { continue; }

		XMFLOAT3 difPos = actor->GetPosition();
		float dif;
		dif = Length(difPos, pos);
		if (check > dif) {
			check = dif;
			itrActor = itr->get();
		}
	}
	return itrActor;
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

Actor* ActorManager::SearchID(int ID) {
	if (ID < 0) { return nullptr; }
	for (auto itr = Actors.begin(); itr != Actors.end(); ++itr) {
		Actor* actor = itr->get();
		if (actor->GetTag() != "Bullet") { continue; }
		if (actor->GetID() == ID) {
			return actor;
		}
	}
	return nullptr;
}

