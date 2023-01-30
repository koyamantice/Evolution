#include"Player.h"
#include"ModelManager.h"
#include"ImageManager.h"
#include"Collision.h"
#include <SourceCode/FrameWork/collision/CollisionManager.h>
#include <SourceCode/FrameWork/collision/SphereCollider.h>
#include "CollisionAttribute.h"
#include<fstream>
#include <SourceCode/FrameWork/ActorManager.h>
#include"PlayerUI.h"
#include "ParticleManager.h"
#include <Easing.h>




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
			vel = (float)std::atof(word.c_str());
		} else if (word.find("STOCK") == 0) {
			getline(line_stream, word, ',');
			stock = (int)std::atof(word.c_str());

			break;
		}
	}
}

void Player::IntroOnUpdate(const float& Timer) {
	fbxObj->Update();
	IntroMove();
	compornent->SetIsActive(false);
}

void Player::IntroMove() {
	fbxObj->SetPosition(obj->GetPosition());
	fbxObj->SetRotation(obj->GetRotation());
}

void Player::ResultOnUpdate(const float& Timer) {
	fbxObj->Update();
	compornent->SetIsActive(false);
}

void Player::OnInit() {
	obj->SetRotation(XMFLOAT3{ 0,0,0 });
	obj->SetPosition({ 0,0,20 });
	
	isVisible = false;

	LoadData();
	UpdateCommand();

	vel /= 5.0f;

	FBXObject3d* fbxObj_ = new FBXObject3d();
	fbxObj_->Initialize();
	fbxObj_->SetModel(ModelManager::GetIns()->GetFBXModel(ModelManager::kSeed));
	fbxObj_->SetScale({ 0.005f,0.005f, 0.005f });
	fbxObj.reset(fbxObj_);
	fbxObj->LoadAnimation();
	fbxObj->PlayAnimation();

	// キャラクターの初期位置をセット
	for (int i = 0; i < AFTIMAGENUM; i++) {
		PlayerX[i] = 0;
		PlayerZ[i] = 18.0f + 10.0f * ((float)(AFTIMAGENUM - i) / AFTIMAGENUM);
	}
	
	//足元のパーティクル
	particleEmitter_ = std::make_unique <ParticleEmitter>(ImageManager::smoke);

	LockOn = std::make_unique<Aim>();
	LockOn->Init();

	compornent = new PlayerUI();
	compornent->Initialize();

	Object2d* Shadow_ = Object2d::Create(ImageManager::Shadow, { 0,0,0 },
		{ 0.2f,0.2f,0.2f }, { 1,1,1,1 });
	Shadow_->Object2dCreate();
	Shadow_->SetRotation({ DEGREE_QUARTER,0,0 });
	Shadow.reset(Shadow_);
}

void Player::OnUpda() {
	particleEmitter_->Update();
	compornent->SetIsActive(true);
	if (!first) {
		LockOn->FirstSet();
		first = true;
	}
	if (canMove) {
		Move();
	}
	HitBoundMotion();
	LimitArea();
	fbxObj->Update();
	fbxObj->SetPosition(obj->GetPosition());
	Shadow->Update();
	Shadow->SetPosition({ obj->GetPosition().x,0.01f, obj->GetPosition().z });
	fbxObj->SetRotation(obj->GetRotation());
	LockOn->Upda(angle);
}

void Player::OnFirstDraw(DirectXCommon* dxCommon) {
	Object2d::PreDraw();
	Shadow->Draw();
	particleEmitter_->Draw(alphaBle);
}

void Player::OnDraw(DirectXCommon* dxCommon) {
	fbxObj->Draw(dxCommon->GetCmdList());
}

void Player::OnLastDraw(DirectXCommon* dxCommon) {
	LockOn->Draw();
}

void Player::OnFinal() {
}

void Player::Move() {
	XMFLOAT3 pos = obj->GetPosition();
	XMFLOAT3 rot = obj->GetRotation();
	aftImage_count_++;
	if (aftImage_count_ >= kAftLocateCountMax) {
		if (((int)pos.x != (int)PlayerX[0] || (int)pos.z != (int)PlayerZ[0]) || !isFasted) {
			// 残像データを一つづつずらす
			for (int i = AFTIMAGENUM - 1; i > 0; i--) {
				PlayerX[i] = PlayerX[i - 1];
				RotY[i] = RotY[i - 1];
				PlayerZ[i] = PlayerZ[i - 1];
			}
		}
		PlayerX[0] = pos.x;
		RotY[0] = rot.y;
		PlayerZ[0] = pos.z;
		aftImage_count_ = 0;
		isFasted = true;
	}
	if (onHoney) {
		vel = 0.2f;
		const int life = 30;
		const float rnd_pos = 2.0f;
		const float rnd_vel = -0.05f;
		particleEmitter_->AddInNest(life, pos, rnd_pos, rnd_vel);

		honeyCount++;
		if (honeyCount >= 60) {
			onHoney = false;
			honeyCount = 0;
		}
	} else {
		vel = 0.5f;
	}

	if (input->TiltPushStick(Input::L_UP, 0.0f) ||
		input->TiltPushStick(Input::L_DOWN, 0.0f) ||
		input->TiltPushStick(Input::L_RIGHT, 0.0f) ||
		input->TiltPushStick(Input::L_LEFT, 0.0f)) {

		float StickX = input->GetLeftControllerX();
		float StickY = input->GetLeftControllerY();
		const float STICK_MAX = 32767.0f;
		
		//移動処理

		if (input->TiltPushStick(Input::L_UP, 0.0f)) {
			XMFLOAT3 vecvel = MoveVECTOR(XMVECTOR{ 0,0,vel,0 }, angle);
			pos.x -= vecvel.x * (StickY / STICK_MAX);
			pos.z -= vecvel.z * (StickY / STICK_MAX);
		}
		if (input->TiltPushStick(Input::L_DOWN, 0.0f)) {
			XMFLOAT3 vecvel = MoveVECTOR(XMVECTOR{ 0,0,-vel,0 }, angle);
			pos.x += vecvel.x * (StickY / STICK_MAX);
			pos.z += vecvel.z * (StickY / STICK_MAX);
		}
		if (input->TiltPushStick(Input::L_RIGHT, 0.0f)) {
			XMFLOAT3 vecvel = MoveVECTOR(XMVECTOR{ vel,0,0,0 }, angle);
			pos.x -= vecvel.x * (StickX / STICK_MAX);
			pos.z -= vecvel.z * (StickX / STICK_MAX);
		}
		if (input->TiltPushStick(Input::L_LEFT, 0.0f)) {
			XMFLOAT3 vecvel = MoveVECTOR(XMVECTOR{ -vel,0,0,0 }, angle);
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
			rot.y = (float)((int)rot.y %(int)DEGREE_MAX);
		}
		obj->SetPosition(pos);
		obj->SetRotation(rot);
	}

}

void Player::OnCollision(const std::string& Tag) {
	if (Tag == "Enemy") {
	}
	if (Tag == "Bullet") {

	}
	if (Tag == "ClearCrystal") {
	}
	if (Tag == "Honey") {
		if (!onHoney) {
			speed = vel;
			onHoney = true;
		}
	}
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
				knock_back_frame_ += 1.0f/ kKnockBackFrameMax;
			}

			pos = {
			Ease(InOut,Quad,knock_back_frame_,s_rebound_pos_.x,e_rebound_pos_.x),
			0,
			Ease(InOut,Quad,knock_back_frame_,s_rebound_pos_.z,e_rebound_pos_.z)
			};

			float spinning = collided_rot_+ (DEGREE_MAX * 3);
			rot = Ease(InOut, Quad, knock_back_frame_, collided_rot_, spinning);

			obj->SetRotation({ 0,rot,0 });
			obj->SetPosition(pos);
		}
	} else {
	}
}

void Player::LimitArea() {
	XMFLOAT3 pos = obj->GetPosition();
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
	obj->SetPosition(pos);
}

XMFLOAT3 Player::MoveVECTOR(XMVECTOR v, float angle) {
	XMMATRIX rot = XMMatrixRotationY(XMConvertToRadians(angle));
	v = XMVector3TransformNormal(v, rot);
	XMFLOAT3 pos = { v.m128_f32[0],v.m128_f32[1] ,v.m128_f32[2] };
	return pos;
}

const DirectX::XMFLOAT3& Player::GetCameraPos(const float& angle, const float& str) {
	XMFLOAT3 pos = obj->GetPosition();
	cameraPos = MoveVECTOR(XMVECTOR{ 0,0,str,0 }, angle);
	cameraPos = { pos.x - cameraPos.x,pos.y - cameraPos.y,pos.z - cameraPos.z };
	return cameraPos;
}
