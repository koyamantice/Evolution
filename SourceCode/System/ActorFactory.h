#pragma once
#include"Actor.h"
#include"Bullet.h"
class ActorFactory {
public:
	ActorFactory(){};
	~ActorFactory(){};

	Actor* CreateActor(const std::string& sceneName, ActorComponent* newActorCompornent=nullptr);
	Bullet* CreateBullet(const std::string& BulletKind);
private:

};

