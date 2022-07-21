#pragma once
#include <list>
#include <memory>
#include<cassert>
#include"ActorFactory.h"


class ActorManager {
public:
	static ActorManager* GetInstance();
	/// <summary>
	/// �X�V
	/// </summary>
	void Initialize();

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();
	/// <summary>
	/// �`��
	/// </summary>
	void Draw();
	/// <summary>
	/// �I������
	/// </summary>
	void Finalize();


	void AttachActor(const std::string& ActorName);

	void RemoveActor();
	Actor* SearchActor(const std::string& tag);
private:
	ActorFactory* actorFactory_ = nullptr;
	std::list<std::unique_ptr<Actor>> Actors;
	std::list<std::unique_ptr<Actor>> removeActors;

	ActorManager()=default;
	~ActorManager()=default;
	ActorManager(const ActorManager&) = delete;
	void operator =(const ActorManager&) = delete;
};