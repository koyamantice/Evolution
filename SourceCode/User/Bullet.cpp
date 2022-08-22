#include "Bullet.h"
#include "Easing.h"
#include"ActorManager.h"
#include <SourceCode/FrameWork/collision/Collision.h>
#include"ImageManager.h"
using namespace DirectX;



Bullet::Bullet() {

	//ease = true;
}

void Bullet::OnInit() {
	ID = ActorManager::GetInstance()->SearchNum("Bullet");
	command = Wait;
	player = ActorManager::GetInstance()->SearchActor("Player");
	enemy = ActorManager::GetInstance()->SearchActorBack("Enemy");
	obj->SetScale({ 0.3f, 0.3f, 0.3f });
	obj->SetColor({ 1.0f, 1.0f, 1.0f,1.0f });
	obj->SetPosition({ ID * 1.0f,0,ID * 1.0f });
	landing = player->GetLockPos();
	Texture* Lock_ = Texture::Create(ImageManager::Battle, { obj->GetPosition().x,obj->GetPosition().y + 1.0f,obj->GetPosition().z
		}, { 0.1f,0.1f,0.1f }, { 1,1,1,1 });
	Lock_->SetIsBillboard(true);
	Lock_->TextureCreate();
	Lock_->SetRotation({ 0,0,0 });
	//Lock_->SetColor({ 1.0f,0.2f,0.2f ,0.6f });
	Status.reset(Lock_);
}

void Bullet::OnUpda() {
	XMFLOAT3 pos = obj->GetPosition();

	switch (command) {
	case Wait:

		WaitUpda();
		break;
	case Attack:

		AttackUpda();
		break;
	default:
		assert(0);
		break;
	}
	Status->Update();
	Status->SetPosition({ obj->GetPosition().x,obj->GetPosition().y + 2.5f,obj->GetPosition().z });
}
void Bullet::Follow2Enemy() {
	XMFLOAT3 pos = obj->GetPosition();
	XMFLOAT3 rot = obj->GetRotation();
	XMFLOAT3 position{};
	position.x = (enemy->GetPosition().x - pos.x);
	position.z = (enemy->GetPosition().z - pos.z);
	rot.y = (atan2f(position.x, position.z) * (180.0f / XM_PI)) - 180; //- 90;// *(XM_PI / 180.0f);
	vel_follow.x = sin(-atan2f(position.x, position.z)) * 0.3f;
	vel_follow.y = cos(-atan2f(position.x, position.z)) * 0.3f;
	pos.x -= vel_follow.x;
	pos.z += vel_follow.y;
	obj->SetPosition(pos);
	obj->SetRotation(rot);
}

void Bullet::Follow2Player() {
	XMFLOAT3 pos = obj->GetPosition();
	XMFLOAT3 rot = obj->GetRotation();
	float vel = (int)(rand() % 10+1) * 0.03f;
	XMFLOAT3 position{};
	position.x = (player->GetPosition().x - pos.x);
	position.z = (player->GetPosition().z - pos.z);
	rot.y = (atan2f(position.x, position.z) * (180.0f / XM_PI)) - 180; //- 90;// *(XM_PI / 180.0f);
	vel_follow.x = sinf(-atan2f(position.x, position.z)) * vel;
	vel_follow.y = cosf(-atan2f(position.x, position.z)) * vel;
	pos.x -= vel_follow.x;
	pos.z += vel_follow.y;
	obj->SetPosition(pos);
	obj->SetRotation(rot);

}


void Bullet::OnDraw() {
	if (enemy->GetIsActive()) {
		if (follow) {
			Texture::PreDraw();
			Status->Draw();
		}
	}
}

void Bullet::OnFinal() {
}

void Bullet::OnCollision(const std::string& Tag) {

	if (Tag == "Player") {
		isRemove = true;
	}

	if (Tag == "Enemy") {
		if (CoolTime == 0) {
			//	obj->SetPosition({ obj->GetPosition(), , obj->GetPosition()});
			enemy->SetHp(enemy->GetHp() - 1);
			CoolTime++;
		}
		follow = false;
	}




}

void Bullet::WaitUpda() {
	if (!Collision::CircleCollision(obj->GetPosition().x, obj->GetPosition().z, 3.0f, player->GetPosition().x, player->GetPosition().z, 1.0f)) {
		Follow2Player();
	}
}

void Bullet::AttackUpda() {

	Follow2Enemy();
}
