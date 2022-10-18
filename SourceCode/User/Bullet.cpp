#include "Bullet.h"
#include "Easing.h"
#include"ActorManager.h"
#include <SourceCode/FrameWork/collision/Collision.h>
#include"ImageManager.h"
#include"ModelManager.h"
using namespace DirectX;



Bullet::Bullet() {

	//ease = true;
}

void Bullet::OnInit() {
	ID = ActorManager::GetInstance()->SearchNum("Bullet");
	command = Wait;
	player = ActorManager::GetInstance()->SearchActor("Player");
	enemy = ActorManager::GetInstance()->SearchActorBack("Enemy");
	//obj->SetScale({ 0.5f, 0.5f, 0.5f });
	//obj->SetColor({ 1.0f, 0.0f, 0.0f,1.0f });
	XMFLOAT3 rot = obj->GetRotation();
	rot.y = -180; //- 90;// *(XM_PI / 180.0f);
	obj->SetRotation(rot);

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

	FBXObject3d* Bird_ = new FBXObject3d();
	Bird_->Initialize();
	Bird_->SetModel(ModelManager::GetIns()->GetFBXModel(ModelManager::Bird));
	Bird_->SetScale({ 0.003f,0.003f, 0.003f });
	//move_object_->SetPosition(position);
	//move_object_->SetRotation(rot);
	Bird.reset(Bird_);
	Bird->LoadAnimation();
	Bird->PlayAnimation();
	isVisible = false;
	WaitBullet();
}

void Bullet::OnUpda() {
	XMFLOAT3 pos = obj->GetPosition();
	Bird->Update();
	Bird->SetPosition(pos);
	Bird->SetRotation(obj->GetRotation());

	if (DeadFlag) {
		Dead();
		return;
	}
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
	case Follow:
		//WaitBullet();
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
	XMFLOAT3 position{};
	position.x = (player->GetPosition().x - pos.x);
	position.z = (player->GetPosition().z - pos.z);
	{
		rot.y = (atan2f(position.x, position.z) * (180.0f / XM_PI)) - 180; //- 90;// *(XM_PI / 180.0f);
		obj->SetRotation(rot);
	}
	float vel = (int)(rand() % 10 + 1) * 0.03f;

	vel_follow.x = sinf(-atan2f(position.x, position.z)) * vel;
	vel_follow.y = cosf(-atan2f(position.x, position.z)) * vel;
	pos.x -= vel_follow.x;
	pos.z += vel_follow.y;
	obj->SetPosition(pos);

}

void Bullet::WaitBullet() {

	obj->SetPosition({ ((int)ID % 10) * 3.0f, 0, (((int)ID / 10) * 5.0f) - 30.0f });
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

void Bullet::DamageInit() {
	if (!knockBacking) {
		knockBacking = true;
		enemy->SetHp(enemy->GetHp() - 1);
		burning = true;
		back = Normalize(obj->GetPosition(), enemy->GetPosition());
	}
}

float Bullet::Normalize(const XMFLOAT3& pos, const XMFLOAT3& pos2) {
	XMFLOAT3 itr{};
	float nor;
	itr = { pos.x - pos2.x,0,pos.z - pos2.z };
	nor = sqrtf(powf(itr.x, 2) + powf(itr.z, 2));
	//back = abs(back);
	return nor;
}

void Bullet::Dead() {
	isRemove = true;
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
	//ImGui::Begin("test");
	//ImGui::SliderFloat("bullet", &back, 0, 360);
	////ImGui::Unindent();
	//ImGui::End();
	Object3d::PreDraw();
	Bird->Draw(dxCommon->GetCmdList());
	if (enemy == NULL) { return; }
	if (enemy->GetIsActive()) {
		if (command == Wait) { return; }
		Texture::PreDraw();
		if (Collision::CircleCollision(obj->GetPosition().x, obj->GetPosition().z, 15.0f, enemy->GetPosition().x, enemy->GetPosition().z, 1.0f)) {
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
		int a = 0;
		switch (command) {
		case Wait:
			break;
		case Attack:
			switch (enemy->GetCommand()) {
			case Actor::Phase::WAIT:
				DamageInit();
					break;
			case Actor::Phase::ATTACK:
				break;
			case Actor::Phase::LEAVE:
				DamageInit();
				break;
			default:
				break;
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
	XMFLOAT3 pos = obj->GetPosition();
	if (pos.y>0) {
		pos.y -= 0.3f;
	}else{
		pos.y = 0;
	}
	obj->SetPosition(pos);

	if (!Collision::CircleCollision(obj->GetPosition().x, obj->GetPosition().z, 3.0f, player->GetPosition().x, player->GetPosition().z, 1.0f)) {
		//Follow2Player();
		WaitBullet();
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
