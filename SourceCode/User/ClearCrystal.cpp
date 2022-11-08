#include"ClearCrystal.h"
#include <SourceCode/FrameWork/ActorManager.h>
#include <SourceCode/FrameWork/collision/Collision.h>
#include "ImageManager.h"
#include <SourceCode/Common/Easing.h>

void ClearCrystal::OnInit() {
	obj->SetColor({1,1,1,0});
	//player = ActorManager::GetInstance()->SearchActor("Player");
	Texture* InOut_ = Texture::Create(ImageManager::InOut, { 0,3,0 }, { 0.5f,0.3f,0.5f }, { 1,1,1,1 });
	InOut_->TextureCreate();
	InOut_->SetRotation({ 0,0,0 });
	InOut_->SetColor({ 1.0f,1.0f,1.0f ,1.0f });
	in_out.reset(InOut_);

}

void ClearCrystal::OnUpda() {
	obj->SetRotation(XMFLOAT3(0, rad, 0));
	rad++;
	frame += a;

	if (frame > 1.0f || frame < 0.0f) {
		a *= -1;
	}
	hight = Ease(In, Quad, frame, 0.5f, 1.5f);
	if (alpha<1.0f) {
		alpha += 0.1f;
	}
	obj->SetColor({1,1,1,alpha});
	obj->SetPosition(XMFLOAT3{ 0.0f, hight,0.0f });
	if(isCollide){
		if (Input::GetInstance()->TriggerButton(Input::A)) {
			pause = true;
		}
	}
	if (pause) {
	}
	in_out->Update();
}
void ClearCrystal::OnCollision(const std::string& Tag) {
	if (Tag == "Player") {
		if (input->TriggerButton(Input::A)) {
			pause= true;
		}
	} else {
		isCollide = false;
	}

}
void ClearCrystal::OnDraw(DirectXCommon* dxCommon) {
}

void ClearCrystal::OnFinal() {
}

void ClearCrystal::ResultOnUpdate(const int& Timer) {
	XMFLOAT3 pos = obj->GetPosition();
	XMFLOAT3 sca = obj->GetScale();
	if (Timer < 50) {
		pos.y += 0.1f;
	}else if (Timer < 150) {
		if (sca.x > 0.0f) {
			sca.x -= 0.01f;
			sca.y -= 0.01f;
			sca.z -= 0.01f;
			const float rnd_vel = 0.1f;
			XMFLOAT3 vel{};
			vel.x = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
			vel.y = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
			vel.z = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
			ParticleManager::GetInstance()->Add(120, pos, vel, XMFLOAT3(), 1.2f, 0.0f);

		}
	}


	obj->SetPosition(pos);
	obj->SetScale(sca);

}
