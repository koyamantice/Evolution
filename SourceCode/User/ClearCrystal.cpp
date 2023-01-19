#include"ClearCrystal.h"
#include <SourceCode/FrameWork/ActorManager.h>
#include <SourceCode/FrameWork/collision/Collision.h>
#include "ImageManager.h"
#include <SourceCode/Common/Easing.h>

void ClearCrystal::OnInit() {
	obj->SetColor({1,1,1,0});
	obj->SetScale({ 0,0,0 });



	partMan = new ParticleManager();
	partMan->Initialize(ImageManager::fire);

}

void ClearCrystal::OnUpda() {


	if(ease){
		if (ease_frame < 1.0f) {
			ease_frame += 0.0066f;
		} else {
			ease = false;
			ease_frame = 1.0f;
		}

		float sca = Ease(InOut,Quint,ease_frame,0.0f,2.0f);

		obj->SetScale({ sca,sca,sca});

		XMFLOAT3 base=obj->GetPosition();
		const float rnd_rad = 360.0f;
		XMFLOAT3 pos{};
		float angle = (float)rand() / RAND_MAX * rnd_rad;
		float area = Ease(In,Linear,ease_frame,15.0f,1.0f);
		pos.x = base.x + (area + 0.5f) * sinf(angle);
		pos.z = base.z + (area + 0.5f) * cosf(angle);
		const float rnd_vel = 0.4f;
		XMFLOAT3 vel{};
		vel.y = (float)rand() / RAND_MAX * rnd_vel;
		partMan->Add(45, pos, vel, {}, 1.5f, 0.0f, { 0.5f,1.0f,0.5f,1.0f }, { 0.5f,1.0f,0.5f,1.0f });

	} else {
		rad++;
		frame += a;
		if (frame > 1.0f || frame < 0.0f) {
			a *= -1;
		}
		hight = Ease(In, Quad, frame, 0.5f, 1.5f);

		if (alpha < 1.0f) {
			alpha += 0.1f;
		}




		XMFLOAT3 base = obj->GetPosition();
		const float rnd_rad = 360.0f;
		XMFLOAT3 pos{};
		float angle = (float)rand() / RAND_MAX * rnd_rad;
		const float rnd_area = 3.0f;
		float area= (float)rand() / RAND_MAX * rnd_area;
		pos.x = base.x + area * sinf(angle);
		pos.z = base.z + area * cosf(angle);
		const float rnd_vel = 0.4f;
		XMFLOAT3 vel{};
		vel.y = (float)rand() / RAND_MAX * rnd_vel;
		partMan->Add(45, pos, vel, {}, 1.5f, 0.0f, { 0.5f,1.0f,0.5f,1.0f }, { 0.5f,1.0f,0.5f,1.0f });

		obj->SetRotation(XMFLOAT3(0, rad, 0));
		obj->SetColor({ 1,1,1,alpha });
		obj->SetPosition(XMFLOAT3{ 0.0f, hight,0.0f });
	}

	partMan->Update();
}
void ClearCrystal::OnCollision(const std::string& Tag) {
	if (Tag == "Player") {
		pause= true;
	} 

}
void ClearCrystal::OnDraw(DirectXCommon* dxCommon) {
	partMan->Draw(addBle);
}

void ClearCrystal::OnFinal() {
}

void ClearCrystal::ResultOnUpdate(const float& Timer) {

	partMan->Update();

}
