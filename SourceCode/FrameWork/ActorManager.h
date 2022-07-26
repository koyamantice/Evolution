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
	/// 描画
	/// </summary>
	void Draw();
	/// <summary>
	/// 終了処理
	/// </summary>
	void Finalize();

	/// <summary>
/// 衝突判定と応答
/// </summary>
	void CheckAllCollisions();

	void AttachActor(const std::string& ActorName);

	void RemoveActor();
	Actor* SearchActor(const std::string& tag);
private:
	ActorFactory* actorFactory_ = nullptr;

	std::list<std::unique_ptr<Actor>> Actors;

	ActorManager()=default;
	~ActorManager()=default;
	ActorManager(const ActorManager&) = delete;
	void operator =(const ActorManager&) = delete;
};