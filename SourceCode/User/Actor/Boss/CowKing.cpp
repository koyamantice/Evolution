#include"CowKing.h"
#include"ModelManager.h"
#include"ImageManager.h"
#include <SourceCode/FrameWork/ActorManager.h>


void CowKing::OnInitialize() {
	LoadData("Cow");

	collide_size = 3.0f;
	
	InitCommon(ModelManager::GetIns()->GetFBXModel(ModelManager::kCow), { 0.025f,0.025f, 0.025f });
	//fbxObject_->PlayAnimation();

	compornent = new EnemyUI();
	compornent->Initialize();

	attack_= std::make_unique<EnemyAttack>(this);
	attack_->Init();

	player_ = ActorManager::GetInstance()->SearchActor("Player");

}

void CowKing::OnUpdate() {
	//HPŠÇ—
	LifeCommon();

	//ŠÖ”ƒ|ƒCƒ“ƒ^‚Åó‘ÔŠÇ—
	(this->*phaseFuncTable[static_cast<size_t>(phase_)])();

	shadow_->Update();
	shadow_->SetPosition({ fbxObject_->GetPosition().x,0.01f, fbxObject_->GetPosition().z });

	obj->SetPosition(fbxObject_->GetPosition());
	fbxObject_->Update();
	attack_->Upda();
}

void CowKing::OnFirstDraw(DirectXCommon* dxCommon) {
	Object2d::PreDraw();
	shadow_->Draw();
}

void CowKing::OnDraw(DirectXCommon* dxCommon) {
	attack_->Draw();
	Object3d::PreDraw();
	fbxObject_->Draw(dxCommon->GetCmdList());
}

void CowKing::OnLastDraw(DirectXCommon* dxCommon) {
}

void CowKing::OnFinalize() {
	levelData_ = {};
}


void CowKing::StartAction() {
	waittimer_++;
	XMFLOAT3 rot{};
	rot.y = DirRotation(player_->GetPosition());
	fbxObject_->SetRotation(rot);

	if (waittimer_>= 100) {
		waittimer_ = 0;
		phase_ = E_Phase::kAttackPredict;
	}

}

void CowKing::AttackPredict() {
	predictTimeMax_ = 180;
	waittimer_++;
	XMFLOAT3 rot{};
	rot.y = DirRotation({ 40.0f ,0,40.0f });
	fbxObject_->SetRotation(rot);

	if (waittimer_ >= predictTimeMax_) {
		waittimer_ = 0;
		scale_frame_ = 0.0f;
		phase_ = E_Phase::kPressAttack;
		return;
	}
	scale_frame_ = waittimer_ / predictTimeMax_;
	float scale = Ease(InOut, Elastic, scale_frame_, 0.0f, 40.0f);
	fbxObject_->SetPosition({scale,0,scale});

}

void CowKing::PressAttack() {
	predictTimeMax_ = 180;
	waittimer_++;
	XMFLOAT3 rot{};
	rot.y = DirRotation({ -40.0f ,0,-40.0f });
	fbxObject_->SetRotation(rot);

	if (waittimer_ >= predictTimeMax_) {
		waittimer_ = 0;
		scale_frame_ = 0.0f;
		phase_ = E_Phase::kChasePlayer;
		return;
	}
	scale_frame_ = waittimer_ / predictTimeMax_;
	float scale = Ease(InOut, Elastic, scale_frame_, 40.0f, -40.0f);
	fbxObject_->SetPosition({ scale,0,scale });

}

void CowKing::ChasePlayer() {
	predictTimeMax_ = 180;
	waittimer_++;
	XMFLOAT3 rot{};
	rot.y = DirRotation({ 0.0f ,0,0.0f });
	fbxObject_->SetRotation(rot);

	if (waittimer_ >= predictTimeMax_) {
		waittimer_ = 0;
		scale_frame_ = 0.0f;
		phase_ = E_Phase::kStartAction;
		return;
	}
	scale_frame_ = waittimer_ / predictTimeMax_;
	float scale = Ease(InOut, Elastic, scale_frame_, -40.0f, 0.0f);
	fbxObject_->SetPosition({ scale,0,scale });


}


float CowKing::DirRotation(const XMFLOAT3& target) {
	float itr{};
	XMFLOAT3 pos = fbxObject_->GetPosition();
	XMFLOAT3 position{};

	position.x = (target.x - pos.x);
	position.z = (target.z - pos.z);
	itr = (atan2f(position.x, position.z) * (DEGREE_HALF / XM_PI))+DEGREE_HALF;

	return itr;
}