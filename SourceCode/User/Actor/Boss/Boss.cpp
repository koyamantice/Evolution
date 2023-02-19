#include "Boss.h"
#include <ActorManager.h>


void (Boss::*Boss::phaseFuncTable[])() = {
	&Boss::StartAction,//�v�f0
	&Boss::AttackPredict, //�v�f1
	&Boss::PressAttack,
	&Boss::ChasePlayer,
	&Boss::FeedHoney,
	&Boss::DeadMotion
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
	cooltime_ = levelData_.coolTime;
}

void Boss::InitCommon(FBXModel* _model,XMFLOAT3 _scale, XMFLOAT3 _rotation) {
	//�A�N�^�[��obj�̔�\��
	isVisible = false;
	//fbx���f������
	fbxObject_= std::make_unique<FBXObject3d>();
	fbxObject_->Initialize(_model, _scale, _rotation);
	fbxObject_->LoadAnimation();
	//�ۉe
	Object2d* Shadow_ = Object2d::Create(ImageManager::Shadow, { 0,0,0 },
		{ 0.7f,0.7f,0.7f }, { 1,1,1,1 });
	Shadow_->SetRotation({ 90,0,0 });
	shadow_.reset(Shadow_);
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
			return;
		}
	}
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
	fbxObject_->SetRotation(rot);

}
