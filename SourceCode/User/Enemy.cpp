#include"Enemy.h"
#include"ModelManager.h"
#include"ImageManager.h"
#include<fstream>
#include <SourceCode/FrameWork/collision/CollisionManager.h>
#include <SourceCode/FrameWork/collision/SphereCollider.h>
#include <SourceCode/FrameWork/collision/CollisionAttribute.h>
#include <SourceCode/FrameWork/ActorManager.h>
#include <SourceCode/Common/Easing.h>

void Enemy::LoadData() {
	std::ifstream file;
	file.open("Resources/csv/EnemyStatus.csv");
	assert(file.is_open());

	parameterCommands << file.rdbuf();
	
	file.close();
}

void Enemy::UpdateCommand() {
	std::string line;

	while (getline(parameterCommands,line)) {
		//解析しやすくする
		std::istringstream line_stream(line);

		std::string word;
		//半角スペース区切りで行の先頭文字列を取得
		getline(line_stream,word,',');
		//"//"から始まる行はコメント
		if (word.find("//")==0) {
			//飛ばす
			continue;
		}
		//各コマンド
		if (word.find("HP")==0) {
			getline(line_stream, word, ',');
			hp = (float)std::atof(word.c_str());
		}else if(word.find("VEL")==0) {
			getline(line_stream, word, ',');
			vel = (float)std::atof(word.c_str());
		} else if (word.find("PHASE") == 0) {
			getline(line_stream, word, ',');
			if (word.find("APPROCH") == 0) {
				command = Actor::Phase::APPROCH;
			}else if (word.find("LEAVE") == 0) {
				command = Actor::Phase::LEAVE;
			}else if (word.find("WAIT") == 0) {
				command = Actor::Phase::WAIT;
			} else {
				assert(0);//フェーズを書こうな
			}
		} else if (word.find("WAITTIMER") == 0) {
			getline(line_stream, word, ',');
			waitTimer = (float)std::atof(word.c_str());
			break;
		}
	}
}

void Enemy::DebugUpdate() {
	fbxObject3d->Update();
	fbxObject3d->SetPosition({0,-3,0});
}

void Enemy::OnInit() {
	isVisible = false;
	collide_size = 3.0f;

	FBXObject3d* Mash_= new FBXObject3d();
	Mash_->Initialize();
	Mash_->SetModel(ModelManager::GetIns()->GetFBXModel(ModelManager::kMash));
	//Mash_->SetPosition({ 0.01f,0.0f, 0.01f });
	Mash_->SetScale({0.025f,0.025f, 0.025f});
	Mash_->LoadAnimation();

	fbxObject3d.reset(Mash_);
	fbxObject3d->PlayAnimation();

	LoadData();
	UpdateCommand();

	command = Actor::Phase::UNGUARD;
	player = ActorManager::GetInstance()->SearchActor("Player");

	compornent = new EnemyUI();
	compornent->Initialize();

	EnemyAttack* Attack_ = new EnemyAttack();
	Attack_->Init();
	Attack.reset(Attack_);

	Object2d* Shadow_ = Object2d::Create(ImageManager::Shadow, { 0,0,0 },
		{ 0.8f,0.8f,0.8f }, { 1,1,1,1 });
	Shadow_->Object2dCreate();
	Shadow_->SetRotation({ 90,0,0 });
	Shadow.reset(Shadow_);

	partMan = new ParticleManager();
	partMan->Initialize(ImageManager::nul);

	fbxObject3d->PlayAnimation(0);
}

void Enemy::OnUpda() {
	PhaseMove();
	LifeCommon();

	Shadow->Update();
	Shadow->SetPosition({ fbxObject3d->GetPosition().x,0.01f, fbxObject3d->GetPosition().z });

	obj->SetPosition(fbxObject3d->GetPosition());
	fbxObject3d->Update();
	Attack->Upda();
}

void Enemy::OnFirstDraw(DirectXCommon* dxCommon) {
	Object2d::PreDraw();
	Shadow->Draw();
}

void Enemy::OnDraw(DirectXCommon* dxCommon) {
	Attack->Draw();
	Object3d::PreDraw();
	fbxObject3d->Draw(dxCommon->GetCmdList());
}

void Enemy::OnLastDraw(DirectXCommon* dxCommon) {
}

void Enemy::OnFinal() {
}

void Enemy::OnCollision(const std::string& Tag) {
	if (Tag == "Player") {
		player->SetHitBound(fbxObject3d->GetPosition());
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
	case Actor::Phase::WAIT:
		WaitUpda();
		break;

	default:
		break;
	}
}

void Enemy::UnguardUpda() {
	if (fbxObject3d->GetIsFinish()) { animecount++; }
	if(animecount >= 2) {
		fbxObject3d->StopAnimation();
		waitTimer++;
		Attack->SetPredict(true);
		if (waitTimer == 240) {
			Attack->SetPredict(false);
			command = Actor::Phase::ATTACK;
			waitTimer = 0;
			scaframe = 0.0f;
			animecount = 0;
			return;
		}

		scale = Ease(In, Quad, scaframe, 25.0f, 0.0f);
		if (scaframe < 1.0f) {
			scaframe += 0.016f;
		} else {
			scaframe = 0.0f;
		}
		scale = Ease(In, Quad, scaframe, 25.0f, 16.0f);
		fbxObject3d->SetScale({ scale * 0.001f,scale * 0.001f,scale * 0.001f });
	}
}

void Enemy::AttackUpda() {
	XMFLOAT3 pos = fbxObject3d->GetPosition();
	waitTimer++;
	if (waitTimer==150) {
		pos.y = 0;
		fbxObject3d->ResetAnimation();
		fbxObject3d->PlayAnimation();
		command = Actor::Phase::UNGUARD;
		waitTimer = 0;
		fbxObject3d->SetPosition(pos);
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
	fbxObject3d->SetRotation({0,-180,0});
	fbxObject3d->SetPosition(pos);
}

void Enemy::WaitUpda() {
	waitTimer++;
	Attack->SetPredict(true);
	if (waitTimer == 240) {
		Attack->SetPredict(false);
		command = Actor::Phase::ATTACK;
		waitTimer = 0;
		return;
	}

	scale = Ease(In, Quad, scaframe, 25.0f, 0.0f);
	if (scaframe < 1.0f) {
		scaframe += 0.016f;
	} else {
		scaframe = 0.0f;
	}
	scale = Ease(In, Quad, scaframe, 25.0f, 16.0f);
	fbxObject3d->SetScale({ scale * 0.001f,scale * 0.001f,scale * 0.001f });

}

void Enemy::LifeCommon() {
	if (hp < 0.0f) {
		if (command != DEAD) {
			pause = true;
			return;
		}
		XMFLOAT3 pos = fbxObject3d->GetPosition();
		XMFLOAT3 rot = fbxObject3d->GetRotation();
		XMFLOAT3 sca = fbxObject3d->GetScale();
		fbxObject3d->ResetAnimation();
		rot.y++;
		scale = Ease(In,Quad, scaframe,25.0f,0.0f);
		if (scaframe < 1.0f) {
			scaframe += 0.01f;
		} else {
			isActive = false;
		}
		fbxObject3d->SetScale({scale * 0.001f,scale * 0.001f,scale * 0.001f });
		fbxObject3d->SetRotation(rot);
	}
}

void Enemy::ChangeCommand(const int& num, const int& command, const int& count) {
	static bool isFirst = true;
	static int MotionCount = 0;
	if (isFirst) {
		fbxObject3d->PlayAnimation(num);
		isFirst = false;
	}
	if (fbxObject3d->GetIsFinish()) { MotionCount++; }
	if (MotionCount == count) {
		this->command = command;
		fbxObject3d->StopAnimation();
		isFirst = true;
		MotionCount = 0;
	}
}

