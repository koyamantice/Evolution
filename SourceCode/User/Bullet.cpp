#include "Bullet.h"
#include "Easing.h"
#include"ActorManager.h"
#include <SourceCode/FrameWork/collision/Collision.h>
#include"ImageManager.h"
using namespace DirectX;



Bullet::Bullet() {

	ease = true;
}

void Bullet::OnInit() {
	player = ActorManager::GetInstance()->SearchActor("Player");
	enemy = ActorManager::GetInstance()->SearchActorBack("Enemy");
	obj->SetScale(XMFLOAT3(1, 1, 1));
	obj->SetPosition(player->GetPosition());
	landing = player->GetLockPos();
	Texture* Lock_ = Texture::Create(ImageManager::Battle, {obj->GetPosition().x,obj->GetPosition().y+1.0f,obj->GetPosition().z
}, { 0.1f,0.1f,0.1f }, { 1,1,1,1 });
	Lock_->SetIsBillboard(true);
	Lock_->TextureCreate();
	Lock_->SetRotation({ 0,0,0 });
	//Lock_->SetColor({ 1.0f,0.2f,0.2f ,0.6f });
	Status.reset(Lock_);
}

void Bullet::OnUpda() {
	XMFLOAT3 pos = obj->GetPosition();
	if (ease) {
		pos.x = Ease(InOut,Quad,frame,pos.x,landing.x);
		pos.y += vel; //+
		vel -= 0.05f;//
		if (pos.y<0.0f) {
			pos.y = 0; 
		}
		pos.z = Ease(InOut,Quad,frame,pos.z,landing.z);
		if (frame<1.0f) {
			frame += 0.02f;
		} else {
			frame = 1.0f;
			pos.y = 0.0f;
			ease = false;
		}
		obj->SetPosition(pos);
	} else {
		if (follow) {
			Follow();
		}
		if (enemy->GetIsActive()) {
			if (Collision::CircleCollision(obj->GetPosition().x, obj->GetPosition().z, 20.0f, enemy->GetPosition().x, enemy->GetPosition().z, 1.0f)) {
				if (!follow) {
					follow=true;
				}
			}
		}
		if(Collision::SphereCollision2(obj->GetPosition(),1.5f, enemy->GetPosition(),1.5f)){
			enemy->SetHp(enemy->GetHp()-1);
			isRemove = true;
		}
		if (Collision::SphereCollision2(obj->GetPosition(), 1.0f, player->GetPosition(), 1.0f)) {
			player->SetStock(player->GetStock()+1);
			isRemove = true;
		}
	}
	Status->Update();
	Status->SetPosition({ obj->GetPosition().x,obj->GetPosition().y + 2.5f,obj->GetPosition().z });
}
void Bullet::Follow() {
	XMFLOAT3 pos = obj->GetPosition();
	XMFLOAT3 rot = obj->GetRotation();
	XMFLOAT3 position{};
	position.x = (enemy->GetPosition().x - pos.x);
	position.z = (enemy->GetPosition().z - pos.z);
	rot.y = (atan2f(position.x, position.z) * (180.0f / XM_PI))-180; //- 90;// *(XM_PI / 180.0f);
	vel_follow.x = sin(-atan2f(position.x, position.z)) * 0.3f;
	vel_follow.y = cos(-atan2f(position.x, position.z)) * 0.3f;
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
