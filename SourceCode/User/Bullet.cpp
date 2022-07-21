#include "Bullet.h"
#include "Easing.h"
#include"ActorManager.h"
using namespace DirectX;



Bullet::Bullet() {

	ease = true;
}

void Bullet::OnInit() {
	enemy = ActorManager::GetInstance()->SearchActor("Enemy");

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
	}
}
void Bullet::Follow() {
	XMFLOAT3 pos = obj->GetPosition();
	XMFLOAT3 position{};
	position.x = (enemy->GetPosition().x - pos.x);
	position.z = (enemy->GetPosition().z - pos.z);
	//rot.y = (atan2f(position.x, position.z) * (180.0f / XM_PI)) - 90;// *(XM_PI / 180.0f);
	vel_follow.x = sin(-atan2f(position.x, position.z)) * 0.2251f;
	vel_follow.y = cos(-atan2f(position.x, position.z)) * 0.2251f;
	pos.x -= vel_follow.x;
	pos.z += vel_follow.y;
	obj->SetPosition(pos);

}


void Bullet::OnDraw() {
}

void Bullet::OnFinal() {
}
