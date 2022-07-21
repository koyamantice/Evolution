#include "Bullet.h"
#include "Easing.h"
#include"ActorManager.h"
#include <SourceCode/FrameWork/collision/Collision.h>
using namespace DirectX;



Bullet::Bullet() {

	ease = true;
}

void Bullet::OnInit() {
	player = ActorManager::GetInstance()->SearchActor("Player");
	enemy = ActorManager::GetInstance()->SearchActor("Enemy");
	obj->SetScale(XMFLOAT3(0.5,0.5,0.5));
	obj->SetPosition(player->GetPosition());
	landing = player->GetLockPos();
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
		Follow();
		if(Collision::SphereCollision2(obj->GetPosition(),1.0f, enemy->GetPosition(),1.0f)){
			isRemove = true;
		}
	}
}
void Bullet::Follow() {
	XMFLOAT3 pos = obj->GetPosition();
	XMFLOAT3 position{};
	position.x = (enemy->GetPosition().x - pos.x);
	position.z = (enemy->GetPosition().z - pos.z);
	//rot.y = (atan2f(position.x, position.z) * (180.0f / XM_PI)) - 90;// *(XM_PI / 180.0f);
	vel_follow.x = sin(-atan2f(position.x, position.z)) * 0.3f;
	vel_follow.y = cos(-atan2f(position.x, position.z)) * 0.3f;
	pos.x -= vel_follow.x;
	pos.z += vel_follow.y;
	obj->SetPosition(pos);

}


void Bullet::OnDraw() {
}

void Bullet::OnFinal() {
}
