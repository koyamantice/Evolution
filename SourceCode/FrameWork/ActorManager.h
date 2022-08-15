#pragma once
#include <list>
#include <memory>
#include<cassert>
#include"ActorFactory.h"


class ActorManager {
public:
	static ActorManager* GetInstance();
	/// <summary>
	/// 更新
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	/// <summary>
	/// 更新
	/// </summary>
	void DemoUpdate();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();
	/// <summary>
/// 更新
/// </summary>
	void DemoDraw();

	/// <summary>
	/// 終了処理
	/// </summary>
	void Finalize();

	/// <summary>
/// 衝突判定と応答
/// </summary>
	void CheckAllCollisions();

	void AttachActor(const std::string& ActorName, ActorComponent* newActorCompornent=nullptr);

	void RemoveActor();

	const int& SearchNum(const std::string& tag);

	Actor* CommandActor(const int& ID);


	Actor* SearchActor(const std::string& tag);
	Actor* SearchActorBack(const std::string& tag);
private:
	ActorFactory* actorFactory_ = nullptr;

	std::list<std::unique_ptr<Actor>> Actors;

	int num = 0;

	ActorManager()=default;
	~ActorManager()=default;
	ActorManager(const ActorManager&) = delete;
	void operator =(const ActorManager&) = delete;
};