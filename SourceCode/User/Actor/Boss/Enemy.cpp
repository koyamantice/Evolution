#include"Enemy.h"
#include"ModelManager.h"
#include"ImageManager.h"
#include<fstream>
#include <SourceCode/FrameWork/collision/CollisionManager.h>
#include <SourceCode/FrameWork/collision/SphereCollider.h>
#include <SourceCode/FrameWork/collision/CollisionAttribute.h>
#include <SourceCode/FrameWork/ActorManager.h>


void Enemy::OnInit() {
	LoadData("Mash");

	collide_size = 3.0f;
	
	InitCommon(ModelManager::GetIns()->GetFBXModel(ModelManager::kMash), { 0.025f,0.025f, 0.025f });
	fbxObject_->PlayAnimation();

	compornent = new EnemyUI();
	compornent->Initialize();

	attack_= std::make_unique<EnemyAttack>(this);
	attack_->Init();


	//�����o�֐��ƌĂяo������bind��std::function�ɑ��
	std::function<void()> callback = std::bind(&Enemy::CommandChange, this);
	//���������C�x���g�𐶐�
	std::unique_ptr<TimedInvoke> timedCall = std::make_unique<TimedInvoke>(callback, (uint16_t)cooltime_);
	//���������C�x���g���X�g�ɒǉ�
	timedInvokes_.push_back(std::move(timedCall));

}

void Enemy::OnUpda() {
	//�֐��|�C���^�ŏ�ԊǗ�
	(this->*phaseFuncTable[static_cast<size_t>(phase_)])();

	//�I�������^�C�}�[���폜
	timedInvokes_.remove_if([](std::unique_ptr<TimedInvoke>& _timedInvoke) {
		return _timedInvoke->GetIsFinish();
	});
	//�͈�for�Ń��X�g�̑S�v�f�ɂ��ĉ�
	for (std::unique_ptr<TimedInvoke>& timedInvoke :timedInvokes_) {
		timedInvoke->Update();
	}

	LifeCommon();

	shadow_->Update();
	shadow_->SetPosition({ fbxObject_->GetPosition().x,0.01f, fbxObject_->GetPosition().z });

	obj->SetPosition(fbxObject_->GetPosition());
	fbxObject_->Update();
	attack_->Upda();
}

void Enemy::OnFirstDraw(DirectXCommon* dxCommon) {
	Object2d::PreDraw();
	shadow_->Draw();
}

void Enemy::OnDraw(DirectXCommon* dxCommon) {
	attack_->Draw();
	Object3d::PreDraw();
	fbxObject_->Draw(dxCommon->GetCmdList());
}

void Enemy::OnLastDraw(DirectXCommon* dxCommon) {
}

void Enemy::OnFinal() {
	levelData_ = {};
}

void Enemy::OnCollision(const std::string& Tag) {
	if (Tag=="Bullet") {
		int a = 0;
		a++;
	}
}

void Enemy::StartAction() {
	if (fbxObject_->GetIsFinish()) { animation_count_++; }
	if (animation_count_ >= 2) {
		fbxObject_->StopAnimation();
		animation_count_ = 0;
		phase_ = E_Phase::kAttackPredict;
	}

}

void Enemy::AttackPredict() {
	waittimer_++;
	attack_->SetPredict(true, waittimer_ / kPredictTime);
	if (waittimer_ >= kPredictTime) {
		attack_->SetPredict(false, 0);
		waittimer_ = 0;
		scale_frame_ = 0.0f;
		phase_ = E_Phase::kPressAttack;
		command = Actor::Phase::ATTACK;
		return;
	}
	//����k�ނ�
	const float kScaleCount = 4.0f;
	if (scale_frame_ < 1.0f) {
		scale_frame_ += 1.0f / (kPredictTime / kScaleCount);
	} else {
		scale_frame_ = 0.0f;
	}
	float scale = Ease(In, Quad, scale_frame_, 25.0f, 15.0f);
	fbxObject_->SetScale({ scale * 0.001f,scale * 0.001f,scale * 0.001f });
}

void Enemy::PressAttack() {
	XMFLOAT3 pos = fbxObject_->GetPosition();
	waittimer_++;
	if (waittimer_ >= kAttackTime) {
		pos.y = 0;
		fbxObject_->SetPosition(pos);
		fbxObject_->ResetAnimation();
		fbxObject_->PlayAnimation();
		phase_ = E_Phase::kStartAction;
		waittimer_ = 0;
		return;
	}
	if (pos.y >= 0) {
		pos.y += speed;
		speed -= accel;
	} else {
		attack_->Stamp(pos);
		pos.y = 0;
		speed = accel * 30.0f;
	}
	//fbxObject_->SetRotation({ 0,-180,0 });
	fbxObject_->SetPosition(pos);
}


