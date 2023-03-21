#include"Bullet.h"
#include <SourceCode/User/ModelManager.h>
#include <SourceCode/FrameWork/ActorManager.h>
#include"ImageManager.h"
#include <SourceCode/Common/Easing.h>
#include <SourceCode/FrameWork/collision/Collision.h>
#include <BaseScene.h>



Bullet::Bullet() {
}

void Bullet::Initialize(FBXModel* model, const std::string& tag, ActorComponent* compornent) {
	FBXObject3d* fbxObj_ = new FBXObject3d();
	fbxObj_->Initialize();
	fbxObj_->SetModel(model);
	fbxObj_->SetScale({ 0.003f,0.003f, 0.003f });
	fbxObj.reset(fbxObj_);
	fbxObj->LoadAnimation();
	fbxObj->PlayAnimation();

	Object2d* Explo_ = Object2d::Create(ImageManager::Fire, { fbxObj->GetPosition().x,fbxObj->GetPosition().y + 1.0f,fbxObj->GetPosition().z},
		{ 0.1f,0.1f,0.1f }, { 1,1,1,1 });
	Explo_->SetIsBillboard(true);
	Explo.reset(Explo_);

	Object2d* Shadow_ = Object2d::Create(ImageManager::Shadow, { 0,0,0 },
		{ 0.2f,0.2f,0.2f }, { 1,1,1,1 });
	Shadow_->Object2dCreate();
	Shadow_->SetRotation({ DEGREE_QUARTER,0,0 });
	Shadow.reset(Shadow_);





	audioManager = std::make_unique<AudioManager>();
	audioManager->LoadWave("SE/attack.wav");
	audioManager->LoadWave("SE/gnormDead.wav");

	OnInit();
	//IDごとの誤差の範囲
	if ((int)ID / 7 == 0) {
		margin = -2.5f;
	} else if ((int)ID / 7 == 1) {
		margin = 2.5f;
	} else {
		margin = 0.0f;
	}
}

void Bullet::Update() {
	if (isActive) {
		CommonUpda();
		CommandUpda();
		OnUpda();
		LimitArea();
		TraceUpda();
	}
}

void Bullet::IntroUpdate(const float& Timer, const int& _stage) {
	if (!isActive) { return; }
	const float delay = 0.5f;
	const float first_hight = 100;
	XMFLOAT3 player_pos = player->GetPosition();
	if (Timer + delay <= 1.0f) {
		hight = Ease(In, Linear, Timer + delay, first_hight, 0);
	}
	fbxObj->Update();
	fbxObj->SetRotation({ 0,DirRotation(player_pos),0 });
	const float angle = DEGREE_HALF / 10;
	const float radius = 7.0f;
	fbxObj->SetPosition({ player_pos.x + sinf(((int)ID)* angle * (XM_PI/DEGREE_HALF)) * radius, hight,  player_pos.z + cos(((int)ID) * angle * (XM_PI / DEGREE_HALF)) * radius });
	IntroOnUpdate(Timer);

	if (_stage == SceneNum::kSecondScene) {
		fbxObj->SetRotation({ 0,180,0 });
		fbxObj->SetPosition({ (((int)ID % 10) - 4.5f) * 3.0f, 0, ((int)ID / 10) * 5.0f });
		fbxObj->Update();
	}
}

void Bullet::ResultUpdate(const float& Timer) {
	isFinish = true;
	ResultOnUpdate(Timer);
	if (burning) {
		BurnOut();
	}
	TraceUpda();
	Explo->Update();
	Status->Update();
	ShadowUpda();
}

void Bullet::SetAggregation() {
	XMFLOAT3 pos = player->GetAFTIMAGE(ID % 7);
	XMFLOAT3 BulletPos = fbxObj->GetPosition();
	XMFLOAT3 position{};

	position.x = (BulletPos.x - (pos.x + margin));
	position.z = (BulletPos.z - (pos.z + margin));
	if (powf(position.x, 2) + powf(position.z, 2) > 4 || collide) {
		vel_follow.x = sin(-atan2f(position.x, position.z)) * 0.2f;
		vel_follow.y = cos(-atan2f(position.x, position.z)) * 0.2f;
		BulletPos.x += vel_follow.x;
		BulletPos.z -= vel_follow.y;
	}
	float rotation = 0;
	if (Collision::CircleCollision(fbxObj->GetPosition().x, fbxObj->GetPosition().z, 3.0f, player->GetPosition().x, player->GetPosition().z, 3.0f)) {
		rotation = Ease(In, Quad, 0.5f, fbxObj->GetRotation().y, pos.y);
	} else {
		rotation = (atan2f(position.x, position.z) * (DEGREE_HALF / XM_PI)); //- 90;// *(XM_PI / 180.0f);
	}
	fbxObj->SetPosition(BulletPos);
	fbxObj->SetRotation({ 0,rotation,0 });
}

void Bullet::LimitArea() {
	XMFLOAT3 pos = fbxObj->GetPosition();
	if (pos.x > 48.0f) {
		pos.x = 48.0f;
	}
	if (pos.x < -48.0f) {
		pos.x = -48.0f;
	}
	if (pos.z > 48.0f) {
		pos.z = 48.0f;
	}
	if (pos.z < -48.0f) {
		pos.z = -48.0f;
	}
	fbxObj->SetPosition(pos);
}

void Bullet::CommonUpda() {
	old_pos = fbxObj->GetPosition();
	if (collide) { collide = false; }
	if (knockBacking) {	KnockBack();}
	if (isPlayActive) { command = Control; }
	if (!wait) { follow_frame = 0.0f; }
	if (enemy != nullptr) {
		if (enemy->GetHp() <= 0) { clear_s_pos = fbxObj->GetPosition(); command = Smash; }
	}
	if (burning) {
		BurnOut();
	}
	if (DeadFlag) {
		DeadEnd();
		return;
	}
	fbxObj->Update();
	Explo->Update();
	ShadowUpda();
	Status->Update();
	Status->SetPosition({ fbxObj->GetPosition().x,fbxObj->GetPosition().y + 2.5f,fbxObj->GetPosition().z });
}

void Bullet::CommandUpda() {
	if (DeadFlag) { return; }
	switch (command) {
	case Wait:
		WaitUpda();
		break;
	case Attack:
		if (enemy != nullptr) {
			if (enemy->GetIsActive()) {
				AttackUpda();
			}
		}
		break;
	case Control:
		ControlUpda();
		break;
	case Slow:
		if (wait) { wait = false; follow_frame = 0.0f; }
		SlowUpda();
		break;
	case Smash:
		SmashUpda();
		break;
	default:
		assert(0);
		break;
	}
}

void Bullet::FirstDraw(DirectXCommon* dxCommon) {
	if (isActive) {
		Object2d::PreDraw();
		for (std::unique_ptr<Trace>& trace : traces_) {
			trace->Draw();
		}
		Shadow->Draw();
		OnFirstDraw(dxCommon);
	}
}

void Bullet::Draw(DirectXCommon* dxCommon) {
	if (isActive) {
		Object3d::PreDraw();
		fbxObj->Draw(dxCommon->GetCmdList());
		OnDraw(dxCommon);
	}
}

void Bullet::LastDraw(DirectXCommon* dxCommon) {
	if (isActive) {
		if (enemy == nullptr) { return; }
		if (DeadFlag) {
			Object2d::PreDraw();
			CharaDead->Draw();
		}
		if (enemy->GetIsActive()) {
			if (command == Wait) { return; }
			if (command == Control) { return; }
			Object2d::PreDraw();
			if (!DeadFlag) {
				if (!isFinish&&Collision::CircleCollision(fbxObj->GetPosition().x, fbxObj->GetPosition().z, 15.0f, enemy->GetPosition().x, enemy->GetPosition().z, 1.0f)) {
					Status->Draw();
				}
				if (burning) { Explo->Draw(); }
			} else {
				CharaDead->Draw();
			}
		}
		OnLastDraw(dxCommon);
	}
}

void Bullet::DemoDraw(DirectXCommon* dxCommon) {
	if (isActive) {
		DebugUpdate();
		Object3d::PreDraw();
		fbxObj->Draw(dxCommon->GetCmdList());
		OnDraw(dxCommon);
	}
}

void Bullet::Finalize() {
}

float Bullet::DirRotation(const XMFLOAT3& target) {
	float itr{};
	XMFLOAT3 pos = fbxObj->GetPosition();
	XMFLOAT3 position{};
	position.x = (target.x - pos.x);
	position.z = (target.z - pos.z);
	itr = (atan2f(position.x, position.z) * (DEGREE_HALF / XM_PI)) + DEGREE_HALF; //- 90;// *(XM_PI / 180.0f);
	if (itr >= 0) {
		itr = (float)((int)itr % (int)DEGREE_MAX);
	} else {
		itr += DEGREE_MAX;
		itr = (float)((int)itr % (int)DEGREE_MAX);
	}
	return itr;
}

void Bullet::Navigation(const XMFLOAT3& target) {
	navi = true;
	before_pos = fbxObj->GetPosition();
	after_pos = target;
}

void Bullet::OnCollision(const std::string& Tag, const XMFLOAT3& pos) {
	switch (command) {
	case Wait:

		break;
	case Attack:
		//プレイヤーとの当たり判定
		if (Tag == "Player") {
			if (isPlayActive) { return; }
			player->SetStock(player->GetStock() + 1);
			command = Wait;
		}
		//蜂蜜移動
		if (Tag == "Honey") {
			if (isPlayActive) { 
				return; 
			}
			ActionActor = ActorManager::GetInstance()->GetAreaActor(fbxObj->GetPosition(), "Honey");
			if (!ActionActor->GetCollide()) {
				ActionActor->SetCollide(true);
			}

		}
		//敵との当たり判定
		if (Tag == "Enemy") {
			if (isPlayActive) { return; }
			//if(enemy->Get) {
			DamageInit();
		}
		break;
	case Control:
		break;

	case Slow:
		break;
	case Smash:
		break;
	default:
		assert(0);
		break;
	}
}

void Bullet::SetCommand(const int& command, XMFLOAT3 pos) {
	this->command = command; after_pos = pos;
}

bool Bullet::Follow2Position(const XMFLOAT3& _pos, const float& _radius) {
	XMFLOAT3 pos = fbxObj->GetPosition();
	XMFLOAT3 position{};
	position.x = (_pos.x - pos.x);
	position.z = (_pos.z - pos.z);
	vel_follow.x = sin(-atan2f(position.x, position.z)) * follow_vel_;
	vel_follow.y = cos(-atan2f(position.x, position.z)) * follow_vel_;
	if (powf(position.x, 2) + powf(position.z, 2) > powf(_radius, 2)) {
		pos.x -= vel_follow.x;
		pos.z += vel_follow.y;
		fbxObj->SetPosition(pos);
		fbxObj->SetRotation({ 0 ,DirRotation(_pos),0 });
		return true;
	} else {
		return false;
	}
}


void Bullet::KnockBack() {
	XMFLOAT3 pos = fbxObj->GetPosition();

	if (damageframe >= 1.0f) {
		damageframe = 0.0f;
		knockBacking = false;
		pos.y = 0.0f;
		fbxObj->SetPosition(pos);
		return;
	} else {
		damageframe += 1.0f / kDeadFrameMax;
	}
	pos.x = Ease(In, Quad, damageframe, s_rebound_.x, e_rebound_.x);

	pos.y += fall; //+
	fall -= kFallHeight / (kDeadFrameMax / 2.0f);//
	if (pos.y < 0.0f) { pos.y = 0; }

	pos.z = Ease(In, Quad, damageframe, s_rebound_.z, e_rebound_.z);
	fbxObj->SetPosition(pos);
}

void Bullet::DamageInit() {
	if (!knockBacking) {
		audioManager->PlayWave("SE/attack.wav", 0.5f);
		enemy->SetHp(enemy->GetHp() - 1);
		burning = true;

		XMFLOAT3 pos = fbxObj->GetPosition();
		fall = kFallHeight;
		exploPos = old_pos;
		XMFLOAT3 distance = { pos.x - old_pos.x,0,pos.z - old_pos.z };
		s_rebound_ = pos;
		e_rebound_ = {
		pos.x - sinf(atan2f(distance.x, distance.z)) * 10.0f,
		0,
		pos.z - cosf(atan2f(distance.x, distance.z)) * 10.0f
		};
		knockBacking = true;
	}
}


void Bullet::DeadEnd() {
	if (deadframe == 0.0f) {
		audioManager->PlayWave("SE/gnormDead.wav", 0.2f);
	}
	if (deadframe > 1.0f) {
		isActive = false;
		isRemove = true;
	} else {
		deadframe += 0.01f;
	}
	CharaDead->Update();
	//イージング処理
	vanishHight = Ease(Out, Quad, deadframe, 0.1f, 4.5f);
	vanishAlpha = Ease(In, Quad, deadframe, 1.0f, 0.5f);
	//更新処理
	fbxObj->SetScale({ 0.003f,0.0001f, 0.003f });
	fbxObj->Update();
	Shadow->Update();
	Shadow->SetPosition({ fbxObj->GetPosition().x,0.01f, fbxObj->GetPosition().z });
	CharaDead->SetPosition({ fbxObj->GetPosition().x,vanishHight, fbxObj->GetPosition().z });
	CharaDead->SetColor({ 1,1,1,vanishAlpha });
}

void Bullet::BurnOut() {
	if (effectRate < 1.0f) {
		effectRate += 0.1f;
	} else {
		effectRate = 0;
		Explo->SetScale({ 0,0,0 });
		burning = false;
		return;
	}
	scale = Ease(In, Quad, effectRate, 0.5f, 1.0f);
	Explo->SetScale({ scale,scale,scale });
	Explo->SetPosition(exploPos);
}

void Bullet::ShadowUpda() {

	XMFLOAT3 pos = fbxObj->GetPosition();
	float max_height_ = 12.0f;
	float scale = ((max_height_ - pos.y) / max_height_) * shadow_side_;
	scale = max(0.0f, scale);


	Shadow->SetScale({ scale,scale, scale, });
	Shadow->Update();
	Shadow->SetPosition({ pos.x,0.01f, pos.z });

}

void Bullet::TraceUpda() {
	foot_count_--;
	if (foot_count_ <=0) {
		float foot_rot = fbxObj->GetPosition().y;
		std::unique_ptr<Trace> Trace_ = std::make_unique<Trace>(foot_rot, fbxObj->GetPosition());
		traces_.push_back(std::move(Trace_));
		foot_count_ = 30;
	}
	for (std::unique_ptr<Trace>& trace : traces_) {
		trace->Update();
	}
	traces_.remove_if([](std::unique_ptr<Trace>& trace) {
		return trace->GetLife()<=0;
	});
}

void Bullet::ControlUpda() {

	if (!Follow2Position(ActionActor->GetPosition(), ActionActor->GetSize())) {
		if (!knockBacking) {
			audioManager->PlayWave("SE/attack.wav", 0.5f);
			burning = true;
			XMFLOAT3 pos = fbxObj->GetPosition();
			fall = kFallHeight;
			exploPos = ActionActor->GetPosition();
			XMFLOAT3 distance = { pos.x - ActionActor->GetPosition().x,0,pos.z - ActionActor->GetPosition().z };
			s_rebound_ = pos;
			e_rebound_ = {
			pos.x + sinf(atan2f(distance.x, distance.z)) * 5.0f,
			0,
			pos.z + cosf(atan2f(distance.x, distance.z)) * 5.0f
			};
			knockBacking = true;
		}
	}
	if (navi) {
		if (navi_frame > 1.0f) {
			navi_frame = 1.0f;
			isPlayActive = false;
			command = Attack;
			navi = false;
		} else {
			navi_frame += 0.008f;
		}
		XMFLOAT3 pos{};
		pos.x = Ease(In, Linear, navi_frame, before_pos.x, (after_pos.x + margin));
		pos.y = Ease(In, Linear, navi_frame, before_pos.y, after_pos.y);
		pos.z = Ease(In, Linear, navi_frame, before_pos.z, (after_pos.z + margin));
		fbxObj->SetPosition(pos);
	}
}



void Bullet::SmashUpda() {

	status_alpha_ = Ease(In, Linear, 0.8f, status_alpha_, 0);


	Status->SetColor({1,1,1,status_alpha_ });
}

void Bullet::WaitUpda() {
	throwReady = true;
	vel = 0.8f;
	frame = 0.0f;
	//投げてる最中の回収時の矛盾解消
	XMFLOAT3 pos = fbxObj->GetPosition();
	if (pos.y > 0) {
		pos.y -= 0.3f;
	} else {
		pos.y = 0;
	}
	fbxObj->SetPosition(pos);
	SetAggregation();
}

void Bullet::SlowUpda() {
	if (throwReady) {
		XMFLOAT3 pos = fbxObj->GetPosition();
		XMFLOAT3 rot = fbxObj->GetRotation();
		pos.x = Ease(InOut, Quad, frame, pos.x, after_pos.x);
		pos.y += vel; //+
		vel -= kSlowHight / (kSlowFrameMax / 2.5f);//
		if (pos.y < 0.0f) {
			pos.y = 0;
		}
		pos.z = Ease(InOut, Quad, frame, pos.z, after_pos.z);

		const float delay = 0.3f;
		if (frame < 1.0f - delay) {
			rot.x = Ease(In, Quad, frame + delay, 0, -DEGREE_MAX);
		}

		if (frame < 1.0f) {
			frame += 1.0f/kSlowFrameMax;
		} else {
			frame = 1.0f;
			pos.y = 0.0f;
			rot.x = 0;
			vel = kSlowHight;
			throwReady = false;
		}
		fbxObj->SetPosition(pos);
		fbxObj->SetRotation(rot);

	} else {
		fbxObj->SetRotation({ 0,fbxObj->GetRotation().y,0 });
		command = Attack;
	}

}

void Bullet::AttackUpda() {

	if (!knockBacking) {
		if (enemy->GetHp() == 0) { return; }
		if (Collision::CircleCollision(fbxObj->GetPosition().x, fbxObj->GetPosition().z, 15.0f, enemy->GetPosition().x, enemy->GetPosition().z, 1.0f)) {
			Follow2Position(enemy->GetPosition(), 2.0f);
		}
	}
}
