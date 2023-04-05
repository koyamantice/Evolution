#include"Bullet.h"
#include <SourceCode/User/ModelManager.h>
#include <SourceCode/FrameWork/ActorManager.h>
#include"ImageManager.h"
#include <SourceCode/Common/Easing.h>
#include <SourceCode/FrameWork/collision/Collision.h>
#include <BaseScene.h>
#include <Helper.h>


void (Bullet::* Bullet::statusFuncTable[])() = {
	&Bullet::WaitUpdate,//要素0
	&Bullet::AttackUpdate, //要素1
	&Bullet::BattleUpdata,
	&Bullet::ControlUpdate,
	&Bullet::SlowUpdate,
	&Bullet::DeadUpdate,
	&Bullet::SmashUpdate,
	&Bullet::DitchUpdate,
	&Bullet::VanishUpdate,
	&Bullet::ScaryUpdate,
};

Bullet::Bullet() {
}


void Bullet::Initialize(FBXModel* model, const std::string& tag, ActorComponent* compornent) {

	fbxobj_ = std::make_unique<FBXObject3d>();
	fbxobj_->Initialize();
	fbxobj_->SetModel(model);
	fbxobj_->SetScale({ 0.003f,0.003f, 0.003f });
	fbxobj_->LoadAnimation();
	fbxobj_->PlayAnimation();

	explo_ = Object2d::Create(ImageManager::Fire, { fbxobj_->GetPosition().x,fbxobj_->GetPosition().y + 1.0f,fbxobj_->GetPosition().z },
		{ 0.1f,0.1f,0.1f }, { 1,1,1,1 });
	explo_->SetIsBillboard(true);

	shadow_ = Object2d::Create(ImageManager::shadow_, { 0,0,0 },
		{ 0.2f,0.2f,0.2f }, { 1,1,1,1 });
	shadow_->SetRotation({ DEGREE_QUARTER,0,0 });

	for (int i = 0; i < StateMax;i++) {
		status_[i] = Object2d::Create(ImageManager::Battle+i, { fbxobj_->GetPosition().x,fbxobj_->GetPosition().y + 1.0f,fbxobj_->GetPosition().z
			}, { 0.15f,0.15f,0.15f }, { 1,1,1,1 });
		status_[i]->SetIsBillboard(true);
		status_[i]->SetRotation({ 0,0,0 });
	}
	audio_ = std::make_unique<AudioManager>();
	audio_->LoadWave("SE/attack.wav");
	audio_->LoadWave("SE/gnormDead.wav");

	OnInitialize();
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
		CommonUpdate();
		//関数ポインタで状態管理
		(this->*statusFuncTable[static_cast<size_t>(command_)])();
		OnUpdate();
		LimitArea();
		TraceUpdate();
	}
}

void Bullet::IntroUpdate(const float& timer, const int& _stage) {
	if (!isActive) { return; }
	const float delay = 0.5f;
	const float first_hight = 100;
	XMFLOAT3 player_pos = player->GetPosition();
	if (timer + delay <= 1.0f) {
		hight = Ease(In, Linear, timer + delay, first_hight, 0);
	}
	fbxobj_->Update();
	fbxobj_->SetRotation({ 0,DirRotation(player_pos),0 });
	const float angle = DEGREE_HALF / 10;
	const float radius = 7.0f;
	fbxobj_->SetPosition({ player_pos.x + sinf(((int)ID) * angle * (XM_PI / DEGREE_HALF)) * radius, hight,  player_pos.z + cos(((int)ID) * angle * (XM_PI / DEGREE_HALF)) * radius });
	IntroOnUpdate(timer);

	if (_stage == SceneNum::kSecondScene) {
		fbxobj_->SetRotation({ 0,180,0 });
		fbxobj_->SetPosition({ (((int)ID % 10) - 4.5f) * 3.0f, 0, ((int)ID / 10) * 5.0f });
		fbxobj_->Update();
	}
}

void Bullet::ResultUpdate(const float& timer) {
	isFinish = true;
	ResultOnUpdate(timer);
	if (burning) {
		BurnOut();
	}
	TraceUpdate();
	explo_->Update();
	for (std::unique_ptr<Object2d>& state : status_) {
		state->Update();
	}
	ShadowUpdate();
}

void Bullet::SetAggregation() {
	XMFLOAT3 pos = player->GetAFTIMAGE(ID % 7);
	XMFLOAT3 BulletPos = fbxobj_->GetPosition();
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
	if (Collision::CircleCollision(fbxobj_->GetPosition().x, fbxobj_->GetPosition().z, 3.0f, player->GetPosition().x, player->GetPosition().z, 3.0f)) {
		rotation = Ease(In, Quad, 0.5f, fbxobj_->GetRotation().y, pos.y);
	} else {
		rotation = (atan2f(position.x, position.z) * (DEGREE_HALF / XM_PI)); //- 90;// *(XM_PI / 180.0f);
	}
	fbxobj_->SetPosition(BulletPos);
	fbxobj_->SetRotation({ 0,rotation,0 });
}

void Bullet::LimitArea() {
	XMFLOAT3 pos = fbxobj_->GetPosition();
	const float limit_ = 48.0f;

	pos.x = min(pos.x, limit_);
	pos.x = max(pos.x, -limit_);

	pos.z = min(pos.z, limit_);
	pos.z = max(pos.z, -limit_);
	fbxobj_->SetPosition(pos);
}

void Bullet::CommonUpdate() {
	old_pos = fbxobj_->GetPosition();
	if (collide) { collide = false; }
	if (isPlayActive) { command_ = BulletStatus::Control; }
	if (DeadFlag){ command_ = BulletStatus::Dead; }
	if (!wait) { follow_frame = 0.0f; }
	if (!noBoss) {
		if (enemy->GetHp() <= 0) {
			clear_s_pos = fbxobj_->GetPosition();
			command_ = BulletStatus::Smash;
		}
	}
	if (burning) {
		BurnOut();
	}
	fbxobj_->Update();
	explo_->Update();
	ShadowUpdate();
	for (std::unique_ptr<Object2d>& state : status_) {
		state->Update();
		state->SetPosition({ fbxobj_->GetPosition().x,fbxobj_->GetPosition().y + 2.5f,fbxobj_->GetPosition().z });
	}
}


void Bullet::FirstDraw(DirectXCommon* dxCommon) {
	if (isActive) {
		Object2d::PreDraw();
		for (std::unique_ptr<Trace>& trace : traces_) {
			trace->Draw();
		}
		shadow_->Draw();
		OnFirstDraw(dxCommon);
	}
}

void Bullet::Draw(DirectXCommon* dxCommon) {
	if (ID==0) {
		ImGui::SetNextWindowPos(ImVec2(0, 500));
		ImGui::Begin("bullet");
		ImGui::Text("pos.y:%f", fbxobj_->GetPosition().y);
		ImGui::Text("frame:%f", frame);

		ImGui::End();
	}
	if (isActive) {
		Object3d::PreDraw();
		fbxobj_->Draw(dxCommon->GetCmdList());
		OnDraw(dxCommon);
	}
}

void Bullet::LastDraw(DirectXCommon* dxCommon) {
	if (isActive) {
		if (noBoss) { 
			if (command_ == BulletStatus::Control) {
				Object2d::PreDraw();
				status_[ControlState]->Draw();
			}
			return;
		}
		if (command_ == BulletStatus::Dead) {
			Object2d::PreDraw();
			chara_dead_->Draw();
			return;
		}
		if (enemy->GetIsActive()) {
			if (isFinish) { return; }
			if (command_ == BulletStatus::Wait) { return; }
			Object2d::PreDraw();
				if (command_ == BulletStatus::Attack) {
					status_[BattleState]->Draw();
				} else  if (command_ == BulletStatus::Battle) {
					if (next_command_ == BulletStatus::Attack) {
						status_[BattleState]->Draw();
					}
				}else if(command_ == BulletStatus::Scary) {
					status_[ScaryState]->Draw();
				} else if (command_ == BulletStatus::Vanish) {
					status_[VanishState]->Draw();
				} else if (command_ == BulletStatus::Control){
					status_[ControlState]->Draw();
				}
 			if (burning) { explo_->Draw(); }
		}
		OnLastDraw(dxCommon);
	}
}

void Bullet::DemoDraw(DirectXCommon* dxCommon) {
	if (isActive) {
		DebugUpdate();
		Object3d::PreDraw();
		fbxobj_->Draw(dxCommon->GetCmdList());
		OnDraw(dxCommon);
	}
}

void Bullet::Finalize() {
}

float Bullet::DirRotation(const XMFLOAT3& target) {
	float itr{};
	XMFLOAT3 pos = fbxobj_->GetPosition();
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
	before_pos = fbxobj_->GetPosition();
	after_pos = target;
}
void Bullet::SetCommand(const BulletStatus& command, XMFLOAT3 pos) {
	this->command_ = command; after_pos = pos;
}

void Bullet::OnCollision(const std::string& Tag, const XMFLOAT3& pos) {
	switch (command_) {
	case BulletStatus::Wait:
		if (Tag == "Enemy") {
			if (isPlayActive) { return; }
			ScaryInit();
		}
		break;
	case BulletStatus::Attack:
		//プレイヤーとの当たり判定
		if (Tag == "Player") {
			if (isPlayActive) { return; }
			player->SetStock(player->GetStock() + 1);
			command_ = BulletStatus::Wait;
		}
		//蜂蜜移動
		if (Tag == "Honey") {
			if (isPlayActive) { return; }
			ActionActor = ActorManager::GetInstance()->GetAreaActor(fbxobj_->GetPosition(), "Honey");
			if (!ActionActor->GetCollide()) {
				ActionActor->SetCollide(true);
			}

		}
		//敵との当たり判定
		if (Tag == "Enemy") {
			if (isPlayActive) { return; }
			DamageInit();
		}
		break;
	case BulletStatus::Battle:
		break;
	case BulletStatus::Control:
		break;

	case BulletStatus::Slow:
		if (Tag == "Enemy") {
			if (isPlayActive) { return; }
			DamageInit();
		}
		break;
	case BulletStatus::Smash:
		break;
	case BulletStatus::Dead:
		break;
	case BulletStatus::Ditch:
		break;
	case BulletStatus::Vanish:
		break;
	case BulletStatus::Scary:
		//プレイヤーとの当たり判定
		if (Tag == "Player") {
			if (isPlayActive) { return; }
			player->SetStock(player->GetStock() + 1);
			command_ = BulletStatus::Wait;
		}

		break;

	default:
		assert(0);
		break;
	}
}


bool Bullet::Follow2Position(const XMFLOAT3& _pos, const float& _radius) {
	XMFLOAT3 pos = fbxobj_->GetPosition();
	XMFLOAT3 position{};
	position.x = (_pos.x - pos.x);
	position.z = (_pos.z - pos.z);
	vel_follow.x = sin(-atan2f(position.x, position.z)) * follow_vel_;
	vel_follow.y = cos(-atan2f(position.x, position.z)) * follow_vel_;
	if (powf(position.x, 2) + powf(position.z, 2) > powf(_radius, 2)) {
		pos.x -= vel_follow.x;
		pos.z += vel_follow.y;
		fbxobj_->SetPosition(pos);
		fbxobj_->SetRotation({ 0 ,DirRotation(_pos),0 });
		return true;
	} else {
		return false;
	}
}



void Bullet::DamageInit(BulletStatus status) {
	if (command_ != BulletStatus::Battle) {
		audio_->PlayWave("SE/attack.wav", 0.5f);
		if (next_command_ == BulletStatus::Attack) {
			enemy->SetDamage(enemy->GetDamage() + 1);
		}
		burning = true;
		frame = 0.0f;
		XMFLOAT3 pos = fbxobj_->GetPosition();
		fall = kFallHeight;
		exploPos = old_pos;
		XMFLOAT3 distance = { pos.x - old_pos.x,0,pos.z - old_pos.z };
		s_rebound_ = pos;
		e_rebound_ = {
		pos.x - sinf(atan2f(distance.x, distance.z)) * 10.0f,
		0,
		pos.z - cosf(atan2f(distance.x, distance.z)) * 10.0f
		};
		next_command_ = status;
		command_ = BulletStatus::Battle;
	}
}

void Bullet::ScaryInit() {
	if (command_ == BulletStatus::Scary) { return; }
	std::mt19937 mt{ std::random_device{}() };
	std::uniform_int_distribution<int> dist(1, 100);
	int rand = dist(mt);
	if(rand > 5) {
		DamageInit(BulletStatus::Wait);
		return;
	}

	frame = 0.0f;
	XMFLOAT3 pos = fbxobj_->GetPosition();
	XMFLOAT3 distance = { pos.x - old_pos.x,0,pos.z - old_pos.z };
	after_pos = {
	pos.x - sinf(atan2f(distance.x, distance.z)) * 47.0f,
	0,
	pos.z - cosf(atan2f(distance.x, distance.z)) * 47.0f
	};
	command_ = BulletStatus::Scary;
}



void Bullet::BurnOut() {
	if (effectRate < 1.0f) {
		effectRate += 0.1f;
	} else {
		effectRate = 0;
		explo_->SetScale({ 0,0,0 });
		burning = false;
		return;
	}
	float scale = Ease(In, Quad, effectRate, 0.0f, 1.0f);
	explo_->SetScale({ scale,scale,scale });
	explo_->SetPosition(exploPos);
}

void Bullet::ShadowUpdate() {
	//影の大小
	XMFLOAT3 pos = fbxobj_->GetPosition();
	float max_height_ = 12.0f;
	float scale = ((max_height_ - pos.y) / max_height_) * shadow_side_;
	scale = max(0.0f, scale);

	shadow_->SetScale({ scale,scale, scale, });
	shadow_->Update();
	shadow_->SetPosition({ pos.x,0.01f, pos.z });
}

void Bullet::TraceUpdate() {
	//足跡の生成
	foot_count_--;
	if (foot_count_ <= 0) {
		float foot_rot = fbxobj_->GetPosition().y;
		Trace::ImageFoot imagefoot_;
		if (odd_count_ % 2 == 0) {
			imagefoot_ = Trace::ImageFoot::LeftFoot;
		} else {
			imagefoot_ = Trace::ImageFoot::RightFoot;
		}
		std::unique_ptr<Trace> Trace_ = std::make_unique<Trace>(imagefoot_, foot_rot, fbxobj_->GetPosition());
		traces_.push_back(std::move(Trace_));
		foot_count_ = kFootCountMax;
		odd_count_++;
	}
	//足跡の更新と削除処理
	for (std::unique_ptr<Trace>& trace : traces_) {
		trace->Update();
	}
	traces_.remove_if([](std::unique_ptr<Trace>& trace) {
		return trace->GetLife() <= 0;
		});
}

void Bullet::ControlUpdate() {
	if (!dig_action) {
		if (!Follow2Position(ActionActor->GetPosition(), ActionActor->GetSize())) {
			audio_->PlayWave("SE/attack.wav", 0.5f);
			burning = true;
			XMFLOAT3 pos = fbxobj_->GetPosition();
			fall = kFallHeight;
			exploPos = ActionActor->GetPosition();
			XMFLOAT3 distance = { pos.x - ActionActor->GetPosition().x,0,pos.z - ActionActor->GetPosition().z };
			s_rebound_ = pos;
			e_rebound_ = {
			pos.x + sinf(atan2f(distance.x, distance.z)) * 5.0f,
			0,
			pos.z + cosf(atan2f(distance.x, distance.z)) * 5.0f
			};
			dig_action = true;
		}
	} else {
		XMFLOAT3 pos = fbxobj_->GetPosition();

		if (damageframe >= 1.0f) {
			damageframe = 0.0f;
			dig_action = false;
			pos.y = 0.0f;
			fbxobj_->SetPosition(pos);
			return;
		} else {
			damageframe += 1.0f / kDeadFrameMax;
		}
		pos.x = Ease(In, Quad, damageframe, s_rebound_.x, e_rebound_.x);
		pos.y += fall; //+
		fall -= kFallHeight / (kDeadFrameMax / 2.0f);//
		pos.y = max(0.0f, pos.y);
		pos.z = Ease(In, Quad, damageframe, s_rebound_.z, e_rebound_.z);
		fbxobj_->SetPosition(pos);

	}


	if (navi) {
		if (navi_frame > 1.0f) {
			command_ = BulletStatus::Attack;
			navi_frame = 1.0f;
			isPlayActive = false;
			navi = false;
		} else {
			navi_frame += 0.008f;
		}
		XMFLOAT3 pos{};
		pos.x = Ease(In, Linear, navi_frame, before_pos.x, (after_pos.x + margin));
		pos.y = Ease(In, Linear, navi_frame, before_pos.y, after_pos.y);
		pos.z = Ease(In, Linear, navi_frame, before_pos.z, (after_pos.z + margin));
		fbxobj_->SetPosition(pos);
	}
}

void Bullet::SmashUpdate() {
	//ステータスの非表示
	status_alpha_ = Ease(In, Linear, 0.8f, status_alpha_, 0);
	for (std::unique_ptr<Object2d>& state: status_) {
		state->SetColor({ 1,1,1,status_alpha_ });
	}
}

void Bullet::DitchUpdate() {
}

void Bullet::VanishUpdate() {
		XMFLOAT3 pos = fbxobj_->GetPosition();
		XMFLOAT3 rot = fbxobj_->GetRotation();
		pos.x = Ease(InOut, Quad, frame, pos.x, after_pos.x);
		pos.y += vanish_vel_; //+
		vanish_vel_ -= kVanishHight / (kVanishFrameMax / 2.5f);//
		pos.y = max(0, pos.y);
		pos.z = Ease(InOut, Quad, frame, pos.z, after_pos.z);

		rot.y = Ease(In, Quad, frame, 0, -DEGREE_MAX * 3.0f);

		if (frame < 1.0f) {
			frame += 1.0f / kVanishFrameMax;
		} else {
			frame = 0.0f;
			pos.y = 0.0f;
			rot.y = 0;
			command_ = BulletStatus::Wait;

			vanish_vel_ = kVanishHight;
			throwReady = false;
		}
		fbxobj_->SetPosition(pos);
		fbxobj_->SetRotation(rot);
}

void Bullet::ScaryUpdate() {
	XMFLOAT3 pos = fbxobj_->GetPosition();
	XMFLOAT3 rot = fbxobj_->GetRotation();
	pos.x = Ease(InOut, Quad, frame, pos.x, after_pos.x);
	pos.z = Ease(InOut, Quad, frame, pos.z, after_pos.z);

	if (frame < 1.0f) {
		frame += 1.0f / kScaryFrameMax;
		rot.y = DirRotation(after_pos);
	} else {
		frame = 1.0f;
		rot.y = DirRotation(player->GetPosition());
	}
	fbxobj_->SetPosition(pos);
	fbxobj_->SetRotation(rot);
}

void Bullet::WaitUpdate() {
	throwReady = true;
	vel_ = 0.8f;
	frame = 0.0f;
	//投げてる最中の回収時の矛盾解消
	XMFLOAT3 pos = fbxobj_->GetPosition();
	if (pos.y > 0) {
		pos.y -= 0.3f;
	}
	pos.y = max(0, pos.y);
	fbxobj_->SetPosition(pos);
	SetAggregation();
}

void Bullet::SlowUpdate() {
	if (wait) { wait = false; follow_frame = 0.0f; }
	if (throwReady) {
		XMFLOAT3 pos = fbxobj_->GetPosition();
		XMFLOAT3 rot = fbxobj_->GetRotation();
		pos.x = Ease(InOut, Quad, frame, pos.x, after_pos.x);
		pos.y += vel_; //+
		vel_ -= kSlowHight / (kSlowFrameMax / 2.5f);//
		pos.y = max(0, pos.y);
		pos.z = Ease(InOut, Quad, frame, pos.z, after_pos.z);

		const float delay = 0.3f;
		if (frame < 1.0f - delay) {
			rot.x = Ease(In, Quad, frame + delay, 0, -DEGREE_MAX);
		}

		if (frame < 1.0f) {
			frame += 1.0f / kSlowFrameMax;
		} else {
			frame = 0.0f;
			pos.y = 0.0f;
			rot.x = 0;
			vel_ = kSlowHight;
			throwReady = false;
		}
		fbxobj_->SetPosition(pos);
		fbxobj_->SetRotation(rot);

	} else {
		fbxobj_->SetRotation({ 0,fbxobj_->GetRotation().y,0 });
		command_ = BulletStatus::Attack;
	}

}

void Bullet::DeadUpdate() {
	if (deadframe == 0.0f) {
		audio_->PlayWave("SE/gnormDead.wav", 0.2f);
	}
	if (deadframe >= 1.0f) {
		isRemove = true;
		isActive = false;
	} else {
		deadframe += 0.015f;
	}
	chara_dead_->Update();
	//イージング処理
	vanishHight = Ease(Out, Quad, deadframe, 0.1f, 4.5f);
	vanishAlpha = Ease(In, Quad, deadframe, 1.0f, 0.5f);
	//更新処理
	fbxobj_->SetScale({ 0.003f,0.0001f, 0.003f });
	fbxobj_->Update();
	shadow_->Update();
	shadow_->SetPosition({ fbxobj_->GetPosition().x,0.01f, fbxobj_->GetPosition().z });
	chara_dead_->SetPosition({ fbxobj_->GetPosition().x,vanishHight, fbxobj_->GetPosition().z });
	chara_dead_->SetColor({ 1,1,1,vanishAlpha });

}

void Bullet::AttackUpdate() {
	if (noBoss) { return; }
	if (Collision::CircleCollision(fbxobj_->GetPosition().x, fbxobj_->GetPosition().z, 15.0f, enemy->GetPosition().x, enemy->GetPosition().z, 1.0f)) {
		Follow2Position(enemy->GetPosition(), 2.0f);
	}
}

void Bullet::BattleUpdata() {
	XMFLOAT3 pos = fbxobj_->GetPosition();

	if (damageframe >= 1.0f) {
		damageframe = 0.0f;
		command_ = next_command_;
		pos.y = 0.0f;
		fbxobj_->SetPosition(pos);
		return;
	} else {
		damageframe += 1.0f / kDeadFrameMax;
	}
	pos.x = Ease(In, Quad, damageframe, s_rebound_.x, e_rebound_.x);
	pos.y += fall; //+
	fall -= kFallHeight / (kDeadFrameMax / 2.0f);//
	pos.y = max(0.0f, pos.y);
	pos.z = Ease(In, Quad, damageframe, s_rebound_.z, e_rebound_.z);
	fbxobj_->SetPosition(pos);

}
