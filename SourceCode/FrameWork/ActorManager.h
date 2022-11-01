#pragma once
#include <list>
#include <memory>
#include<cassert>
#include"ActorFactory.h"
#include <SourceCode/System/DirectXCommon.h>


class ActorManager {
private:
	using XMFLOAT3 = DirectX::XMFLOAT3;
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
	/// �X�V
	/// </summary>
	void DemoUpdate();

	/// <summary>
	/// �ŏ��̍X�V
	/// </summary>
	void IntroUpdate();
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

	const int& SearchNum(const std::string& tag);
	const int& SerchWaitBul();
	Bullet* CommandBullet(const int& ID);

	Bullet* SearchWaitBullet();

	void DamageBullet(XMFLOAT3 pos, float radius = 1.0f);

	DirectX::XMFLOAT3 Dist(XMFLOAT3 pos,XMFLOAT3 pos2);
	float Length(XMFLOAT3 pos, XMFLOAT3 pos2);

	void ChangeBulletCommand(XMFLOAT3 pos,float scale);
	//�t���b�L���O�A���S���Y��
	void BoidIsolate();
	//�t���b�L���O�A���S���Y��
	void BoidAverage();
	//�t���b�L���O�A���S���Y��
	void BoidAlignment();

	Actor* SearchActorArea(XMFLOAT3 pos);
	Actor* SearchActor(const std::string& tag);

	Actor* SearchActorBack(const std::string& tag);

	Bullet* SearchID(int ID);
private:
	ActorFactory* actorFactory_ = nullptr;

	std::list<std::unique_ptr<Actor>> Actors;
	std::list<std::unique_ptr<Bullet>> Bullets;
	int Bulletnum = 0;

	float contX[100];
	float contY[100];


	ActorManager()=default;
	~ActorManager()=default;
	ActorManager(const ActorManager&) = delete;
	void operator =(const ActorManager&) = delete;
};