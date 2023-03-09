#include"CowKing.h"
#include"ModelManager.h"
#include"ImageManager.h"
#include <SourceCode/FrameWork/ActorManager.h>


void CowKing::OnInit() {
	LoadData("Cow");

	collide_size = 3.0f;
	
	InitCommon(ModelManager::GetIns()->GetFBXModel(ModelManager::kCow), { 0.025f,0.025f, 0.025f });
	//fbxObject_->PlayAnimation();

	compornent = new EnemyUI();
	compornent->Initialize();

	attack_= std::make_unique<EnemyAttack>(this);
	attack_->Init();


}

void CowKing::OnUpda() {
	//HPä«óù
	LifeCommon();

	//ä÷êîÉ|ÉCÉìÉ^Ç≈èÛë‘ä«óù
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

void CowKing::OnFinal() {
	levelData_ = {};
}


void CowKing::StartAction() {
	if (fbxObject_->GetIsFinish()) { animation_count_++; }
	if (animation_count_ >= 2) {
		fbxObject_->StopAnimation();
		animation_count_ = 0;
		phase_ = E_Phase::kAttackPredict;
	}

}

void CowKing::AttackPredict() {
	waittimer_++;
	attack_->SetPredict(true, waittimer_ / predictTimeMax_);
	if (waittimer_ >= predictTimeMax_) {
		attack_->SetPredict(false, 0);
		waittimer_ = 0;
		scale_frame_ = 0.0f;
		phase_ = E_Phase::kPressAttack;
		return;
	}
	//âΩâÒèkÇﬁÇ©
	const float kScaleCount = 4.0f;
	if (scale_frame_ < 1.0f) {
		scale_frame_ += 1.0f / (predictTimeMax_ / kScaleCount);
	} else {
		scale_frame_ = 0.0f;
	}
	float scale = Ease(In, Quad, scale_frame_, 25.0f, 15.0f);
	fbxObject_->SetScale({ scale * 0.001f,scale * 0.001f,scale * 0.001f });
}

void CowKing::PressAttack() {
	XMFLOAT3 pos = fbxObject_->GetPosition();
	waittimer_++;
	if (waittimer_ >= attackTimeMax_) {
		pos.y = 0;
		fbxObject_->SetPosition(pos);
		//fbxObject_->ResetAnimation();
		//fbxObject_->PlayAnimation();
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


