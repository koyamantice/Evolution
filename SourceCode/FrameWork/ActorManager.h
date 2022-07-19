#pragma once
#include <list>
#include <memory>
#include<cassert>
#include <SourceCode/Obj/Actor.h>


class ActorManager {
public:
	static ActorManager* GetInstance();


	/// <summary>
	/// XV
	/// </summary>
	void Update();
	/// <summary>
	/// •`‰æ
	/// </summary>
	void Draw();
	/// <summary>
	/// I—¹ˆ—
	/// </summary>
	void Finalize();

	void AttachList(Actor* actor) { 
		std::unique_ptr<Actor> act;act.reset(actor);
		Actors.push_back(std::move(act)); }
private:

	std::list<std::unique_ptr<Actor>> Actors;

	ActorManager()=default;
	~ActorManager()=default;
	ActorManager(const ActorManager&) = delete;
	void operator =(const ActorManager&) = delete;
};