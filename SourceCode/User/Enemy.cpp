#include"Enemy.h"
#include"ModelManager.h"
#include"ImageManager.h"
#include<fstream>
#include <SourceCode/FrameWork/collision/CollisionManager.h>
#include <SourceCode/FrameWork/collision/SphereCollider.h>
#include <SourceCode/FrameWork/collision/CollisionAttribute.h>
#include <SourceCode/FrameWork/ActorManager.h>

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
	Mash->Update();
	Mash->SetPosition({0,-3,0});
}

void Enemy::OnInit() {
	isVisible = false;/*
	obj->SetPosition(XMFLOAT3(0, 2, 0));
	obj->SetRotation(XMFLOAT3{0,-98,0});
	obj->SetScale(XMFLOAT3(2.0f,2.0f,2.0f));
	float radius = 1.0f;
	obj->SetCollider(new SphereCollider(XMVECTOR({ 0,radius,0,0 }), radius));
	obj->collider->SetAttribute(COLLISION_ATTR_ALLIES);*/
	FBXObject3d* Mash_= new FBXObject3d();
	Mash_->Initialize();
	Mash_->SetModel(ModelManager::GetIns()->GetFBXModel(ModelManager::Mash));
	Mash_->SetScale({0.01f,0.01f, 0.01f});
	Mash_->LoadAnimation();
	//move_object_->SetPosition(position);
	//move_object_->SetRotation(rot);
	Mash.reset(Mash_);
	Mash->PlayAnimation();

	LoadData();
	UpdateCommand();
	player = ActorManager::GetInstance()->SearchActor("Player");

	compornent = new EnemyUI();
	compornent->Initialize();

	EnemyAttack* Attack_ = new EnemyAttack();
	Attack_->Init();
	Attack.reset(Attack_);
	Texture* Shadow_ = Texture::Create(ImageManager::Shadow, { 0,0,0 },
		{ 0.5f,0.5f,0.5f }, { 1,1,1,1 });
	//Shadow_->SetIsBillboard(true);
	Shadow_->TextureCreate();
	Shadow_->SetRotation({ 90,0,0 });
	Shadow.reset(Shadow_);

}

void Enemy::OnUpda() {
	obj->SetRotation(XMFLOAT3{ 0,obj->GetRotation().y-1,0});
	obj->SetPosition(Mash->GetPosition());
	Mash->Update();
	Attack->Upda();
	PhaseMove();
	LifeCommon();
	//Collide();
	Shadow->Update();
	Shadow->SetPosition({ Mash->GetPosition().x,0.01f, Mash->GetPosition().z });
}

void Enemy::OnDraw(DirectXCommon* dxCommon) {
//	ImGui::Begin("test");
	//ImGui::SliderInt("bullet", &a, 0, 360);
	//ImGui::SliderFloat("Anglet", &angle, 0, 360);
//	ImGui::Unindent();
//	ImGui::End();

	Object3d::PreDraw();
	Mash->Draw(dxCommon->GetCmdList());
	Texture::PreDraw();
	Shadow->Draw();
	Attack->Draw();
}

void Enemy::OnFinal() {
}

void Enemy::Collide() {
	XMFLOAT3 pos = obj->GetPosition();
	SphereCollider* sphereCollider = dynamic_cast<SphereCollider*>(obj->collider);
	assert(sphereCollider);
	obj->collider->Update();

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
}

void Enemy::PhaseMove() {
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

void Enemy::ApprochUpda() {
	XMFLOAT3 pos = Mash->GetPosition();

	angle += 2.5f;
	pos.x =sinf(angle * (XM_PI / 180)) *15.0f;
	pos.z =cosf(angle * (XM_PI / 180)) *15.0f;
	if (pos.y >= 0) {
		pos.y += speed;
		speed -= accel;
	} else {
		pos.y = 0;
		speed = accel * 30.0f;
	}
	waitTimer++;
	if (waitTimer == 300) {//150fps単位
		pos.y = 0;
		speed = accel * 30.0f;
		Mash->ResetAnimation();
		command = Actor::Phase::ATTACK;
		Mash->PlayAnimation();
		waitTimer = 0;
	}
	Mash->SetPosition(pos);


}

void Enemy::LeaveUpda() {
}

void Enemy::WaitUpda() {
	waitTimer++;
	if (waitTimer == 300) {//150fps単位
		Mash->ResetAnimation();
		command = Actor::Phase::APPROCH;
		waitTimer = 0;
	}
}

void Enemy::AttackUpda() {
	XMFLOAT3 pos = Mash->GetPosition();
	waitTimer++;
	if (waitTimer==450) {
		pos.y = 0;
		Mash->ResetAnimation();
		Mash->PlayAnimation();
		command = Actor::Phase::WAIT;
		waitTimer = 0;
		Mash->SetPosition(pos);
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
	Mash->SetRotation({0,-180,0});
	Mash->SetPosition(pos);
}

void Enemy::LifeCommon() {
	if (hp<0) {
		isActive = false;
		//isRemove = true;
	}
}