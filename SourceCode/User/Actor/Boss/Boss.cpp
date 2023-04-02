#include "Boss.h"
#include <ActorManager.h>


void (Boss::*Boss::phaseFuncTable[])() = {
	&Boss::StartAction,//要素0
	&Boss::AttackPredict, //要素1
	&Boss::JumpAttack,
	&Boss::PressAttack,
	&Boss::ChasePlayer,
	&Boss::FeedHoney,
	&Boss::DeadMotion,
	&Boss::StopMotion,
	&Boss::PinchMotion,
};


void Boss::LoadData(const std::string& bossname) {
	BossLevelLoader& gFoo = Singleton<BossLevelLoader>::get_instance();
	//名前からステータスを取得
	levelData_= gFoo.takeData(bossname);
	//体力
	hp = levelData_.hp;
	max_hp = hp;
	pinchLife = max_hp - 10.0f;
	//速度
	vel_ = levelData_.vel;
	//初期スケール
	float scale = levelData_.scale;
	baseScale_ = { scale,scale,scale };
	//クールタイム
	coolTimeMax_ = levelData_.coolTimeMax;
	//攻撃準備時間
	predictTimeMax_ = levelData_.predictTimeMax;
	//攻撃時間
	attackTimeMax_ = levelData_.attackTimeMax;


}

void Boss::ResultOnUpdate(const float& timer) {
	isResult_ = true;
	fbxObject_->Update();
	ShadowUpdate();
}

void Boss::InitCommon(FBXModel* model,XMFLOAT3 scale, XMFLOAT3 rotation) {
	//アクターのobjの非表示
	isVisible = false;
	//fbxモデル生成
	fbxObject_= std::make_unique<FBXObject3d>();
	fbxObject_->Initialize(model, scale, rotation);
	fbxObject_->LoadAnimation();
	//丸影
	shadow_ = Object2d::Create(ImageManager::shadow_, { 0,0,0 },
		{ 0.7f,0.7f,0.7f }, { 1,1,1,1 });
	shadow_->SetRotation({ 90,0,0 });
	//パーティクル
	particle_ = std::make_unique<ParticleEmitter>(ImageManager::nul);
}

void Boss::LifeCommon() {
	for (; damage > 0; damage--) {
		if (hp <= pinchLife) { 
			hp = pinchLife;
			pinchLife -= 10.0f;
			damage = 0;
			smash_rot_= fbxObject_->GetRotation().y;
			old_phase_ = phase_;
			phase_=E_Phase::kPinchMotion;
		} else {
			hp--;
		}
	}


	if (hp <= 0.0f) {
		if (!canMove && phase_!= E_Phase::kDeadMotion) {
			smash_scale_= fbxObject_->GetScale().x * 1000.0f;
			smash_shadow_ = shadow_side_;
			smash_rot_ = fbxObject_->GetRotation().y;
			phase_ = E_Phase::kDeadMotion;
			return;
		}
		if (!pause) {
			pause = true;
			phase_ = E_Phase::kStopMotion;
			return;
		}
	}
}

void Boss::ShadowUpdate() {
	XMFLOAT3 pos = fbxObject_->GetPosition();
	float scale = ((5.0f - pos.y) / 5.0f) * shadow_side_;
	scale =max(0.5f,scale);

	shadow_->SetScale({scale,scale, scale,});
	shadow_->Update();
	shadow_->SetPosition({ fbxObject_->GetPosition().x,0.01f, fbxObject_->GetPosition().z });
}


void Boss::DeadMotion() {
	XMFLOAT3 pos = fbxObject_->GetPosition();
	XMFLOAT3 rot = fbxObject_->GetRotation();
	XMFLOAT3 sca = fbxObject_->GetScale();
	fbxObject_->ResetAnimation();
	rot.y = Ease(In, Linear, scale_frame_, smash_rot_, smash_rot_+(DEGREE_MAX * 5.0f));
	float scale = Ease(In, Linear, scale_frame_, smash_scale_ , 0.0f);
	shadow_side_ = Ease(In, Linear, scale_frame_, smash_shadow_, 0.0f);
	if (scale_frame_ < 1.0f) {
		scale_frame_ += 1.0f / 100.0f;
	} else {
		isActive = false;
	}
	fbxObject_->SetScale({ scale * 0.001f,scale * 0.001f,scale * 0.001f });
	obj->SetScale(fbxObject_->GetScale());
	fbxObject_->SetRotation(rot);
	ShadowUpdate();
}

void Boss::PinchMotion() {
	if (scale_frame_ < 1.0f) {
		scale_frame_ += 1.0f / 50.0f;
	} else {
		scale_frame_ = 0.0f;
		phase_ = old_phase_;
	}
	XMFLOAT3 rot = fbxObject_->GetRotation();
	rot.y = Ease(In, Linear, scale_frame_, smash_rot_, smash_rot_ + (DEGREE_MAX * 3.0f));
	fbxObject_->SetRotation(rot);
	SpecialPinch();
	ActorManager::GetInstance()->ChangeStatus(Bullet::BulletStatus::Attack, Bullet::BulletStatus::Vanish);
}

