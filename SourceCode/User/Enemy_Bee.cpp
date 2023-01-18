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
	file.open("Resources/csv/EnemyBeeStatus.csv");
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
			max_hp = hp;

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
	Mash_->SetModel(ModelManager::GetIns()->GetFBXModel(ModelManager::kBee));
	Mash_->SetScale({ 0.02f,0.02f, 0.02f });
	obj->SetScale({ 2.0f,2.0f, 2.0f });
	Mash_->SetRotation({ 0,-90,0 });
	Mash_->LoadAnimation();
	before_pos = { 0,20,0 };
	fbxObject3d.reset(Mash_);
	LoadData();
	UpdateCommand();

	player = ActorManager::GetInstance()->SearchActor("Player");

	compornent = new EnemyUI();
	compornent->Initialize();
	compornent->SetMagic(true);

	ActorManager::GetInstance()->AttachActor("Honey");
	ActorManager::GetInstance()->AttachActor("Honey");
	honey[0] = ActorManager::GetInstance()->SearchActor("Honey");
	honey[1] = ActorManager::GetInstance()->SearchActorBack("Honey");

	honey[0]->SetPosition({ 25,0,25 });
	honey[1]->SetPosition({ -25,0,-25 });

	Object2d* Shadow_ = Object2d::Create(ImageManager::Shadow, { 0,0,0 },
		{ 0.5f,0.5f,0.5f }, { 1,1,1,1 });
	Shadow_->Object2dCreate();
	Shadow_->SetRotation({ 90,0,0 });
	Shadow.reset(Shadow_);

	command = LEAVE;
	fbxObject3d->PlayAnimation(Fly);

}


void Enemy_Bee::OnUpda() {
	PhaseMove();
	LifeCommon();
	HoneyControl();

	fbxObject3d->Update();
	Shadow->Update();
	Shadow->SetPosition({ fbxObject3d->GetPosition().x,0.01f, fbxObject3d->GetPosition().z });
	obj->SetRotation(XMFLOAT3{ 0,obj->GetRotation().y - 1,0 });
	obj->SetPosition(fbxObject3d->GetPosition());
}

void Enemy_Bee::OnDraw(DirectXCommon* dxCommon) {
	Object2d::PreDraw();
	Shadow->Draw();
	Object3d::PreDraw();
	fbxObject3d->Draw(dxCommon->GetCmdList());
}

void Enemy_Bee::OnFinal() {
}

void Enemy_Bee::OnCollision(const std::string& Tag) {
	if (Tag == "Player") {
		switch (command) {
		case ATTACK:
			if (rand_pattern == 0) {
				if (pattern == 1) {
					if (!OnePunch) {
						OnePunch = true;
						player->SetHitBound(fbxObject3d->GetPosition());
					}
				}
			}
			if (rand_pattern == 1) {
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
	case Actor::Phase::UNGUARD:
		UnguardUpda();
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

void Enemy_Bee::UnguardUpda() {
	if (honey[0]->GetCommand() == APPROCH) { honey[0]->SetCommand(DEAD); }
	if (honey[1]->GetCommand() == APPROCH) { honey[1]->SetCommand(DEAD); }
	XMFLOAT3 pos = fbxObject3d->GetPosition();
	if (!overDamage) {
		if (pinchLife > hp) {
			hp = pinchLife;
		}
	}
	commandTimer += 1.0f;
	fbxObject3d->SetPosition({ pos.x,-3,pos.z });
	if (honey[0]->GetCommand() == WAIT && honey[1]->GetCommand() == WAIT) {
		fbxObject3d->StopAnimation();
		fbxObject3d->PlayAnimation(Fly);
		if (pinchLife >= hp) {
			overDamage = true;
		}
		commandTimer = 0;
		command = LEAVE;
	}
}

void Enemy_Bee::LeaveUpda() {
	XMFLOAT3 pos = fbxObject3d->GetPosition();
	commandTimer += 1.0f;
	fbxObject3d->SetPosition({ pos.x,sinf(commandTimer * XM_PI / 180) * 2.0f,pos.z });

	if (honey[0]->GetCommand() == APPROCH || honey[1]->GetCommand() == APPROCH) {
		fbxObject3d->StopAnimation();
		fbxObject3d->PlayAnimation(Dawn);
		commandTimer = 0.0f;
		command = UNGUARD;
	} else {
		if (commandTimer > 240) {
			commandTimer = 0;
			waitTimer = 0;
			before_pos = pos;
			if (hp > max_hp * 0.8f) {
				rand_pattern = 0;
				obj->SetScale({ 2.0f,2.0f, 2.0f });
			} else if ((hp > max_hp * 0.5f)) {
				rand_pattern = 1;
				obj->SetScale({ 2.5f,2.5f, 2.5f });
			} else {
				rand_pattern = rand() % 2;
				obj->SetScale({ 2.5f,2.5f, 2.5f });
			}
			command = ATTACK;
		}
	}
}

void Enemy_Bee::WaitUpda() {
}

void Enemy_Bee::AttackUpda() {
	//ChangeCommand(1,WAIT,2);
	XMFLOAT3 PlaPos = player->GetPosition();
	XMFLOAT3 pos = fbxObject3d->GetPosition();
	XMFLOAT3 rot = fbxObject3d->GetRotation();
	if (rand_pattern == 0) {
		switch (pattern) {
		case 0:
			waitTimer++;
			if (OnePunch) {
				OnePunch = false;
			}
			after_pos = {
			PlaPos.x,
			sinf(waitTimer * XM_PI / 180) * 2.0f,
			PlaPos.z
			};
			pos.x = Ease(In, Quad, 0.4f, pos.x, after_pos.x);
			pos.y = Ease(In, Quad, 0.4f, pos.y, after_pos.y);
			pos.z = Ease(In, Quad, 0.4f, pos.z, after_pos.z);
			if (waitTimer > 100) {
				pattern = 1;
				before_pos = pos;
				fbxObject3d->ResetAnimation();
				waitTimer = 0;
			}
			rot.y = DirRotation(PlaPos);
			fbxObject3d->SetRotation(rot);

			fbxObject3d->SetPosition(pos);
			break;
		case 1:
			after_pos = {
			pos.x,
			-8,
			pos.z
			};
			waitTimer++;
			if ((float)(waitTimer / 50) < 1.0f) {
				pos.y = Ease(In, Quad, (float)(waitTimer / 50), before_pos.y, after_pos.y);
			}
			if (waitTimer > 120) {
				if (countAttack < 3) {
					countAttack++;
					pattern = 0;
				} else {
					countAttack = 0;
					pattern = 3;
				}
				before_pos = pos;
				waitTimer = 0;
				fbxObject3d->PlayAnimation(Fly);
				waitTimer = 0;
			}
			rot.y += 5.0f;
			fbxObject3d->SetRotation(rot);
			fbxObject3d->SetPosition(pos);
			break;
		case 3:
			waitTimer++;
			after_pos = {
			0,
			sinf(waitTimer * XM_PI / 180) * 2.0f,
			0
			};
			if ((float)(waitTimer / 120) < 1.0f) {
				pos.x = Ease(In, Quad, (float)(waitTimer / 120), before_pos.x, after_pos.x);
				pos.y = Ease(In, Quad, (float)(waitTimer / 120), before_pos.y, after_pos.y);
				pos.z = Ease(In, Quad, (float)(waitTimer / 120), before_pos.z, after_pos.z);
				rot.y = DirRotation(after_pos);
			} else {
				rot.y = DirRotation(PlaPos);
			}
			if (waitTimer > 150) {
				pattern = 0;
				command = LEAVE;
				waitTimer = 0;
				before_pos = pos;
			}
			fbxObject3d->SetRotation(rot);
			fbxObject3d->SetPosition(pos);
			break;
		default:
			break;
		}
	} else {
		switch (pattern) {
		case 0:
			waitTimer++;
			if (OnePunch) {
				OnePunch = false;
			}
			after_pos = {
			45,
			0,
			45
			};
			if ((float)(waitTimer / 130) < 1.0f) {
				pos.x = Ease(In, Quad, (float)(waitTimer / 130), before_pos.x, after_pos.x);
				pos.y = Ease(In, Quad, (float)(waitTimer / 130), before_pos.y, after_pos.y);
				pos.z = Ease(In, Quad, (float)(waitTimer / 130), before_pos.z, after_pos.z);
				rot.y = DirRotation(after_pos);
			}
			if (waitTimer > 150) {
				pattern = 1;
				before_pos = pos;
				waitTimer = 0;
			}
			fbxObject3d->SetPosition(pos);
			break;
		case 1:
			waitTimer++;
			if (OnePunch) { OnePunch = false; }
			after_pos = {
			45,
			0,
			-45
			};
			if ((float)(waitTimer / 130) < 1.0f) {
				pos.x = Ease(In, Quad, (float)(waitTimer / 130), before_pos.x, after_pos.x);
				pos.y = Ease(In, Quad, (float)(waitTimer / 130), before_pos.y, after_pos.y);
				pos.z = Ease(In, Quad, (float)(waitTimer / 130), before_pos.z, after_pos.z);
				rot.y = DirRotation(after_pos);
			}
			if (waitTimer > 190) {
				pattern = 2;
				before_pos = pos;
				waitTimer = 0;
			}
			fbxObject3d->SetPosition(pos);
			break;
		case 2:
			after_pos = {
			-45,
			0,
			45
			};
			waitTimer++;
			if ((float)(waitTimer / 130) < 1.0f) {
				pos.x = Ease(In, Quad, (float)(waitTimer / 130), before_pos.x, after_pos.x);
				pos.y = Ease(In, Quad, (float)(waitTimer / 130), before_pos.y, after_pos.y);
				pos.z = Ease(In, Quad, (float)(waitTimer / 130), before_pos.z, after_pos.z);
				rot.y = DirRotation(after_pos);
			}
			if (waitTimer > 190) {
				pattern = 3;
				before_pos = pos;
				waitTimer = 0;
			}
			fbxObject3d->SetPosition(pos);
			break;
		case 3:
			after_pos = {
			-45,
			0,
			-45
			};
			waitTimer++;
			if ((float)(waitTimer / 130) < 1.0f) {
				pos.x = Ease(In, Quad, (float)(waitTimer / 130), before_pos.x, after_pos.x);
				pos.y = Ease(In, Quad, (float)(waitTimer / 130), before_pos.y, after_pos.y);
				pos.z = Ease(In, Quad, (float)(waitTimer / 130), before_pos.z, after_pos.z);
				rot.y = DirRotation(after_pos);
			}
			if (waitTimer > 190) {
				pattern = 4;
				before_pos = pos;
				waitTimer = 0;
			}
			fbxObject3d->SetPosition(pos);
			break;
		case 4:
			waitTimer++;
			after_pos = {
			0,
			0,
			0
			};
			waitTimer++;
			if ((float)(waitTimer / 130) < 1.0f) {
				pos.x = Ease(In, Quad, (float)(waitTimer / 130), before_pos.x, after_pos.x);
				pos.y = Ease(In, Quad, (float)(waitTimer / 130), before_pos.y, after_pos.y);
				pos.z = Ease(In, Quad, (float)(waitTimer / 130), before_pos.z, after_pos.z);
				rot.y = DirRotation(after_pos);
			}
			if (waitTimer > 190) {
				pattern = 0;
				command = LEAVE;
				waitTimer = 0;
				before_pos = pos;
			}
			fbxObject3d->SetPosition(pos);
			break;

		default:
			break;
		}
		fbxObject3d->SetRotation(rot);

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

void Enemy_Bee::HoneyControl() {
	if (honey[0]->GetCommand() != WAIT) {
		honey[1]->SetPause(true);
	} else {
		honey[1]->SetPause(false);
	}
	if (honey[1]->GetCommand() != WAIT) {
		honey[0]->SetPause(true);
	} else {
		honey[0]->SetPause(false);
	}
}

void Enemy_Bee::IntroOnUpdate(const float& Timer) {
	XMFLOAT3 pos = fbxObject3d->GetPosition();
	XMFLOAT3 rot = fbxObject3d->GetRotation();

	if (Timer <= 0.2f) {
		after_pos = {
			0,
			0,
			0
		};
		if ((Timer / 0.2f) < 1.0f) {
			pos.y = Ease(Out, Quad, Timer / 0.2f, before_pos.y, after_pos.y);
		} else {
			fbxObject3d->PlayAnimation(Posing);
			before_pos = after_pos;
		}
	} else if (Timer <= 0.5f) {
		if (fbxObject3d->GetIsFinish()) {
			fbxObject3d->StopAnimation();
			fbxObject3d->PlayAnimation(Fly);
		}
		after_pos = {
		15,
		-8,
		10
		};
	} else if (Timer <= 0.65f) {
		float time = (Timer - 0.5f);
		if (time <= 0.1f) {
			pos.x = Ease(In, Linear, time / 0.1f, before_pos.x, after_pos.x);
			pos.y = Ease(In, Linear, time / 0.1f, before_pos.y, after_pos.y);
			pos.z = Ease(In, Linear, time / 0.1f, before_pos.z, after_pos.z);
			rot.y = DirRotation(after_pos);
		}
	} else {




	}

	fbxObject3d->SetPosition(pos);
	fbxObject3d->SetRotation(rot);
	fbxObject3d->Update();

	Shadow->SetPosition({ fbxObject3d->GetPosition().x,0.01f, fbxObject3d->GetPosition().z });
	Shadow->Update();
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
			if (MotionCount >= count + 1) {
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

bool Enemy_Bee::WaitingElapsedTime() {

	waiting_timer++;
	if (waiting_timer > change_time) {

	}
	return true;
}
float Enemy_Bee::DirRotation(const XMFLOAT3& target) {
	float itr{};
	XMFLOAT3 pos = fbxObject3d->GetPosition();
	XMFLOAT3 position{};
	position.x = (target.x - pos.x);
	position.z = (target.z - pos.z);
	itr = (atan2f(position.x, position.z) * (180.0f / XM_PI)) - 90; //- 90;// *(XM_PI / 180.0f);
	//if (itr >= 0) {
	//	itr = (float)((int)itr % 360);
	//} else {
	//	itr += 360;
	//	itr = (float)((int)itr % 360);
	//}
	return itr;
}