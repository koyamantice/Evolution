#include"Touch.h"
#include"ModelManager.h"
#include "ImageManager.h"
#include <random>



Touch::Touch(const XMFLOAT3& pos, const XMFLOAT3& rot) {
	Initialize(pos,rot);
}

Touch::~Touch() {
}

void Touch::Initialize(const XMFLOAT3& pos, const XMFLOAT3& rot) {
	Object3d* touch_obj_ = new Object3d();
	touch_obj_->SetModel(ModelManager::GetInstance()->GetModel(ModelManager::kTouch));
	touch_obj_->Initialize();
	touch_obj_->SetPosition(pos);
	touch_obj_->SetRotation(rot);
	touch_obj_->SetScale({ 5,5,5 });
	touch_obj.reset(touch_obj_);

	ParticleManager* fire_ = new ParticleManager();
	fire_->Initialize(ImageManager::fire);
	fire.reset(fire_);
}

void Touch::SetColor(const int color) {
	if (color==f_green) {
		s_color={ 0.0f,1.0f,0.0f,1.0f };
		e_color={ 0.3f,1.0f,0.3f,1.0f };
	}
	if (color == f_blue) {
		s_color= { 0.0f,0.0f,1.0f,1.0f };
		e_color= { 0.3f,0.3f,1.0f,1.0f };
	}
}

void Touch::Update() {
	touch_obj->Update();
	FireAdd();
}

void Touch::Draw() {
	Object3d::PreDraw();
	touch_obj->Draw();

	fire->Draw(addBle);
}

void Touch::FireAdd() {
	XMFLOAT3 pos = touch_obj->GetPosition();
	const float rnd_vel = 0.05f;
	std::mt19937 mt{ std::random_device{}() };
	std::uniform_real_distribution<float> rad(0, 1.0f);
	XMFLOAT3 vel = {
	vel.x = rad(mt) * rnd_vel - rnd_vel / 2.0f,
	vel.y = rad(mt) * rnd_vel * 2.0f,
	vel.z = rad(mt) * rnd_vel - rnd_vel / 2.0f
	};
	fire->Add(20, { pos.x,pos.y + 3.0f,pos.z }, vel, {}, 1.0f, 0.0f, s_color, e_color);

	fire->Update();
}
