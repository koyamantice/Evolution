#include"ActorManager.h"
#include <algorithm>
#include <SourceCode/FrameWork/collision/Collision.h>
#include "NullActor.h"

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
	for (std::unique_ptr<Bullet>& bullet : Bullets) {
		bullet->Update();
	}
	CheckActorCollisions();
	CheckBulletCollisions();
	RemoveActor();
}
void ActorManager::DemoUpdate() {
	for (std::unique_ptr<Actor>& actor : Actors) {
		actor->Demo();
	}
}
void ActorManager::IntroUpdate(const float& timer, const std::string& voidname, const int& _stage) {
	if (voidname == "Bullet") {
		for (std::unique_ptr<Actor>& actor : Actors) {
			actor->IntroUpdate(timer);
		}
		return;
	} else if (voidname == "Actor") {
		for (std::unique_ptr<Bullet>& bullet : Bullets) {
			bullet->IntroUpdate(timer, _stage);
		}
		return;
	} else {
		for (std::unique_ptr<Actor>& actor : Actors) {
			actor->IntroUpdate(timer);
		}
		for (std::unique_ptr<Bullet>& bullet : Bullets) {
			bullet->IntroUpdate(timer, _stage);
		}
	}
}
void ActorManager::ResultUpdate(const float& timer) {
	for (std::unique_ptr<Actor>& actor : Actors) {
		actor->ResultUpdate(timer);
	}
	for (std::unique_ptr<Bullet>& bullet : Bullets) {
		bullet->ResultUpdate(timer);
	}
	RemoveActor();
}
void ActorManager::Draw(DirectXCommon* dxCommon) {
	for (auto itr = Bullets.rbegin(); itr != Bullets.rend(); ++itr) {
		Bullet* bullet = itr->get();
		bullet->FirstDraw(dxCommon);
	}
	for (auto itr = Actors.rbegin(); itr != Actors.rend(); ++itr) {
		Actor* Actor = itr->get();
		Actor->FirstDraw(dxCommon);
	}

	for (auto itr = Bullets.rbegin(); itr != Bullets.rend(); ++itr) {
		Bullet* bullet = itr->get();
		bullet->Draw(dxCommon);
	}
	for (auto itr = Actors.rbegin(); itr != Actors.rend(); ++itr) {
		Actor* Actor = itr->get();
		Actor->Draw(dxCommon);
	}
	for (auto itr = Bullets.rbegin(); itr != Bullets.rend(); ++itr) {
		Bullet* bullet = itr->get();
		bullet->LastDraw(dxCommon);
	}
	for (auto itr = Actors.rbegin(); itr != Actors.rend(); ++itr) {
		Actor* Actor = itr->get();
		Actor->LastDraw(dxCommon);
	}

}
void ActorManager::DemoDraw(DirectXCommon* dxCommon) {
	for (std::unique_ptr<Actor>& actor : Actors) {
		actor->DemoDraw(dxCommon);
	}
}
void ActorManager::Finalize() {
	for (std::unique_ptr<Bullet>& bullet : Bullets) {
		bullet->Finalize();
	}
	Bullets.clear();
	for (std::unique_ptr<Actor>& actor : Actors) {
		actor->Finalize();
	}
	Actors.clear();
}

void ActorManager::CheckActorCollisions() {
	for (auto itrA = Actors.begin(); itrA != Actors.end(); ++itrA) {
		for (auto itrB = Actors.rbegin(); itrB != Actors.rend(); ++itrB) {
			Actor* actorA = itrA->get();
			Actor* actorB = itrB->get();
			if (Collision::SphereCollision2(actorA->GetPosition(), actorA->GetSize(), actorB->GetPosition(), actorB->GetSize())) {
				if (!actorA->GetIsActive() || !actorB->GetIsActive()) { continue; }
				if (actorA->GetTag() != actorB->GetTag()) {
					actorA->OnCollision(actorB->GetTag());
					actorB->OnCollision(actorA->GetTag());
				}
			}
		}
	}
}

void ActorManager::CheckBulletCollisions() {
	for (auto itrA = Actors.begin(); itrA != Actors.end(); ++itrA) {
		for (auto itrB = Bullets.begin(); itrB != Bullets.end(); ++itrB) {
			Actor* actor = itrA->get();
			Bullet* bullet = itrB->get();
			if (Collision::SphereCollision2(actor->GetPosition(), actor->GetSize(), bullet->GetPosition(), 2.0f)) {
				if (actor->GetIsUnrivaled()) { continue; }

				actor->OnCollision("Bullet");
				bullet->OnCollision(actor->GetTag(), actor->GetPosition());
			}
		}
	}
	for (auto itrA = Bullets.begin(); itrA != Bullets.end(); ++itrA) {
		for (auto itrB = Bullets.begin(); itrB != Bullets.end(); ++itrB) {
			Bullet* bullet_a = itrA->get();
			Bullet* bullet_b = itrB->get();
			if (itrA == itrB) { continue; }
			if (bullet_a->GetDeadFlag() || bullet_b->GetDeadFlag()) { continue; }
			if (Collision::SphereCollision2(bullet_a->GetPosition(), 1.0f, bullet_b->GetPosition(), 1.0f)) {
				bullet_a->BulletCollision(bullet_b->GetPosition(), bullet_b->GetID());
				bullet_b->BulletCollision(bullet_a->GetPosition(), bullet_a->GetID());
			}
		}
	}
}

void ActorManager::AttachActor(const std::string& ActorName, ActorComponent* newActorCompornent) {
	assert(actorFactory_);
	std::unique_ptr<Actor> newActor;
	newActor.reset(actorFactory_->CreateActor(ActorName, newActorCompornent));
	Actors.push_back(std::move(newActor));
}
void ActorManager::AttachBullet(const std::string& ActorName) {
	assert(actorFactory_);
	std::unique_ptr<Bullet> newBullet;
	newBullet.reset(actorFactory_->CreateBullet(ActorName));
	Bullets.push_back(std::move(newBullet));
}

void ActorManager::RemoveActor() {
	Actors.remove_if([](std::unique_ptr<Actor>& actor) {
		return actor->GetIsRemove();
		});
	Bullets.remove_if([](std::unique_ptr<Bullet>& bullet) {
		return bullet->GetIsRemove();
		});
}

int ActorManager::SearchNum(const std::string& tag) {
	int Bulletnum = 0;
	for (auto itr = Bullets.begin(); itr != Bullets.end(); ++itr) {
		Bulletnum++;
	}
	return Bulletnum;
}

int ActorManager::SerchWaitBul() {
	int Bulletnum = 0;
	for (auto itr = Bullets.begin(); itr != Bullets.end(); ++itr) {
		Bullet* bullet = itr->get();
		if (bullet->GetCommand() == Bullet::BulletStatus::Wait) { Bulletnum++; }
	}
	return Bulletnum;
}

Bullet* ActorManager::CommandBullet(const int& ID) {
	for (auto itr = Bullets.begin(); itr != Bullets.end(); ++itr) {
		Bullet* actor = itr->get();
		if (actor->GetID() == ID) {
			return actor;
		}
	}
	return nullptr;
}

Bullet* ActorManager::SearchWaitBullet() {
	for (auto itr = Bullets.begin(); itr != Bullets.end(); ++itr) {
		Bullet* bullet = itr->get();
		if (bullet->GetCommand() != Bullet::BulletStatus::Wait) { continue; }
		return bullet;
	}
	return nullptr;
}

Bullet* ActorManager::SearchBulletBack() {
	for (auto itr = Bullets.rbegin(); itr != Bullets.rend(); ++itr) {
		Bullet* bullet = itr->get();
		if (bullet->GetCommand() != Bullet::BulletStatus::Wait) { continue; }
		return bullet;
	}
	return nullptr;
}

void ActorManager::DamageBullet(XMFLOAT3 pos, float radius) {
	for (auto itr = Bullets.begin(); itr != Bullets.end(); ++itr) {
		Bullet* bullet = itr->get();
		//if (bullet->GetCommand() == Bullet::command::Wait) { continue; }
		XMFLOAT3 itrPos = bullet->GetPosition();
		if (itrPos.y > 0.1f) { continue; }
		if (Collision::CircleCollision(pos.x, pos.z, radius, itrPos.x, itrPos.z, 1.0f)) {
			bullet->SetDeadFlag(true);
		}
	}
}

XMFLOAT3 ActorManager::Dist(XMFLOAT3 pos, XMFLOAT3 pos2) {
	XMFLOAT3 itr{};
	itr.x = sqrtf(powf((pos2.x - pos.x), 2));
	itr.y = sqrtf(powf((pos2.y - pos.y), 2));
	itr.z = sqrtf(powf((pos2.z - pos.z), 2));
	return itr;
}

float ActorManager::Length(XMFLOAT3 pos, XMFLOAT3 pos2) {
	float itr = 0.0f;

	itr = sqrtf(powf((pos2.x - pos.x), 2) + powf((pos2.y - pos.y), 2) + powf((pos2.z - pos.z), 2));

	return itr;
}

void ActorManager::ChangeBulletCommand(XMFLOAT3 pos, float scale) {
	for (auto itr = Bullets.begin(); itr != Bullets.end(); ++itr) {
		Bullet* bullet = itr->get();
		if (bullet->GetCommand() != Bullet::BulletStatus::Attack) { continue; }
		if (bullet->GetIsPlayActive()) { continue; }
		if (Collision::CircleCollision(pos.x, pos.z, scale, bullet->GetPosition().x, bullet->GetPosition().z, 1.0f)) {
			bullet->SetCommand(Bullet::BulletStatus::Wait);
		}
	}
}

Bullet* ActorManager::SetActionBullet(const XMFLOAT3& pos, const float& _collide_size) {
	for (auto itr = Bullets.begin(); itr != Bullets.end(); ++itr) {
		Bullet* bullet = itr->get();
		if (bullet->GetCommand() != Bullet::BulletStatus::Attack) { continue; }
		if (Collision::CircleCollision(pos.x, pos.z, _collide_size, bullet->GetPosition().x, bullet->GetPosition().z, 1.0f)) {
			if (!bullet->GetIsPlayActive()) {
				return bullet;
			}
		}
	}
	return nullptr;
}


Actor* ActorManager::SearchActorArea(const XMFLOAT3& pos) {
	Actor* itrActor = SearchActor("Player");
	const float limit = 30.0f;
	float check = limit;
	for (auto itr = Actors.begin(); itr != Actors.end(); ++itr) {
		Actor* actor = itr->get();
		if (actor->GetTag() == "Player" || actor->GetTag() == "Bullet") { continue; }
		if (actor->GetIsActive() == false) { continue; }
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

Actor* ActorManager::GetAreaActor(const XMFLOAT3& pos, const std::string& tag) {
	Actor* itrActor = nullptr;
	const float limit = 40.0f;
	float check = limit;
	for (auto itr = Actors.begin(); itr != Actors.end(); ++itr) {
		Actor* actor = itr->get();
		if (actor->GetTag() != tag) { continue; }
		if (actor->GetIsActive() == false) { continue; }
		XMFLOAT3 difPos = actor->GetPosition();
		float dif;
		dif = Length(difPos, pos);
		if (check > dif) {
			check = dif;
			itrActor = actor;
		}
	}
	return itrActor;

}



Actor* ActorManager::SearchActor(const std::string& tag) {

	for (auto itr = Actors.begin(); itr != Actors.end(); ++itr) {
		Actor* actor = itr->get();
		if (actor->GetIsRefer()) { continue; }
		if (actor->GetTag() == tag) {
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

Bullet* ActorManager::SearchID(int ID) {
	if (ID < 0) { assert(0); }
	for (auto itr = Bullets.begin(); itr != Bullets.end(); ++itr) {
		Bullet* actor = itr->get();
		if (actor->GetID() == ID) {
			return actor;
		}
	}
	return nullptr;
}

void ActorManager::ChangeStatus(const Bullet::BulletStatus& old_status, const Bullet::BulletStatus& status) {
	for (auto itr = Bullets.begin(); itr != Bullets.end(); ++itr) {
		Bullet* actor = itr->get();
		if (actor->GetCommand() == old_status) {
			actor->SetCommand(status);
		}
	}
}

