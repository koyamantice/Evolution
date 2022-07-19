#pragma once
#include <list>
#include <memory>
#include<cassert>
#include <SourceCode/Obj/Actor.h>


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