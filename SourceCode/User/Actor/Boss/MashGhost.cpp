#include"MashGhost.h"
#include"ModelManager.h"
#include"ImageManager.h"
#include <SourceCode/FrameWork/ActorManager.h>


void MashGhost::OnInit() {
	LoadData("Mash");

	collide_size = 3.0f;
	
	InitCommon(ModelManager::GetIns()->GetFBXModel(ModelManager::kMash), { 0.025f,0.025f, 0.025f });
	fbxObject_->PlayAnimation();

	compornent = new EnemyUI();
	compornent->Initialize();

	attack_= std::make_unique<EnemyAttack>(this);
	attack_->Init();
}

void MashGhost::OnUpda() {
	//HP�Ǘ�
	LifeCommon();

	//�֐��|�C���^�ŏ�ԊǗ�
	(this->*phaseFuncTable[static_cast<size_t>(phase_)])();

	ShadowUpdate();
	obj->SetPosition(fbxObject_->GetPosition());
	fbxObject_->Update();
	attack_->Upda();
}

void MashGhost::OnFirstDraw(DirectXCommon* dxCommon) {
	//Object2d::PreDraw();
	//ImGui::SetNextWindowPos(ImVec2(1100, 240));
	//ImGui::SetNextWindowSize(ImVec2(180, 250));

	//ImGui::Begin("enemy");
	//ImGui::Text("nowscale:%f", fbxObject_->GetScale().x);
	//ImGui::Text("smashscale:%f", smash_scale_);

	//ImGui::End();

	if (!isResult) { shadow_->Draw(); }
}

void MashGhost::OnDraw(DirectXCommon* dxCommon) {
	attack_->Draw();
	Object3d::PreDraw();
	fbxObject_->Draw(dxCommon->GetCmdList());
}

void MashGhost::OnLastDraw(DirectXCommon* dxCommon) {
}

void MashGhost::OnFinal() {
	levelData_ = {};
}


void MashGhost::StartAction() {
	if (fbxObject_->GetIsFinish()) { animation_count_++; }
	if (animation_count_ >= 2) {
		animation_count_ = 0;
		fbxObject_->StopAnimation();
		phase_ = E_Phase::kAttackPredict;
	}

}

void MashGhost::AttackPredict() {
	waittimer_++;
	attack_->SetPredict(true, waittimer_ / predictTimeMax_);
	if (waittimer_ >= predictTimeMax_) {
		odd_ = 1;
		attack_->SetPredict(false, 0);
		waittimer_ = 0;
		scale_frame_ = 0.0f;
		phase_ = E_Phase::kPressAttack;
		return;
	}
	//����k�ނ�
	const float kScaleCount = 10.0f;
	if (scale_frame_ <= 1.0f) {
		scale_frame_ += 1.0f / (predictTimeMax_ / kScaleCount);
	} else {
		scale_frame_ = 0.0f;
		odd_++;
	}
	float scale = 0;
	if ((odd_%2)==1) {
		scale = Ease(InOut,Linear, scale_frame_, 25.0f, 35.0f);
		shadow_side_ = Ease(InOut, Linear, scale_frame_, 1.5f, 2.5f);
	} else {
		scale = Ease(InOut, Linear, scale_frame_, 35.0f, 25.0f);
		shadow_side_ = Ease(InOut, Linear, scale_frame_, 2.5f, 1.5f);

	}
	fbxObject_->SetScale({ scale * 0.001f,scale * 0.001f,scale * 0.001f });
}

void MashGhost::JumpAttack() {
	waittimer_++;
	if (waittimer_ >= (attackTimeMax_/4.0f)) {
		speed = 0;
		waittimer_ = 0;
		phase_ = E_Phase::kPressAttack;
	}
	float frame = waittimer_/(attackTimeMax_ / 4.0f);
	float position_y_= Ease(In, Linear, frame, 0.0f, 3.0f);
	XMFLOAT3 pos= fbxObject_->GetPosition();
	fbxObject_->SetPosition({pos.x,position_y_,pos.z});
}

void MashGhost::PressAttack() {
	XMFLOAT3 pos = fbxObject_->GetPosition();

	if (pos.y >= 0) {
		pos.y += speed;
		speed -= accel;
	} else {
		attack_->Stamp(pos);
		pos.y = 0;
		speed = accel * 30.0f;
		stamp_count_++;
	}
	if (stamp_count_ >= 3) {
		stamp_count_ = 0;
		pos.y = 0;
		fbxObject_->SetPosition(pos);
		fbxObject_->ResetAnimation();
		fbxObject_->PlayAnimation();
		phase_ = E_Phase::kStartAction;
		return;
	}
	fbxObject_->SetPosition(pos);
}

void MashGhost::StopMotion() {

	shadow_->Update();
	shadow_->SetPosition({ fbxObject_->GetPosition().x,0.01f, fbxObject_->GetPosition().z });
	fbxObject_->StopAnimation();
	fbxObject_->Update();
	attack_->SetPredict(false,0);
	attack_->Upda();
}


