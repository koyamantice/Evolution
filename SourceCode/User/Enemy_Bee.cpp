#include"Enemy_Bee.h"
#include"ModelManager.h"
#include"ImageManager.h"
#include<fstream>
#include <SourceCode/FrameWork/collision/CollisionManager.h>
#include <SourceCode/FrameWork/collision/SphereCollider.h>
#include <SourceCode/FrameWork/collision/CollisionAttribute.h>
#include <SourceCode/FrameWork/ActorManager.h>
#include <SourceCode/Common/Easing.h>


void Enemy_Bee::LoadData() {
	std::ifstream file;
	file.open("Resources/csv/EnemyStatus.csv");
	assert(file.is_open());
	parameterCommands << file.rdbuf();
	file.close();
}

void Enemy_Bee::UpdateCommand() {
	std::string line;

	while (getline(parameterCommands, line)) {
		//解析しやすくする
		std::istringstream line_stream(line);

		std::string word;
		//半角スペース区切りで行の先頭文字列を取得
		getline(line_stream, word, ',');
		//"//"から始まる行はコメント
		if (word.find("//") == 0) {
			//飛ばす
			continue;
		}
		//各コマンド
		if (word.find("HP") == 0) {
			getline(line_stream, word, ',');
			hp = (float)std::atof(word.c_str());
		} else if (word.find("VEL") == 0) {
			getline(line_stream, word, ',');
			vel = (float)std::atof(word.c_str());
		} else if (word.find("PHASE") == 0) {
			getline(line_stream, word, ',');
			if (word.find("APPROCH") == 0) {
				command = Actor::Phase::APPROCH;
			} else if (word.find("LEAVE") == 0) {
				command = Actor::Phase::LEAVE;
			} else if (word.find("WAIT") == 0) {
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

void Enemy_Bee::DebugUpdate() {
	fbxObject3d->Update();
	fbxObject3d->SetPosition({ 0,-3,0 });
}

void Enemy_Bee::OnInit() {
	isVisible = false;
	FBXObject3d* Mash_ = new FBXObject3d();
	Mash_->Initialize();
	Mash_->SetModel(ModelManager::GetIns()->GetFBXModel(ModelManager::Demo));
	Mash_->SetScale({ 0.02f,0.02f, 0.02f });
	Mash_->SetRotation({ 0,-90,0 });
	Mash_->LoadAnimation();
	fbxObject3d.reset(Mash_);
	fbxObject3d->PlayAnimation();
	LoadData();
	UpdateCommand();

	player = ActorManager::GetInstance()->SearchActor("Player");

	compornent = new EnemyUI();
	compornent->Initialize();

	ActorManager::GetInstance()->AttachActor("Honey");
	ActorManager::GetInstance()->AttachActor("Honey");
	Honey[0]= ActorManager::GetInstance()->SearchActor("Honey");
	Honey[1]= ActorManager::GetInstance()->SearchActorBack("Honey");

	Honey[0]->SetPosition({ 25,0,25 });
	Honey[1]->SetPosition({-25,0,-25});

	Texture* Shadow_ = Texture::Create(ImageManager::Shadow, { 0,0,0 },
	{ 0.5f,0.5f,0.5f }, { 1,1,1,1 });
	//Shadow_->SetIsBillboard(true);
	Shadow_->TextureCreate();
	Shadow_->SetRotation({ 90,0,0 });
	Shadow.reset(Shadow_);

	command = LEAVE;
}


void Enemy_Bee::OnUpda() {
	fbxObject3d->Update();
	PhaseMove();
	LifeCommon();
	Shadow->Update();
	Shadow->SetPosition({ fbxObject3d->GetPosition().x,0.01f, fbxObject3d->GetPosition().z });
	obj->SetRotation(XMFLOAT3{ 0,obj->GetRotation().y - 1,0 });
	obj->SetPosition(fbxObject3d->GetPosition());
}

void Enemy_Bee::OnDraw(DirectXCommon* dxCommon) {
	//float k = fbxObject3d->GetPosition().y;
	//ImGui::Begin("test");
	//ImGui::SliderInt("yabei", &pattern, 0, 10);
	//ImGui::SliderFloat("yabai", &k, 0, 10);
	//ImGui::End();

	Object3d::PreDraw();
	fbxObject3d->Draw(dxCommon->GetCmdList());
	Texture::PreDraw();
	Shadow->Draw();
}

void Enemy_Bee::OnFinal() {
}
void Enemy_Bee::OnCollision(const std::string& Tag) {
	if (Tag=="Player") {
		switch (command) {
		case ATTACK:
			if (pattern == 1) {
				if (!OnePunch) {
					OnePunch = true;
					player->SetHitBound(fbxObject3d->GetPosition());
				}
			}
			break;
		default:
			break;
		}






	}



}
void Enemy_Bee::PhaseMove() {
	switch (command) {
	case Actor::Phase::APPROCH:
		ApprochUpda();
		break;
	case Actor::Phase::LEAVE:
		LeaveUpda();
		break;
	case Actor::Phase::WAIT:
		WaitUpda();
		break;
	case Actor::Phase::ATTACK:
		AttackUpda();
		break;
	default:
		break;
	}
}

void Enemy_Bee::ApprochUpda() {
	ChangeCommand(1,LEAVE,3);

}

void Enemy_Bee::LeaveUpda() {
	XMFLOAT3 pos = fbxObject3d->GetPosition();
	waitTimer += 2.0f;
	pos.y = sinf(waitTimer * XM_PI / 180);
	fbxObject3d->SetPosition(pos);
	ChangeCommand(2, ATTACK, 3);
	Honey[0]->SetCommand(WAIT);
}

void Enemy_Bee::WaitUpda() {
	ChangeCommand(0, APPROCH, 3);

}

void Enemy_Bee::AttackUpda() {
	//ChangeCommand(1,WAIT,2);
	XMFLOAT3 PlaPos = player->GetPosition();
	XMFLOAT3 pos = fbxObject3d->GetPosition();

	switch (pattern) {
	case 0:
		waitTimer++;

		AfterPos = {
		PlaPos.x,
		sinf(waitTimer * XM_PI / 180),
		PlaPos.z
		};
		pos.x = Ease(In,Quad,0.3f,pos.x,AfterPos.x);
		pos.y = Ease(In,Quad,0.3f,pos.y,AfterPos.y);
		pos.z = Ease(In,Quad,0.3f,pos.z,AfterPos.z);
		if (waitTimer > 100) {
			pattern++;
			waitTimer = 0;
		}
		fbxObject3d->SetPosition(pos);
		break;
	case 1:
		AfterPos = {
		pos.x,
		-7,
		pos.z
		};
		waitTimer++;
		if ((float)(waitTimer / 100) < 1.0f) {
			pos.y = Ease(In, Quad, (float)(waitTimer / 100), pos.y, AfterPos.y);

		}
		if (waitTimer > 130) {
			pattern++;
			waitTimer = 0;
		}
		fbxObject3d->SetPosition(pos);
		break;
	case 2:
		waitTimer++;
		if (OnePunch) {
			OnePunch = false;
		}
		AfterPos = {
		PlaPos.x,
		sinf(waitTimer * XM_PI / 180),
		PlaPos.z
		};
		pos.x = Ease(In, Quad, 0.3f, pos.x, AfterPos.x);
		pos.y = Ease(In, Quad, 0.3f, pos.y, AfterPos.y);
		pos.z = Ease(In, Quad, 0.3f, pos.z, AfterPos.z);
		if (waitTimer > 70) {
			pattern=0;
			waitTimer = 0;
		}
		fbxObject3d->SetPosition(pos);
		break;
	case 3:
		break;
	case 4:
		break;
	case 5:
		break;
	default:
		break;
	}






}

void Enemy_Bee::LifeCommon() {
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
		scale = Ease(In, Quad, scaframe, 1.0f, 0.0f);
		if (scaframe < 1.0f) {
			scaframe += 0.01f;
		} else {
			isActive = false;
		}
		fbxObject3d->SetScale({ scale * 0.01f,scale * 0.01f,scale * 0.01f });
		fbxObject3d->SetRotation(rot);
	}
}

void Enemy_Bee::ChangeCommand(const int& num, const int& command, const int& count, const bool& reverese) {
	static bool isFirst = true;
	static int MotionCount = 0;
	if (isFirst) {
		fbxObject3d->PlayAnimation(num);
		isFirst = false;
	}
	if (fbxObject3d->GetIsFinish()) { MotionCount++; }
	if (MotionCount >= count) {
		if (reverese) {
			fbxObject3d->ReverseAnimation(num);
			if (MotionCount>=count+1) {
				if (fbxObject3d->GetIsFinish()) {
					this->command = command;
					isFirst = true;
					MotionCount = 0;
				}
			}
		}
	}
	if (MotionCount == count) {
		this->command = command;
		isFirst = true;
		MotionCount = 0;
		waitTimer = 0;
	}
}
