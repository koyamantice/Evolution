#include "Boss.h"
#include <ActorManager.h>


void (Boss::*Boss::phaseFuncTable[])() = {
	&Boss::StartAction,//�v�f0
	&Boss::AttackPredict, //�v�f1
	&Boss::PressAttack
};

void Boss::CommandChange() {
	if (command == UNGUARD) {
		command = ATTACK;
	} else {
		command = UNGUARD;
	}
}



void Boss::LoadData(const std::string& _bossname) {
	BossLevelLoader& gFoo = Singleton<BossLevelLoader>::get_instance();
	//���O����X�e�[�^�X���擾
	levelData_= gFoo.takeData(_bossname);
	//�̗�
	hp = levelData_.hp;
	max_hp = hp;
	//���x
	vel_ = levelData_.vel;
	//
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
		if (!pause) {
			pause = true;
			return;
		}
		XMFLOAT3 pos = fbxObject_->GetPosition();
		XMFLOAT3 rot = fbxObject_->GetRotation();
		XMFLOAT3 sca = fbxObject_->GetScale();
		fbxObject_->ResetAnimation();
		rot.y++;
		scale = Ease(In, Quad, scale_frame_, baseScale_.x * 1000.0f, 0.0f);
		if (scale_frame_ < 1.0f) {
			scale_frame_ += 0.01f;
		} else {
			isActive = false;
		}
		fbxObject_->SetScale({ scale * 0.001f,scale * 0.001f,scale * 0.001f });
		fbxObject_->SetRotation(rot);
	}
}