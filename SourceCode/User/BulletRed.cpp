#include"BulletRed.h"
#include"ActorManager.h"
#include"ModelManager.h"
#include <SourceCode/FrameWork/collision/Collision.h>
#include <SourceCode/Common/Easing.h>
using namespace DirectX;



BulletRed::BulletRed() {
}

void BulletRed::OnInit() {
	ID = ActorManager::GetInstance()->SearchNum("BulletRed");
	command = Wait;
	player = ActorManager::GetInstance()->SearchActor("Player");
	enemy = ActorManager::GetInstance()->SearchActorBack("Enemy");
	landing = player->GetLockPos();
	Color = DeathColor::Red;

	Texture* CharaDead_ = Texture::Create(Color, { fbxObj->GetPosition().x,fbxObj->GetPosition().y,fbxObj->GetPosition().z},
		{ 0.3f,0.3f,0.3f }, { 1,1,1,1 });
	CharaDead_->SetIsBillboard(true);
	CharaDead_->TextureCreate();
	CharaDead_->SetRotation({ 0,0,0 });
	CharaDead.reset(CharaDead_);


	Texture* Lock_ = Texture::Create(ImageManager::Battle, { fbxObj->GetPosition().x,fbxObj->GetPosition().y + 1.0f,fbxObj->GetPosition().z
		}, { 0.1f,0.1f,0.1f }, { 1,1,1,1 });
	Lock_->SetIsBillboard(true);
	Lock_->TextureCreate();
	Lock_->SetRotation({ 0,0,0 });
	Status.reset(Lock_);

	Texture* Explo_ = Texture::Create(ImageManager::Fire, { fbxObj->GetPosition().x,fbxObj->GetPosition().y + 1.0f,fbxObj->GetPosition().z
		}, { 0.1f,0.1f,0.1f }, { 1,1,1,1 });
	Explo_->SetIsBillboard(true);
	Explo_->TextureCreate();
	Explo_->SetRotation({ 0,0,0 });
	Explo.reset(Explo_);

	WaitBullet();
}

void BulletRed::OnUpda() {
	if (collide) { collide = false; }
	if (DeadFlag) {
		DeadEnd();
		return;
	}
	switch (command) {
	case Wait:
		//fbxObj->SetRotation({ 0,fbxObj->GetRotation().y,0 });
		WaitUpda();
		break;
	case Attack:
		if (enemy->GetIsActive()) {
			if (knockBacking) {
				KnockBack();
			} else {
				AttackUpda();
			}
		} else {
			if (knockBacking) {
				KnockBack();
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

void BulletRed::OnDraw(DirectXCommon* dxCommon) {
	if (ID==0) {
		ImGui::Begin("bullet");
		float rot = fbxObj->GetRotation().y;
		ImGui::SliderFloat("Bulletrotation", &rot, 0, 360);
		ImGui::End();
	}
	if (enemy == NULL) { return; }
	if (enemy->GetIsActive()) {
		if (command == Wait) { return; }
		Texture::PreDraw();
		if (!DeadFlag) {
			if (Collision::CircleCollision(fbxObj->GetPosition().x, fbxObj->GetPosition().z, 15.0f, enemy->GetPosition().x, enemy->GetPosition().z, 1.0f)) {
				Status->Draw();
			}
			if (burning) {
				Explo->Draw();
			}
		} else {
			CharaDead->Draw();
		}
	}
}
void BulletRed::OnFinal() {
}

void BulletRed::OnCollision(const std::string& Tag, const XMFLOAT3& pos) {
	if (Tag == "Player") {
		switch (command) {
		case Wait:

			break;
		case Attack:
			player->SetStock(player->GetStock() + 1);
			command = Wait;
			break;
		case Slow:

			break;

		default:
			assert(0);
			break;
		}
	}

	if (Tag == "Honey") {
		switch (command) {
		case Wait:
			break;
		case Attack:
			if (!isPlayActive) {
				ActionActor = ActorManager::GetInstance()->GetAreaActor(fbxObj->GetPosition(), "Honey");
				ActionActor->SetStock(ActionActor->GetStock() + 1);
			} else {

			}
			break;
		case Slow:

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
			case Actor::Phase::DEAD:

			default:
				break;
			}

			break;
		case Slow:

			break;

		default:
			assert(0);
			break;
		}
	}
}

void BulletRed::ResultOnUpdate(const int& Timer) {
	XMFLOAT3 rot = fbxObj->GetRotation();
	XMFLOAT3 pos = fbxObj->GetPosition();
	pos.y += vel; //+
	vel -= 0.05f;//
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
	Shadow->Update();
	Shadow->SetPosition({ fbxObj->GetPosition().x,0.01f, fbxObj->GetPosition().z });
}

void BulletRed::BulletCollision(const XMFLOAT3& pos, const int& Id) {
	if (collide) { return; }
	if (command == Dead) { return; }
	//if (command == Wait) { return; }
	if (ID > Id) { return; }
	XMFLOAT3 pos2 = fbxObj->GetPosition();
	if (!collide) {
	}
	collide = true;

	rad = atan2f((pos2.x - pos.x), (pos2.z - pos.z));
	float dir = ((pos.x * pos2.z) - (pos2.x * pos.z));

	if (dir <= 0) {
		pos2.x += sin(rad+randRad) * 0.1f;
		pos2.z += cos(rad +randRad) * 0.1f;
	} else {
		pos2.x -= sin(rad+randRad) * 0.1f;
		pos2.z -= cos(rad +randRad) * 0.1f;

	}
	fbxObj->SetPosition(pos2);
}



