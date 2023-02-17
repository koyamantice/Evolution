#include"Enemy.h"
#include"ModelManager.h"
#include"ImageManager.h"
#include<fstream>
#include <SourceCode/FrameWork/collision/CollisionManager.h>
#include <SourceCode/FrameWork/collision/SphereCollider.h>
#include <SourceCode/FrameWork/collision/CollisionAttribute.h>
#include <SourceCode/FrameWork/ActorManager.h>


void Enemy::OnInit() {
	collide_size = 3.0f;
	InitCommon(ModelManager::GetIns()->GetFBXModel(ModelManager::kMash), { 0.025f,0.025f, 0.025f });
	fbxObject_->PlayAnimation();
	LoadData("Mash");

	compornent = new EnemyUI();
	compornent->Initialize();

	Attack= std::make_unique <EnemyAttack>(this);
	Attack->Init();
}

void Enemy::OnUpda() {
	//PhaseMove();
	(this->*phaseFuncTable[static_cast<size_t>(phase_)])();

	LifeCommon();

	shadow_->Update();
	shadow_->SetPosition({ fbxObject_->GetPosition().x,0.01f, fbxObject_->GetPosition().z });

	obj->SetPosition(fbxObject_->GetPosition());
	fbxObject_->Update();
	Attack->Upda();
}

void Enemy::OnFirstDraw(DirectXCommon* dxCommon) {
	Object2d::PreDraw();
	shadow_->Draw();
}

void Enemy::OnDraw(DirectXCommon* dxCommon) {
	ImGui::Begin("test");
	ImGui::Text("%f", hp);
	ImGui::Unindent();
	ImGui::End();
	Attack->Draw();
	Object3d::PreDraw();
	fbxObject_->Draw(dxCommon->GetCmdList());
}

void Enemy::OnLastDraw(DirectXCommon* dxCommon) {
}

void Enemy::OnFinal() {
}

void Enemy::OnCollision(const std::string& Tag) {
	if (Tag == "Player") {
		player_->SetHitBound(fbxObject_->GetPosition());
	}
}
void Enemy::PhaseMove() {
	switch (command) {
	case Actor::Phase::UNGUARD:
		UnguardUpda();
		break;
	case Actor::Phase::ATTACK:
		AttackUpda();
		break;
	default:
		break;
	}
}

void Enemy::UnguardUpda() {
	if (fbxObject_->GetIsFinish()) { animecount++; }
	if (animecount >= 2) {
		fbxObject_->StopAnimation();
		waitTimer++;
		Attack->SetPredict(true, waitTimer / kPredictTime);
		if (waitTimer == kPredictTime) {
			Attack->SetPredict(false, 0);
			command = Actor::Phase::ATTACK;
			waitTimer = 0;
			scaframe = 0.0f;
			animecount = 0;
			return;
		}
		if (scaframe < 1.0f) {
			scaframe += 1.0f / (kPredictTime / kScaleCount);
		} else {
			scaframe = 0.0f;
		}
		scale = Ease(In, Quad, scaframe, 25.0f, 16.0f);
		fbxObject_->SetScale({ scale * 0.001f,scale * 0.001f,scale * 0.001f });
	}
}

void Enemy::AttackUpda() {
	XMFLOAT3 pos = fbxObject_->GetPosition();
	waitTimer++;
	if (waitTimer == kAttackTime) {
		pos.y = 0;
		fbxObject_->ResetAnimation();
		fbxObject_->PlayAnimation();
		command = Actor::Phase::UNGUARD;
		waitTimer = 0;
		fbxObject_->SetPosition(pos);
		return;
	}
	if (pos.y >= 0) {
		pos.y += speed;
		speed -= accel;
	} else {
		Attack->Stamp(pos);
		pos.y = 0;
		speed = accel * 30.0f;
	}
	fbxObject_->SetRotation({ 0,-180,0 });
	fbxObject_->SetPosition(pos);
}


void Enemy::LifeCommon() {
	if (hp < 0.0f) {
		if (command != DEAD) {
			pause = true;
			return;
		}
		XMFLOAT3 pos = fbxObject_->GetPosition();
		XMFLOAT3 rot = fbxObject_->GetRotation();
		XMFLOAT3 sca = fbxObject_->GetScale();
		fbxObject_->ResetAnimation();
		rot.y++;
		scale = Ease(In, Quad, scaframe, 25.0f, 0.0f);
		if (scaframe < 1.0f) {
			scaframe += 0.01f;
		} else {
			isActive = false;
		}
		fbxObject_->SetScale({ scale * 0.001f,scale * 0.001f,scale * 0.001f });
		fbxObject_->SetRotation(rot);
	}
}

void Enemy::ChangeCommand(const int& num, const int& command, const int& count) {
	static bool isFirst = true;
	static int MotionCount = 0;
	if (isFirst) {
		fbxObject_->PlayAnimation(num);
		isFirst = false;
	}
	if (fbxObject_->GetIsFinish()) { MotionCount++; }
	if (MotionCount == count) {
		this->command = command;
		fbxObject_->StopAnimation();
		isFirst = true;
		MotionCount = 0;
	}
}

