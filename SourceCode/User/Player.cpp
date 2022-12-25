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
#define STICK__MAX 32768.0f



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
		}
		else if (word.find("STOCK") == 0) {
			getline(line_stream, word, ',');
			stock = (int)std::atof(word.c_str());
			
			break;
		}
	}
}

void Player::DebugUpdate() {
	obj->SetScale({ 1.0f,1.0f,1.0f });
}

void Player::IntroOnUpdate(const int& Timer) {
	fbxObj->Update();
	IntroMove();
	compornent->SetIsActive(false);
}

void Player::IntroMove() {
	fbxObj->SetPosition(obj->GetPosition());
	fbxObj->SetRotation(obj->GetRotation());
}

void Player::ResultOnUpdate(const int& Timer) {
}

void Player::OnInit() {
	obj->SetRotation(XMFLOAT3{ 0,0,0 });
	obj->SetPosition({0,0,15});
	isVisible = false;
	LoadData();
	UpdateCommand();

	vel /= 5.0f;

	FBXObject3d* fbxObj_ = new FBXObject3d();
	fbxObj_->Initialize();
	fbxObj_->SetModel(ModelManager::GetIns()->GetFBXModel(ModelManager::Bird));
	fbxObj_->SetScale({ 0.005f,0.005f, 0.005f });
	fbxObj.reset(fbxObj_);
	fbxObj->LoadAnimation();
	fbxObj->PlayAnimation();

	float radius = 1.0f;
	obj->SetCollider(new SphereCollider(XMVECTOR({ 0,radius,0,0 }), radius));
	obj->collider->SetAttribute(COLLISION_ATTR_ALLIES);

	Aim* LockOn_=new Aim();
	LockOn.reset(LockOn_);
	LockOn->Init();

	compornent = new PlayerUI();
	compornent->Initialize();

	Texture* Shadow_ = Texture::Create(ImageManager::Shadow, { 0,0,0 },
		{ 0.2f,0.2f,0.2f }, { 1,1,1,1 });
	Shadow_->TextureCreate();
	Shadow_->SetRotation({ 90,0,0 });
	Shadow.reset(Shadow_);
}

void Player::OnUpda() {
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

void Player::OnDraw(DirectXCommon* dxCommon) {
	fbxObj->Draw(dxCommon->GetCmdList());
	Texture::PreDraw();
	Shadow->Draw();
	LockOn->Draw();
}

void Player::OnFinal() {
}

void Player::Move() {
	XMFLOAT3 pos = obj->GetPosition();
	XMFLOAT3 rot = obj->GetRotation();

	float StickX = input->GetLeftControllerX();
	float StickY = input->GetLeftControllerY();
	const float PI = 3.14159f;
	const float STICK_MAX =32768.0f;
	if (onHoney) {
		vel = speed * 0.65f;
		honeyCount++;
		if (honeyCount==60) {
			onHoney = false;
			honeyCount = 0;
		}
	} else {
		vel = 0.5f;
	}

	if (input->TiltPushStick(Input::L_UP, 0.0f)||
		input->TiltPushStick(Input::L_DOWN, 0.0f)||
		input->TiltPushStick(Input::L_RIGHT, 0.0f)||
		input->TiltPushStick(Input::L_LEFT, 0.0f)) {
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
		const float rnd_vel = 0.1f;
		XMFLOAT3 vel{};
		vel.x = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.y = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.z = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		ParticleManager::GetInstance()->Add(0,15, oldPos, vel, XMFLOAT3(), 1.2f, 0.6f);
		rot.y = angle + (atan2f(StickX,StickY) * (180.0f / XM_PI));

		obj->SetPosition(pos);
		obj->SetRotation(rot);
	}

}



void Player::OnCollision(const std::string& Tag) {
	if (Tag == "Enemy") {
	}
	if (Tag == "Bullet") {
		
	}
	if (Tag == "Crystal") {
		int a = 0;
		a++;
	}
	if (Tag=="Honey") {
		if (!onHoney) {
			speed = vel;
			onHoney = true;
		}
	}
}

void Player::HitBoundMotion() {
	if (hitBound.isHit) {
		canMove = false;
		if (!knockBacking) {
			Bullet* bullet = ActorManager::GetInstance()->SearchBulletBack();
			if (bullet != nullptr) {
				bullet->SetIsRemove(true);
			}
			compornent->SetIsDamage(true);
			XMFLOAT3 pos = obj->GetPosition();
			Start = pos;
			XMFLOAT3 pos2 = hitBound.HitingPos;
			distance = { pos2.x - pos.x,0,pos2.z - pos.z };
			rebound.x = pos.x + sin(atan2f(distance.x, distance.z)) * 15.5f;
			rebound.z = pos.z + cos(atan2f(distance.x, distance.z)) * 15.5f;
			knockBacking = true;
		} else {
			XMFLOAT3 pos = obj->GetPosition();
			float rot = obj->GetRotation().y;
			rot+=30;
			if (damageframe >= 1.0f) {
				//compornent->SetIsDamage(false);
				damageframe = 0.0f;
				knockBacking = false;
				hitBound.isHit = false;
				rot = 0;
				canMove = true;
				obj->SetRotation({ 0,rot,0 });
				obj->SetPosition(pos);
				return;
			} else {
				damageframe += 0.02f;
			}

			pos = {
			Ease(InOut,Quad,damageframe,Start.x,rebound.x),
			0,
			Ease(InOut,Quad,damageframe,Start.z,rebound.z)
			};

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
	rot2 = XMMatrixRotationY(XMConvertToRadians(angle));
	v = XMVector3TransformNormal(v, rot2);
	XMFLOAT3 pos = { v.m128_f32[0],v.m128_f32[1] ,v.m128_f32[2] };
	return pos;
}

const DirectX::XMFLOAT3& Player::GetCameraPos(const float& angle, const float& str) {
	XMFLOAT3 pos = obj->GetPosition();
	cameraPos = MoveVECTOR(XMVECTOR{ 0,0,str,0 }, angle);
	cameraPos = { pos.x - cameraPos.x,pos.y - cameraPos.y,pos.z - cameraPos.z };
	return cameraPos;
}


void Player::ContactObj() {
	XMFLOAT3 pos = obj->GetPosition();

	if (!onGround) {
		// 下向き加速度
		const float fallAcc = -0.01f;
		const float fallVYMin = -0.5f;
		// 加速
		fallV.m128_f32[1] = max(fallV.m128_f32[1] + fallAcc, fallVYMin);
		// 移動
		pos.x += fallV.m128_f32[0];
		pos.y += fallV.m128_f32[1];
		pos.z += fallV.m128_f32[2];
	}
	// ワールド行列更新
	obj->UpdateWorldMatrix();
	obj->SetPosition(pos);
	obj->collider->Update();
	SphereCollider* sphereCollider = dynamic_cast<SphereCollider*>(obj->collider);
	assert(sphereCollider);

	// クエリーコールバッククラス
	class PlayerQueryCallback : public QueryCallback {
	public:
		PlayerQueryCallback(Sphere* sphere) : sphere(sphere) {};

		// 衝突時コールバック関数
		bool OnQueryHit(const QueryHit& info) {

			const XMVECTOR up = { 0,1,0,0 };

			XMVECTOR rejectDir = XMVector3Normalize(info.reject);
			float cos = XMVector3Dot(rejectDir, up).m128_f32[0];

			// 地面判定しきい値
			const float threshold = cosf(XMConvertToRadians(30.0f));

			if (-threshold < cos && cos < threshold) {

				sphere->center += info.reject;
				move += info.reject;
			}

			return true;
		}

		Sphere* sphere = nullptr;
		DirectX::XMVECTOR move = {};
	};
	
	PlayerQueryCallback callback(sphereCollider);

	// 球と地形の交差を全検索
	CollisionManager::GetInstance()->QuerySphere(*sphereCollider, &callback, COLLISIONSHAPE_MESH);
	// 交差による排斥分動かす
	pos.x += callback.move.m128_f32[0];
	pos.y += callback.move.m128_f32[1];
	pos.z += callback.move.m128_f32[2];
	// ワールド行列更新
	obj->UpdateWorldMatrix();
	obj->SetPosition(pos);
	obj->collider->Update();

	// 球の上端から球の下端までのレイキャスト
	Ray ray;
	ray.start = sphereCollider->center;
	ray.start.m128_f32[1] += sphereCollider->GetRadius();
	ray.dir = { 0,-1,0,0 };
	RaycastHit raycastHit;
	// 接地状態
	if (onGround) {
		// スムーズに坂を下る為の吸着距離
		const float adsDistance = 0.2f;
		// 接地を維持
		if (CollisionManager::GetInstance()->Raycast(ray, COLLISION_ATTR_LANDSHAPE, &raycastHit, sphereCollider->GetRadius() * 2.0f + adsDistance)) {
			onGround = true;
			pos.y -= (raycastHit.distance - sphereCollider->GetRadius() * 2.0f);
		}
		// 地面がないので落下
		else {
			onGround = false;
			fallV = {};
		}
	}
	// 落下状態
	else if (fallV.m128_f32[1] <= 0.0f) {
		if (CollisionManager::GetInstance()->Raycast(ray, COLLISION_ATTR_LANDSHAPE, &raycastHit, sphereCollider->GetRadius() * 2.0f)) {
			// 着地
			onGround = true;
			pos.y -= (raycastHit.distance - sphereCollider->GetRadius() * 2.0f);
		}
	}


}
