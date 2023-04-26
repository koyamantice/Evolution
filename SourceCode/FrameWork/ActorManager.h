#pragma once
#include <DirectXCommon.h>
#include <list>
#include <memory>
#include<cassert>
#include"ActorFactory.h"


class ActorManager {
private:
	using XMFLOAT3 = DirectX::XMFLOAT3;
private:
	std::list<std::unique_ptr<Actor>> Actors;
	std::list<std::unique_ptr<Bullet>> Bullets;
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
	/// �f���X�V
	/// </summary>
	void DemoUpdate();
	/// <summary>
	/// �ŏ��̒e�Ɩ��ҍX�V
	/// </summary>
	void IntroUpdate(const float& timer, const std::string& voidname = "null", const int& _stage=0);
	/// <summary>
	/// �ŏ��̍X�V
	/// </summary>
	void ResultUpdate(const float& timer);
	/// <summary>
	/// �`��
	/// </summary>
	void Draw(DirectXCommon* dxCommon);
	/// <summary>
	/// �X�V
	/// </summary>
	void DemoDraw(DirectXCommon* dxCommon);
	/// <summary>
	/// �I������
	/// </summary>
	void Finalize();

	/// <summary>
	/// �Փ˔���Ɖ���
	/// </summary>
	void CheckActorCollisions();
	/// <summary>
	/// �Փ˔���Ɖ���
	/// </summary>
	void CheckBulletCollisions();

	void AttachActor(const std::string& ActorName, ActorComponent* newActorCompornent=nullptr);
	void AttachBullet(const std::string& ActorName);
	void RemoveActor();

	int SearchNum(const std::string& tag);
	int SerchWaitBul();
	Bullet* CommandBullet(const int& ID);
	Bullet* SearchWaitBullet();
	Bullet* SearchBulletBack();
	Bullet* SetActionBullet(const XMFLOAT3& pos,const float& _collide_size=5.0f);
	Bullet* SearchID(int ID);
	void ChangeStatus(const Bullet::BulletStatus& old_status, const Bullet::BulletStatus& status, const XMFLOAT3& pos = {});

	void DamageBullet(XMFLOAT3 pos, float radius = 1.0f);

	void ChangeBulletCommand(XMFLOAT3 pos,float scale);

	Actor* SearchActorArea(const XMFLOAT3& pos);
	Actor* SearchActor(const std::string& tag);
	Actor* SearchActorBack(const std::string& tag);
	Actor* GetAreaActor(const XMFLOAT3& pos, const std::string& tag);



	DirectX::XMFLOAT3 Dist(XMFLOAT3 pos, XMFLOAT3 pos2);
	float Length(XMFLOAT3 pos, XMFLOAT3 pos2);

private:
	ActorFactory* actorFactory_ = nullptr;
	ActorManager()=default;
	~ActorManager()=default;
	ActorManager(const ActorManager&) = delete;
	void operator =(const ActorManager&) = delete;
};