#include"ActorFactory.h"
#include"ModelManager.h"
#include"Player.h"
#include"Enemy.h"
#include"Bullet.h"




Actor* ActorFactory::CreateActor(const std::string& sceneName) {
	//ŽŸ‚ÌƒV[ƒ“‚Ìì¬
	Actor* newActor = nullptr;
	
	if (sceneName == "Player") {
		newActor = new Player();
		newActor->Initialize(ModelManager::GetIns()->GetModel(ModelManager::Player),"Player");
	}
	if (sceneName == "Enemy") {
		newActor = new Enemy();
		newActor->Initialize(ModelManager::GetIns()->GetModel(ModelManager::Enemy), "Enemy");
	}
	if (sceneName == "Bullet") {
		newActor = new Bullet();
		newActor->Initialize(ModelManager::GetIns()->GetModel(ModelManager::Player),"Bullet");

	}
	
	return newActor;
}
