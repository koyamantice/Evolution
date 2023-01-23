#include"ClearCrystal.h"
#include <SourceCode/FrameWork/ActorManager.h>
#include <SourceCode/FrameWork/collision/Collision.h>
#include "ImageManager.h"
#include <SourceCode/Common/Easing.h>

void ClearCrystal::OnInit() {
	obj->SetColor({1,1,1,1});
	obj->SetScale({ 0,0,0 });
	//当たり判定
	collide_size = 3.0f;

	//パーティクルの初期化
	partcleEmitter_ = std::make_unique <ParticleEmitter>(ImageManager::fire);
}

void ClearCrystal::OnUpda() {
	/// <summary>
	/// 出現処理
	/// </summary>
	if(ease_){
		if (ease_frame < 1.0f) {
			ease_frame += 1.0f/kEaseFrameMax;
		} else {
			ease_ = false;
			ease_frame = 1.0f;
		}

		float sca = Ease(InOut,Quint,ease_frame,0.0f,2.0f);

		obj->SetScale({ sca,sca,sca});

		const int life = 45;
		XMFLOAT3 pos = obj->GetPosition();
		float area = Ease(In,Linear,ease_frame,15.0f,1.0f);
		const float rnd_vel = 0.4f;
		partcleEmitter_->AddContraction(life, pos, area, rnd_vel);

	} else {
		//ゆっくり浮かび上がっていく処理
		frame_ += frame_add_;
		if (frame_ >= 1.0f || frame_ <= 0.0f) {
			frame_add_ *= -1.0f;
		}
		float hight = Ease(In, Quad, frame_, 0.5f, 1.5f);
		
		//パーティクルを渡す
		const int life = 45;
		XMFLOAT3 base = obj->GetPosition();
		const float rnd_area = 3.0f;
		const float rnd_vel = 0.4f;
		partcleEmitter_->AddCrystalMove(life, base, rnd_area, rnd_vel);
		//オブジェクト回転します
		stand_radius_++;
		obj->SetRotation(XMFLOAT3(0, stand_radius_, 0));
		obj->SetPosition(XMFLOAT3{ 0.0f, hight,0.0f });
	}

	partcleEmitter_->Update();
}
void ClearCrystal::OnCollision(const std::string& Tag) {
	if (Tag == "Player") {
		pause= true;
	} 

}
void ClearCrystal::OnDraw(DirectXCommon* dxCommon) {
	partcleEmitter_->Draw(addBle);
}

void ClearCrystal::OnFinal() {
}

void ClearCrystal::ResultOnUpdate(const float& Timer) {
	partcleEmitter_->Update();
}
