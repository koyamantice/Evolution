#include"BulletGreen.h"
#include "Easing.h"
#include"ActorManager.h"
#include <SourceCode/FrameWork/collision/Collision.h>
#include"ImageManager.h"
#include"ModelManager.h"
using namespace DirectX;



BulletGreen::BulletGreen() {
}

void BulletGreen::OnInit() {
	ID = ActorManager::GetInstance()->SearchNum("BulletGreen");
	command = Wait;
	player = ActorManager::GetInstance()->SearchActor("Player");
	enemy = ActorManager::GetInstance()->SearchActorBack("Enemy");
	landing = player->GetLockPos();
	Object2d* Lock_ = Object2d::Create(ImageManager::Battle, { fbxObj->GetPosition().x,fbxObj->GetPosition().y + 1.0f,fbxObj->GetPosition().z
		}, { 0.1f,0.1f,0.1f }, { 1,1,1,1 });
	Lock_->SetIsBillboard(true);
	Lock_->Object2dCreate();
	Lock_->SetRotation({ 0,0,0 });
	Status.reset(Lock_);

	Object2d* Explo_ = Object2d::Create(ImageManager::Fire, { fbxObj->GetPosition().x,fbxObj->GetPosition().y + 1.0f,fbxObj->GetPosition().z
		}, { 0.1f,0.1f,0.1f }, { 1,1,1,1 });
	Explo_->SetIsBillboard(true);
	Explo_->Object2dCreate();
	Explo_->SetRotation({ 0,0,0 });
	Explo.reset(Explo_);

	Object2d* CharaDead_ = Object2d::Create(Color, { fbxObj->GetPosition().x,fbxObj->GetPosition().y,fbxObj->GetPosition().z },
		{ 0.3f,0.3f,0.3f }, { 1,1,1,1 });
	CharaDead_->SetIsBillboard(true);
	CharaDead_->Object2dCreate();
	CharaDead_->SetRotation({ 0,0,0 });
	CharaDead.reset(CharaDead_);
}

void BulletGreen::OnUpda() {
	if (collide) { collide=false; }
	if(!wait) {Followframe = 0.0f;}
	if (DeadFlag) {
		DeadEnd();
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
	case Slow:
		if (wait) { wait = false; Followframe = 0.0f; }
		SlowUpda();
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
	Status->SetPosition({ fbxObj->GetPosition().x,fbxObj->GetPosition().y + 2.5f,fbxObj->GetPosition().z });
}

void BulletGreen::OnDraw(DirectXCommon* dxCommon) {
	if (enemy == NULL) { return; }
	if (enemy->GetIsActive()) {
		if (command == Wait) { return; }
		Object2d::PreDraw();
		if (Collision::CircleCollision(fbxObj->GetPosition().x, fbxObj->GetPosition().z, 15.0f, enemy->GetPosition().x, enemy->GetPosition().z, 1.0f)) {
			Status->Draw();
		}
		if (burning) {
			Explo->Draw();
		}
	}
}
void BulletGreen::OnFinal() {
}

void BulletGreen::OnCollision(const std::string& Tag, const XMFLOAT3& pos) {
	switch (command) {
	case Wait:

		break;
	case Attack:
		//ƒvƒŒƒCƒ„[‚Æ‚Ì“–‚½‚è”»’è
		if (Tag == "Player") {
			player->SetStock(player->GetStock() + 1);
			command = Wait;
		}
		//–I–¨ˆÚ“®
		if (Tag == "Honey") {
			if (!isPlayActive) {
				ActionActor = ActorManager::GetInstance()->GetAreaActor(fbxObj->GetPosition(), "Honey");
				if (ActionActor->GetStock() < 5) {
					ActionActor->SetStock(ActionActor->GetStock() + 1);
				} else {
					ActionActor = nullptr;
				}
			}
		}
		//“G‚Æ‚Ì“–‚½‚è”»’è
		if (Tag == "Enemy") {
			switch (enemy->GetCommand()) {
			case Actor::Phase::WAIT:
				DamageInit();
				break;
			case Actor::Phase::ATTACK:
				break;
			case Actor::Phase::LEAVE:
				DamageInit();
				break;
			case Actor::Phase::DEAD:

			default:
				break;
			}
		}
		break;
	case Slow:
		break;
	default:
		assert(0);
		break;
	}
}

void BulletGreen::BulletCollision(const XMFLOAT3& pos, const int& Id) {
	if (collide) { return; }
	if (command == Dead) { return; }
	//if (command == Wait) { return; }
	if (ID < Id) { return; }
	collide = true;
	XMFLOAT3 pos2 = fbxObj->GetPosition();

	float dir = ((pos.x * pos2.z) - (pos2.x * pos.z));

	if (dir <= 0) {
		pos2.x += sin(atan2f((pos2.x - pos.x), (pos2.z - pos.z))) * 0.1f;
		pos2.z += cos(atan2f((pos2.x - pos.x), (pos2.z - pos.z))) * 0.1f;
	} else {
		pos2.x -= sin(atan2f((pos2.x - pos.x), (pos2.z - pos.z))) * 0.1f;
		pos2.z -= cos(atan2f((pos2.x - pos.x), (pos2.z - pos.z))) * 0.1f;

	}
	fbxObj->SetPosition(pos2);
}

void BulletGreen::ResultOnUpdate(const int& Timer) {
	XMFLOAT3 rot = fbxObj->GetRotation();
	XMFLOAT3 pos = fbxObj->GetPosition();
	pos.y += vel; //+
	vel -= 0.04f;//
	if (frame < 0.7f) {
		rot.x = Ease(In, Quad, frame + 0.3f, 0, -360);
	}
	if (pos.y < 0.0f) {
		pos.y = 0;
	}
	if (frame < 1.0f) {
		frame += 0.02f;
	} else {
		frame = 0.0f;
		vel = 0.8f;
		rot.x = 0.0f;
		pos.y = 0.0f;
	}
	fbxObj->SetPosition(pos);
	fbxObj->SetRotation(rot);
	fbxObj->Update();
	Shadow->SetPosition({ fbxObj->GetPosition().x,0.01f, fbxObj->GetPosition().z });
	Shadow->Update();
}

