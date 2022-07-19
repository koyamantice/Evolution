#pragma once
#include"Actor.h"

class ActorFactory {
public:
	ActorFactory(){};
	~ActorFactory(){};

	Actor* CreateActor(const std::string& sceneName);
private:

};

