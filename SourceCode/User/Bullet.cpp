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
	obj->SetScale({ 0.5f, 0.5f, 0.5f });
	obj->SetColor({ 1.0f, 1.0f, 1.0f,1.0f });
	obj->SetPosition({ ID * 1.0f,0,ID * 1.0f });
	landing = player->GetLockPos();
	Texture* Lock_ = Texture::Create(ImageManager::Battle, { obj->GetPosition().x,obj->GetPosition().y + 1.0f,obj->GetPosition().z
		}, { 0.1f,0.1f,0.1f }, { 1,1,1,1 });
	Lock_->SetIsBillboard(true);
	Lock_->TextureCreate();
	Lock_->SetRotation({ 0,0,0 });
	Status.reset(Lock_);

	Texture* Explo_ = Texture::Create(ImageManager::Fire, { obj->GetPosition().x,obj->GetPosition().y + 1.0f,obj->GetPosition().z
		}, { 0.1f,0.1f,0.1f }, { 1,1,1,1 });
	Explo_->SetIsBillboard(true);
	Explo_->TextureCreate();
	Explo_->SetRotation({ 0,0,0 });
	Explo.reset(Explo_);
}

void Bullet::OnUpda() {
	XMFLOAT3 pos = obj->GetPosition();

	switch (command) {
	case Wait:
		WaitUpda();
		break;
	case Attack:
		if (enemy->GetIsActive()) {
			if (knockBacking) {
				KnockBack();
			} else {
				AttackUpda();
			}
		}
		break;
	default:
		assert(0);
		break;
	}
	if (burning) {
		Explo->Update();
		BurnOut();
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
	float vel = (int)(rand() % 10 + 1) * 0.03f;
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

void Bullet::KnockBack() {
	XMFLOAT3 pos = obj->GetPosition();
	pos.x += (pos.x / back) * 0.1f;
	pos.y += 0.5f - fall;
	fall += 0.5f / 15.0f;
	pos.z += (pos.z / back) * 0.1f;
	if (pos.y < 0) {
		pos.y = 0;
		fall = 0.0f;
		knockBacking = false;
	}
	obj->SetPosition(pos);
}

float Bullet::Normalize(const XMFLOAT3& pos, const XMFLOAT3& pos2) {
	XMFLOAT3 itr{};
	itr = { pos.x - pos2.x,0,pos.z - pos2.z };
	back = sqrtf(powf(itr.x, 2) + powf(itr.z, 2));
	//back = abs(back);
	return back;
}

void Bullet::BurnOut() {
	XMFLOAT3 pos = obj->GetPosition();
	if (effectRate < 1.0f) {
		effectRate += 0.08f;
	} else {
		effectRate = 0;
		Explo->SetScale({ 0,0,0});
		Explo->SetPosition({ pos.x,-100,pos.z });
		burning = false;
	}
	scale = Ease(In, Quad, effectRate, 0.5f, 1.0f);
	Explo->SetScale({ scale,scale,scale });
	Explo->SetPosition({ pos.x,pos.y,pos.z });
}

void Bullet::OnDraw(DirectXCommon* dxCommon) {
	if (enemy == NULL) { return; }
	if (enemy->GetIsActive()) {
		if (command == Wait) { return; }
		if (Collision::CircleCollision(obj->GetPosition().x, obj->GetPosition().z, 15.0f, enemy->GetPosition().x, enemy->GetPosition().z, 1.0f)) {
			Texture::PreDraw();
			Status->Draw();
		}
		if (burning) {
			Explo->Draw();
		}
	}
}

void Bullet::OnFinal() {
}

void Bullet::OnCollision(const std::string& Tag) {
	if (Tag == "Player") {
		switch (command) {
		case Wait:

			break;
		case Attack:
			player->SetStock(player->GetStock() + 1);
			command = Wait;
			break;
		default:
			assert(0);
			break;
		}
	}

	if (Tag == "Enemy") {
		switch (command) {
		case Wait:

			break;
		case Attack:
			if (!knockBacking) {
				knockBacking = true;
				enemy->SetHp(enemy->GetHp()-1);
				burning = true;
				back = Normalize(obj->GetPosition(), enemy->GetPosition());
			}
			break;
		default:
			assert(0);
			break;
		}

	}




}

void Bullet::WaitUpda() {
	throwReady = true;
	if (!Collision::CircleCollision(obj->GetPosition().x, obj->GetPosition().z, 3.0f, player->GetPosition().x, player->GetPosition().z, 1.0f)) {
		Follow2Player();
	}
}

void Bullet::AttackUpda() {
	if (throwReady) {
		XMFLOAT3 pos = obj->GetPosition();
		pos.x = Ease(InOut, Quad, frame, pos.x, AftaerPos.x);
		pos.y += vel; //+
		vel -= 0.05f;//
		if (pos.y < 0.0f) {
			pos.y = 0;
		}
		pos.z = Ease(InOut, Quad, frame, pos.z, AftaerPos.z);
		if (frame < 1.0f) {
			frame += 0.02f;
		} else {
			frame = 1.0f;
			pos.y = 0.0f;
			vel = 0.8f;
			throwReady = false;
		}
		obj->SetPosition(pos);

	} else {
		frame = 0.0f;
		if (Collision::CircleCollision(obj->GetPosition().x, obj->GetPosition().z, 15.0f, enemy->GetPosition().x, enemy->GetPosition().z, 1.0f)) {
			Follow2Enemy();
		}
	}

}
