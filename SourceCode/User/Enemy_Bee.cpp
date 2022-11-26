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
	Mash->Update();
	Mash->SetPosition({ 0,-3,0 });
}

void Enemy_Bee::OnInit() {
	isVisible = false;
	FBXObject3d* Mash_ = new FBXObject3d();
	Mash_->Initialize();
	Mash_->SetModel(ModelManager::GetIns()->GetFBXModel(ModelManager::Demo));
	Mash_->SetScale({ 0.01f,0.01f, 0.01f });
	Mash_->SetRotation({ 0,-90,0 });
	Mash_->LoadAnimation();
	Mash.reset(Mash_);
	Mash->PlayAnimation();
	LoadData();
	UpdateCommand();
	player = ActorManager::GetInstance()->SearchActor("Player");

	compornent = new EnemyUI();
	compornent->Initialize();

	ActorManager::GetInstance()->AttachActor("Honey");

	Texture* Shadow_ = Texture::Create(ImageManager::Shadow, { 0,0,0 },
		{ 0.5f,0.5f,0.5f }, { 1,1,1,1 });
	//Shadow_->SetIsBillboard(true);
	Shadow_->TextureCreate();
	Shadow_->SetRotation({ 90,0,0 });
	Shadow.reset(Shadow_);

	command = WAIT;

}


void Enemy_Bee::OnUpda() {
	Mash->Update();
	PhaseMove();
	LifeCommon();
	Shadow->Update();
	Shadow->SetPosition({ Mash->GetPosition().x,0.01f, Mash->GetPosition().z });
	obj->SetRotation(XMFLOAT3{ 0,obj->GetRotation().y - 1,0 });
	obj->SetPosition(Mash->GetPosition());
}

void Enemy_Bee::OnDraw(DirectXCommon* dxCommon) {
	Object3d::PreDraw();
	Mash->Draw(dxCommon->GetCmdList());
	Texture::PreDraw();
	Shadow->Draw();
}

void Enemy_Bee::OnFinal() {
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

}

void Enemy_Bee::LeaveUpda() {
}

void Enemy_Bee::WaitUpda() {
	waitTimer++;
	if (waitTimer==1) {
		Mash->PlayAnimation(1);
	}else if (waitTimer == 450) {//150fps単位
		//Mash->ResetAnimation();
		//command = Actor::Phase::ATTACK;
	}
}

void Enemy_Bee::AttackUpda() {

}

void Enemy_Bee::LifeCommon() {
	if (hp < 0.0f) {
		if (command != DEAD) {
			pause = true;
			return;
		}
		XMFLOAT3 pos = Mash->GetPosition();
		XMFLOAT3 rot = Mash->GetRotation();
		XMFLOAT3 sca = Mash->GetScale();
		Mash->ResetAnimation();

		rot.y++;
		scale = Ease(In, Quad, scaframe, 1.0f, 0.0f);
		if (scaframe < 1.0f) {
			scaframe += 0.01f;
		} else {
			isActive = false;
		}
		Mash->SetScale({ scale * 0.01f,scale * 0.01f,scale * 0.01f });
		Mash->SetRotation(rot);
	}
}