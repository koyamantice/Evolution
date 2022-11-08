#include"Bullet.h"
#include <SourceCode/User/ModelManager.h>
#include <SourceCode/FrameWork/ActorManager.h>
#include"ImageManager.h"
#include <SourceCode/Common/Easing.h>
#include <SourceCode/FrameWork/collision/Collision.h>



Bullet::Bullet() {
}

void Bullet::Initialize(FBXModel* model, const std::string& tag, ActorComponent* compornent) {
	FBXObject3d* fbxObj_ = new FBXObject3d();
	fbxObj_->Initialize();
	fbxObj_->SetModel(model);
	fbxObj_->SetScale({ 0.003f,0.003f, 0.003f });
	fbxObj.reset(fbxObj_);
	fbxObj->LoadAnimation();
	fbxObj->PlayAnimation();
	Texture* Shadow_ = Texture::Create(ImageManager::Shadow, { 0,0,0 },
		{ 0.2f,0.2f,0.2f }, { 1,1,1,1 });
	//Shadow_->SetIsBillboard(true);
	Shadow_->TextureCreate();
	Shadow_->SetRotation({ 90,0,0 });
	Shadow.reset(Shadow_);

	OnInit();

}

void Bullet::Update() {
	if (isActive) {
		fbxObj->Update();
		oldPos = fbxObj->GetPosition();
		Shadow->Update();
		Shadow->SetPosition({ fbxObj->GetPosition().x,0.01f, fbxObj->GetPosition().z });

		OnUpda();
	}

}

void Bullet::Demo() {
}

void Bullet::IntroUpdate(const int& Timer) {
	if (isActive) {
		if(Timer<5){
			fbxObj->Update();
			fbxObj->SetPosition({ (((int)ID % 10)- 5) * 3.0f, hight, ((int)ID / 10) * 5.0f });
		} else {
			fbxObj->Update();
			fbxObj->SetPosition({ (((int)ID % 10) - 4.5f) * 3.0f, hight, ((int)ID / 10) * 5.0f });
			fbxObj->SetRotation({ 0,180,0 });
			if (hight > 0) {
				hight-=0.2f;
			}

		}

		//Shadow->Update();
		//Shadow->SetPosition({ fbxObj->GetPosition().x,0.01f, fbxObj->GetPosition().z });
		IntroOnUpdate(Timer);
	}

}

void Bullet::ResultUpdate(const int& Timer) {
	
	ResultOnUpdate(Timer);

}

void Bullet::Draw(DirectXCommon* dxCommon) {
	if (isActive) {
		Object3d::PreDraw();
		fbxObj->Draw(dxCommon->GetCmdList());
		Texture::PreDraw();
		Shadow->Draw();
		OnDraw(dxCommon);
	}
}

void Bullet::DemoDraw(DirectXCommon* dxCommon) {
	if (isActive) {
		DebugUpdate();
		Object3d::PreDraw();
		fbxObj->Draw(dxCommon->GetCmdList());
		OnDraw(dxCommon);
	}
}

void Bullet::Finalize() {
}

void Bullet::BoidAverage() {
	XMFLOAT3 pos = fbxObj->GetPosition();
	player = ActorManager::GetInstance()->SearchActor("Player");
	flocking.average = player->GetPosition();
	flocking.ctrDirX = flocking.average.x - pos.x;
	flocking.ctrDirY = flocking.average.z - pos.z;
}

void Bullet::Move() {
	XMFLOAT3 pos = fbxObj->GetPosition();
	BoidAverage();
	float kX = 0.7f * flocking.ctrDirX + 0.2f * flocking.vel.x + 0.8f * flocking.contX;
	float kY = 0.7f * flocking.ctrDirY + 0.2f * flocking.vel.x + 0.8f * flocking.contY;

	float tempVel = sqrtf(kX * kX + kY * kY);
	if (tempVel > 0.2f) {
		kX = 0.2f * kX / tempVel;
		kY = 0.2f * kY / tempVel;
	}

	dx += (kX - dx) * 0.02f;
	dy += (kY - dy) * 0.02f;


	pos.x += dx;
	pos.z += dy;
	fbxObj->SetPosition(pos);
}



void Bullet::SetCommand(const int& command, XMFLOAT3 pos) {
	this->command = command; AftaerPos = pos;
}

void Bullet::Follow2Enemy() {
	XMFLOAT3 pos = fbxObj->GetPosition();
	XMFLOAT3 rot = fbxObj->GetRotation();
	XMFLOAT3 position{};
	position.x = (enemy->GetPosition().x - pos.x);
	position.z = (enemy->GetPosition().z - pos.z);
	rot.y = (atan2f(position.x, position.z) * (180.0f / XM_PI)) - 180; //- 90;// *(XM_PI / 180.0f);
	vel_follow.x = sin(-atan2f(position.x, position.z)) * 0.3f;
	vel_follow.y = cos(-atan2f(position.x, position.z)) * 0.3f;
	pos.x -= vel_follow.x;
	pos.z += vel_follow.y;
	fbxObj->SetPosition(pos);
	fbxObj->SetRotation(rot);
}

void Bullet::Follow2Player() {
	XMFLOAT3 pos = fbxObj->GetPosition();
	XMFLOAT3 rot = fbxObj->GetRotation();
	float vel = (int)(rand() % 10 + 1) * 0.03f;
	XMFLOAT3 position{};
	position.x = (player->GetPosition().x - pos.x);
	position.z = (player->GetPosition().z - pos.z);
	rot.y = (atan2f(position.x, position.z) * (180.0f / XM_PI)) - 180; //- 90;// *(XM_PI / 180.0f);
	vel_follow.x = sinf(-atan2f(position.x, position.z)) * vel;
	vel_follow.y = cosf(-atan2f(position.x, position.z)) * vel;
	pos.x -= vel_follow.x;
	pos.z += vel_follow.y;
	//fbxObj->SetPosition(pos);
	fbxObj->SetRotation(rot);

}

void Bullet::WaitBullet() {

	fbxObj->SetPosition({ ((int)ID % 10) * 3.0f, 0, ((int)ID / 10) * 5.0f });
}

void Bullet::KnockBack() {
	XMFLOAT3 pos = fbxObj->GetPosition();
	rebound.x = sin(atan2f(distance.x, distance.z)) * 0.5f;
	rebound.z = cos(atan2f(distance.x, distance.z)) * 0.5f;

	if (damageframe >= 1.0f) {
		damageframe = 0.0f;
		knockBacking = false;
		fall = 0.4f;
		pos.y = 0.0f;
		fbxObj->SetPosition(pos);
		return;
	} else {
		damageframe += 0.05f;
	}

	rebound = {
	Ease(InOut,Quad,damageframe,rebound.x,0),
	0,
	Ease(InOut,Quad,damageframe,rebound.z,0)
	};
	pos.y += fall; //+
	fall -= 0.16f;//
	if (pos.y < 0.0f) {
		pos.y = 0;
	}
	if (pos.x <= 100.0f && pos.x >= -100.0f) {
		pos.x -= rebound.x;
	}
	if (pos.z <= 100.0f && pos.z >= -100.0f) {
		pos.z -= rebound.z;
	}

	fbxObj->SetPosition(pos);
}

void Bullet::DamageInit() {
	if (!knockBacking) {
		enemy->SetHp(enemy->GetHp() - 1);
		burning = true;
		XMFLOAT3 pos = fbxObj->GetPosition();
		XMFLOAT3 pos2 = enemy->GetPosition();
		if (pos2.x < pos.x) {
			isLeft = true;
		}
		fall = 1.0f;
		exploPos = pos;
		distance = { pos2.x - pos.x,0,pos2.z - pos.z };
		knockBacking = true;
	}
}


void Bullet::DeadEnd() {
	fbxObj->SetScale({ 0.003f,0.0001f, 0.003f });
	deadframe += 0.01f;
	CharaDead->Update();


	vanishHight = Ease(Out,Quad,deadframe,0.1f,4.5f);
	vanishAlpha = Ease(In,Quad,deadframe,1.0f,0.5f);

	CharaDead->SetPosition({ fbxObj->GetPosition().x,vanishHight, fbxObj->GetPosition().z });
	CharaDead->SetColor({ 1,1,1,vanishAlpha });
	if (deadframe > 1.0f) {
		isRemove = true;
	} else {

	}
}

void Bullet::BurnOut() {
	XMFLOAT3 pos = fbxObj->GetPosition();
	if (effectRate < 1.0f) {
		effectRate += 0.08f;
	} else {
		effectRate = 0;
		Explo->SetScale({ 0,0,0 });
		Explo->SetPosition({ pos.x,-100,pos.z });
		burning = false;
	}
	scale = Ease(In, Quad, effectRate, 0.5f, 1.0f);
	Explo->SetScale({ scale,scale,scale });
	Explo->SetPosition(exploPos);
}

void Bullet::WaitUpda() {
	throwReady = true;
	XMFLOAT3 pos = fbxObj->GetPosition();
	if (pos.y > 0) {
		pos.y -= 0.3f;
	} else {
		pos.y = 0;
	}
	if (pos.x > 100) { pos.x = 100; }
	if (pos.z > 100) { pos.z = 100; }
	if (pos.x < -100) { pos.x = -100; }
	if (pos.z < -100) { pos.z = -100; }
	vel = 0.8f;
	frame = 0.0f;

	fbxObj->SetPosition(pos);

	if (!Collision::CircleCollision(fbxObj->GetPosition().x, fbxObj->GetPosition().z, 3.0f, player->GetPosition().x, player->GetPosition().z, 1.0f)) {
		Follow2Player();
		Move();
		//WaitBullet();
	}
}

void Bullet::SlowUpda() {
	if (throwReady) {
		XMFLOAT3 pos = fbxObj->GetPosition();
		XMFLOAT3 rot = fbxObj->GetRotation();
		pos.x = Ease(InOut, Quad, frame, pos.x, AftaerPos.x);
		pos.y += vel; //+
		vel -= 0.05f;//
		if (frame < 0.7f) {
			rot.x = Ease(In, Quad, frame + 0.3f, 0, -360);
		}
		if (pos.y < 0.0f) {
			pos.y = 0;
		}
		pos.z = Ease(InOut, Quad, frame, pos.z, AftaerPos.z);
		if (frame < 1.0f) {
			frame += 0.02f;
		} else {
			frame = 1.0f;
			pos.y = 0.0f;
			rot.x = 0;
			vel = 0.8f;
			throwReady = false;
		}
		fbxObj->SetPosition(pos);
		fbxObj->SetRotation(rot);

	} else {
		frame = 0.0f;
		vel = 0.8f;
		fbxObj->SetRotation({ 0,fbxObj->GetRotation().y,0 });
		command = Attack;
	}

}

void Bullet::AttackUpda() {
	if (!knockBacking) {
		if (Collision::CircleCollision(fbxObj->GetPosition().x, fbxObj->GetPosition().z, 15.0f, enemy->GetPosition().x, enemy->GetPosition().z, 1.0f)) {
			Follow2Enemy();
		}
	}


}
