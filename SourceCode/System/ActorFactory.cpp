#include"ActorFactory.h"
#include"ModelManager.h"
#include"Player.h"
#include"MashGhost.h"
#include"Hornet.h"
#include"Bullet.h"
#include"BulletRed.h"
#include"BulletGreen.h"
#include "Crystal.h"
#include "ClearCrystal.h"
#include"PlayerUI.h"
#include "Honey.h"




Actor* ActorFactory::CreateActor(const std::string& sceneName, ActorComponent* newActorCompornent) {
	//ŽŸ‚ÌƒV[ƒ“‚Ìì¬
	Actor* newActor = nullptr;

	if (sceneName == "Player") {
		newActor = new Player();
		newActor->Initialize(nullptr,"Player", newActorCompornent);
	}
	if (sceneName == "MashGhost") {
		newActor = new MashGhost();
		newActor->Initialize(nullptr, "Enemy");
	}
	if (sceneName == "Hornet") {
		newActor = new Hornet();
		newActor->Initialize(nullptr, "Enemy");
	}
	if (sceneName == "ClearCrystal") {
		newActor = new ClearCrystal();
		newActor->Initialize(ModelManager::GetIns()->GetModel(ModelManager::kGoalcrystal),"ClearCrystal");

	}
	if (sceneName == "Crystal") {
		newActor = new Crystal();
		newActor->Initialize(nullptr, "Crystal");

	}
	if (sceneName == "Honey") {
		newActor = new Honey();
		newActor->Initialize(ModelManager::GetIns()->GetModel(ModelManager::kNest), "Honey");

	}
	return newActor;
}

Bullet* ActorFactory::CreateBullet(const std::string& BulletKind) {
	Bullet* newBullet=nullptr;
	if (BulletKind=="Red") {
		newBullet = new BulletRed();
		newBullet->Initialize(ModelManager::GetIns()->GetFBXModel(ModelManager::kRedkobito));
	}
	if (BulletKind == "Green") {
		newBullet = new BulletGreen();
		newBullet->Initialize(ModelManager::GetIns()->GetFBXModel(ModelManager::kGreenkobito));
	}



	return newBullet;
}
