#include "Boss.h"
#include <ActorManager.h>


void (Boss::*Boss::phaseFuncTable[])() = {
	&Boss::StartAction,//�v�f0
	&Boss::AttackPredict, //�v�f1
	&Boss::JumpAttack,
	&Boss::PressAttack,
	&Boss::ChasePlayer,
	&Boss::FeedHoney,
	&Boss::DeadMotion,
	&Boss::StopMotion
};


void Boss::LoadData(const std::string& _bossname) {
	BossLevelLoader& gFoo = Singleton<BossLevelLoader>::get_instance();
	//���O����X�e�[�^�X���擾
	levelData_= gFoo.takeData(_bossname);
	//�̗�
	hp = levelData_.hp;
	max_hp = hp;
	//���x
	vel_ = levelData_.vel;
	//�����X�P�[��
	float scale = levelData_.scale;
	baseScale_ = { scale,scale,scale };
	//�N�[���^�C��
	coolTimeMax_ = levelData_.coolTimeMax;
	//�U����������
	predictTimeMax_ = levelData_.predictTimeMax;
	//�U������
	attackTimeMax_ = levelData_.attackTimeMax;


}

void Boss::ResultOnUpdate(const float& Timer) {
	isResult = true;
	fbxObject_->Update();
	ShadowUpdate();
}

void Boss::InitCommon(FBXModel* _model,XMFLOAT3 _scale, XMFLOAT3 _rotation) {
	//�A�N�^�[��obj�̔�\��
	isVisible = false;
	//fbx���f������
	fbxObject_= std::make_unique<FBXObject3d>();
	fbxObject_->Initialize(_model, _scale, _rotation);
	fbxObject_->LoadAnimation();
	//�ۉe
	shadow_ = Object2d::Create(ImageManager::Shadow, { 0,0,0 },
		{ 0.7f,0.7f,0.7f }, { 1,1,1,1 });
	shadow_->SetRotation({ 90,0,0 });
	//�p�[�e�B�N��
	particle_ = std::make_unique<ParticleEmitter>(ImageManager::nul);
}

void Boss::LifeCommon() {
	if (hp <= 0.0f) {
		if (!canMove) {
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
	rot.y++;
	float scale = Ease(In, Linear, scale_frame_, baseScale_.x, 0.0f);
	if (scale_frame_ < 1.0f) {
		scale_frame_ += 1.0f / 100.0f;
	} else {
		isActive = false;
	}
	fbxObject_->SetScale({ scale,scale,scale });
	obj->SetScale({ scale,scale,scale });
	fbxObject_->SetRotation(rot);

}

