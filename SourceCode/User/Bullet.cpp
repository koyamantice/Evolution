#include "Bullet.h"
#include "Easing.h"
#include"ActorManager.h"



Bullet::Bullet() {
	ease = true;
}

void Bullet::OnInit() {
	//ActorManager::GetInstance()->DettachList();
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

	}
}

void Bullet::OnDraw() {
}

void Bullet::OnFinal() {
}
