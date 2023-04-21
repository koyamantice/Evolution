#include"Player.h"
#include"ModelManager.h"
#include"ImageManager.h"
#include<fstream>
#include <SourceCode/FrameWork/ActorManager.h>
#include"PlayerUI.h"
#include <Easing.h>
#include <Helper.h>
#include "Collision.h"


void Player::LoadData() {
	std::ifstream file;
	file.open("Resources/csv/status.csv");
	assert(file.is_open());

	parameterCommands << file.rdbuf();

	file.close();
}

void Player::UpdateCommand() {
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
			vel_ = (float)std::atof(word.c_str());
		} else if (word.find("STOCK") == 0) {
			getline(line_stream, word, ',');
			stock = (int)std::atof(word.c_str());

			break;
		}
	}
}

void Player::IntroOnUpdate(const float& timer) {
	fbxobj_->Update();
	IntroMove();
	compornent->SetIsActive(false);
}

void Player::IntroMove() {
	fbxobj_->SetPosition(obj->GetPosition());
	fbxobj_->SetRotation(obj->GetRotation());
}

void Player::ResultOnUpdate(const float& timer) {
	fbxobj_->Update();
	aim_->Upda(angle);
	XMFLOAT3 pos = fbxobj_->GetPosition();
	if (pos.y <= 0.0f) {
		pos.y = max(0.0f, pos.y);
		if (jump_count % 2 == 0) {
			y_add = 0.2f;  // ジャンプするため重力加速度をマイナスにする
		} else {
			y_add = 0.3f;  // ジャンプするため重力加速度をマイナスにする
		}
		jump_count++;
	}
	pos.y += y_add;
	y_add -= 0.02f;
	ShadowUpda();
	TraceUpda();
	fbxobj_->SetPosition(pos);
	compornent->SetIsActive(false);
	aim_->SetIsActive(false);
}

void Player::OnInitialize() {
	obj->SetRotation(XMFLOAT3{ 0,0,0 });
	obj->SetPosition({ 0,0,20 });

	isVisible = false;

	LoadData();
	UpdateCommand();


	fbxobj_ = std::make_unique<FBXObject3d>();
	fbxobj_->Initialize();
	fbxobj_->SetModel(ModelManager::GetInstance()->GetFBXModel(ModelManager::kSeed));
	fbxobj_->SetScale({ 0.005f,0.005f, 0.005f });
	fbxobj_->LoadAnimation();
	fbxobj_->PlayAnimation();

	// キャラクターの初期位置をセット
	for (int i = 0; i < AFTIMAGENUM; i++) {
		imagin_[i].x = 0;
		imagin_[i].z = 18.0f + 10.0f * ((float)(AFTIMAGENUM - i) / AFTIMAGENUM);
	}

	//足元のパーティクル
	particleEmitter_ = std::make_unique <ParticleEmitter>(ImageManager::smoke);

	aim_ = std::make_unique<Aim>();
	aim_->Init();

	compornent = new PlayerUI();
	compornent->Initialize();

	shadow_ = Object2d::Create(ImageManager::shadow_, { 0,0,0 },
		{ 0.2f,0.2f,0.2f }, { 1,1,1,1 });
	shadow_->SetRotation({ DEGREE_QUARTER,0,0 });
}

void Player::OnUpdate() {
	particleEmitter_->Update();
	compornent->SetIsActive(true);
	if (!first) {
		aim_->FirstSet();
		first = true;
	}
	if (canMove) {
		Move();
	}
	HitBoundMotion();
	LimitArea();
	ShadowUpda();
	TraceUpda();
	fbxobj_->Update();
	fbxobj_->SetPosition(obj->GetPosition());
	fbxobj_->SetRotation(obj->GetRotation());
	aim_->Upda(angle);
}

void Player::OnFirstDraw(DirectXCommon* dxCommon) {
	Object2d::PreDraw();
	//ImGui::SetNextWindowPos(ImVec2(0, 0));
	//ImGui::SetNextWindowSize(ImVec2(180, 250));

	//ImGui::Begin("enemy");
	//ImGui::Text("imagin_[0].x:%f", imagin_[0].x);
	//ImGui::Text("imagin_[1].x:%f", imagin_[1].x);
	//ImGui::Text("imagin_[2].x:%f", imagin_[2].x);
	//ImGui::Text("imagin_[3].x:%f", imagin_[3].x);

	//ImGui::End();

	shadow_->Draw();
	for (std::unique_ptr<Trace>& trace : traces_) {
		trace->Draw();
	}
	particleEmitter_->Draw(alphaBle);
}

void Player::OnDraw(DirectXCommon* dxCommon) {
	fbxobj_->Draw(dxCommon->GetCmdList());
}

void Player::OnLastDraw(DirectXCommon* dxCommon) {
	if (canMove) {
		aim_->Draw();
	}
}

void Player::OnFinalize() {
}

void Player::Move() {
	XMFLOAT3 pos = obj->GetPosition();
	XMFLOAT3 rot = obj->GetRotation();
	aftImage_count_++;
	if (aftImage_count_ >= kAftLocateCountMax) {
		if (((int)pos.x != (int)imagin_[0].x || (int)pos.z != (int)imagin_[0].z) || !isFasted) {
			// 残像データを一つづつずらす
			for (int i = AFTIMAGENUM - 1; i > 0; i--) {
				imagin_[i].x = imagin_[i - 1].x;
				imagin_[i].y = imagin_[i - 1].y;
				imagin_[i].z = imagin_[i - 1].z;
			}
		}
		imagin_[0].x = pos.x;
		imagin_[0].y = rot.y;
		imagin_[0].z = pos.z;
		aftImage_count_ = 0;
		isFasted = true;
	}
	if (onHoney) {
		honeyCount++;

		vel_ = 0.2f;
		const int life = 30;
		const float rnd_pos = 2.0f;
		const float rnd_vel = -0.05f;
		particleEmitter_->AddInNest(life, pos, rnd_pos, rnd_vel);

		if (honeyCount >= kHoneyCountMax) {
			onHoney = false;
			honeyCount = 0;
		}
	} else {
		vel_ = 0.5f;
	}

	if (PushMoveButton()) {
		foot_count_--;
		float StickX = input->GetLeftControllerX();
		float StickY = input->GetLeftControllerY();
		const float STICK_MAX = 32767.0f;
		if (input->PushKey(DIK_W)) {
			StickY = STICK_MAX;
		} else if (input->PushKey(DIK_S)) {
			StickY = -STICK_MAX;

		} else if (input->PushKey(DIK_D)) {
			StickX = STICK_MAX;
		} else if (input->PushKey(DIK_A)) {
			StickX = -STICK_MAX;
		}

		//移動処理

		if (input->TiltPushStick(Input::L_UP, 0.0f) ||
			input->PushKey(DIK_W)) {
			XMFLOAT3 vecvel = MoveVECTOR(XMVECTOR{ 0,0,vel_,0 }, angle);
			pos.x -= vecvel.x * (StickY / STICK_MAX);
			pos.z -= vecvel.z * (StickY / STICK_MAX);
		}
		if (input->TiltPushStick(Input::L_DOWN, 0.0f) ||
			input->PushKey(DIK_S)) {
			XMFLOAT3 vecvel = MoveVECTOR(XMVECTOR{ 0,0,-vel_,0 }, angle);
			pos.x += vecvel.x * (StickY / STICK_MAX);
			pos.z += vecvel.z * (StickY / STICK_MAX);
		}
		if (input->TiltPushStick(Input::L_RIGHT, 0.0f) ||
			input->PushKey(DIK_D)) {
			XMFLOAT3 vecvel = MoveVECTOR(XMVECTOR{ vel_,0,0,0 }, angle);
			pos.x -= vecvel.x * (StickX / STICK_MAX);
			pos.z -= vecvel.z * (StickX / STICK_MAX);
		}
		if (input->TiltPushStick(Input::L_LEFT, 0.0f) ||
			input->PushKey(DIK_A)) {
			XMFLOAT3 vecvel = MoveVECTOR(XMVECTOR{ -vel_,0,0,0 }, angle);
			pos.x += vecvel.x * (StickX / STICK_MAX);
			pos.z += vecvel.z * (StickX / STICK_MAX);
		}


		//足元の煙
		particle_pop_time_++;

		if (particle_pop_time_ > kPopTimeMax) {
			const float rnd_vel = 0.1f;
			const int life = 60;
			const float s_sce = 0.5f;
			particleEmitter_->AddCommon(life, pos, rnd_vel, 0.0f, s_sce, 0.0f);
			particle_pop_time_ = 0;
		}
		//向き
		rot.y = angle + (atan2f(StickX / STICK_MAX, StickY / STICK_MAX) * (DEGREE_HALF / XM_PI));
		if (rot.y >= 0) {
			rot.y = (float)((int)rot.y % (int)DEGREE_MAX);
		} else {
			rot.y += DEGREE_MAX;
			rot.y = (float)((int)rot.y % (int)DEGREE_MAX);
		}
		foot_rot_ = rot.y - DEGREE_HALF;
		obj->SetPosition(pos);
		obj->SetRotation(rot);
	}

}

bool Player::PushMoveButton() {
	if (input->TiltPushStick(Input::L_UP, 0.0f) ||
		input->TiltPushStick(Input::L_DOWN, 0.0f) ||
		input->TiltPushStick(Input::L_RIGHT, 0.0f) ||
		input->TiltPushStick(Input::L_LEFT, 0.0f) ||
		input->PushKey(DIK_W) ||
		input->PushKey(DIK_S) ||
		input->PushKey(DIK_D) ||
		input->PushKey(DIK_A)) {
		return true;
	} else {
		return false;
	}
}

void Player::OnCollision(const std::string& Tag) {

	if (Tag == "Enemy") {
		SetHitBound(old_pos);
	}
	if (Tag == "Honey") {
		Actor* honey = ActorManager::GetInstance()->SearchActor(Tag);
		XMFLOAT3 pos = fbxobj_->GetPosition();
		if (Collision::SphereCollision2(pos,1.0f,honey->GetPosition(),5.0f)) {
			if (!onHoney) {
				onHoney = true;
			}
		}
	}
}

void Player::ShadowUpda() {
	XMFLOAT3 pos = fbxobj_->GetPosition();
	float max_height_ = 12.0f;
	float scale = ((max_height_ - pos.y) / max_height_) * shadow_side_;
	scale = max(0.0f, scale);
	shadow_->SetScale({ scale,scale,scale });
	shadow_->Update();
	shadow_->SetPosition({ obj->GetPosition().x,0.01f, obj->GetPosition().z });
}

void Player::TraceUpda() {
	if (foot_count_ <= 0) {
		Trace::ImageFoot imagefoot_;
		if (odd_count_ % 2 == 0) {
			imagefoot_ = Trace::ImageFoot::LeftFoot;
		} else {
			imagefoot_ = Trace::ImageFoot::RightFoot;
		}
		std::unique_ptr<Trace> Trace_ = std::make_unique<Trace>(imagefoot_, foot_rot_, fbxobj_->GetPosition());
		traces_.push_back(std::move(Trace_));
		foot_count_ = kFootCountMax;
		odd_count_++;
	}
	for (std::unique_ptr<Trace>& trace : traces_) {
		trace->Update();
	}
	traces_.remove_if([](std::unique_ptr<Trace>& trace) {
		return trace->GetLife() <= 0;
		});
}

void Player::HitBoundMotion() {
	if (hitBound.isHit) {
		canMove = false;
		XMFLOAT3 pos = obj->GetPosition();
		float rot = obj->GetRotation().y;
		if (!knockBacking) {
			Bullet* bullet = ActorManager::GetInstance()->SearchBulletBack();
			if (bullet != nullptr) {
				bullet->SetDeadFlag(true);
			}
			compornent->SetIsDamage(true);

			collided_rot_ = rot;

			s_rebound_pos_ = pos;

			XMFLOAT3 distance{};
			distance = { old_pos.x - pos.x,0,old_pos.z - pos.z };

			e_rebound_pos_.x = pos.x + sin(atan2f(distance.x, distance.z)) * kKnockBackRange;
			e_rebound_pos_.z = pos.z + cos(atan2f(distance.x, distance.z)) * kKnockBackRange;

			knockBacking = true;
		} else {
			if (knock_back_frame_ >= 1.0f) {
				knock_back_frame_ = 0.0f;
				knockBacking = false;
				hitBound.isHit = false;
				rot = collided_rot_;
				canMove = true;
				obj->SetRotation({ 0,rot,0 });
				obj->SetPosition(pos);
				return;
			} else {
				knock_back_frame_ += 1.0f / kKnockBackFrameMax;
			}

			pos = {
			Ease(InOut,Quad,knock_back_frame_,s_rebound_pos_.x,e_rebound_pos_.x),
			0,
			Ease(InOut,Quad,knock_back_frame_,s_rebound_pos_.z,e_rebound_pos_.z)
			};
			//何回回るか
			float spinning = collided_rot_ + (DEGREE_MAX * 3);
			rot = Ease(InOut, Quad, knock_back_frame_, collided_rot_, spinning);

			obj->SetRotation({ 0,rot,0 });
			obj->SetPosition(pos);
		}
	} else {
	}
}

void Player::LimitArea() {
	XMFLOAT3 pos = obj->GetPosition();

	const float limit_Max = 48.0f;
	const float limit_Min = -48.0f;

	pos.x = clamp(pos.x, limit_Max, limit_Min);
	pos.z = clamp(pos.z, limit_Max, limit_Min);

	obj->SetPosition(pos);
}

XMFLOAT3 Player::MoveVECTOR(XMVECTOR v, float angle) {
	XMMATRIX rot = XMMatrixRotationY(XMConvertToRadians(angle));
	v = XMVector3TransformNormal(v, rot);
	XMFLOAT3 pos = { v.m128_f32[0],v.m128_f32[1] ,v.m128_f32[2] };
	return pos;
}

DirectX::XMFLOAT3 Player::GetCameraPos(const float& angle, const float& str) {
	XMFLOAT3 pos = obj->GetPosition();
	XMFLOAT3 cameraPos = MoveVECTOR(XMVECTOR{ 0,0,str,0 }, angle);
	XMFLOAT3 itr = { pos.x - cameraPos.x,pos.y - cameraPos.y,pos.z - cameraPos.z };
	return itr;
}
