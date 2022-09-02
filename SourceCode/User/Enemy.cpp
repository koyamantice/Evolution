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
				phase_ = Enemy::Phase::Approch;
			}else if (word.find("LEAVE") == 0) {
				phase_ = Enemy::Phase::Leave;
			}else if (word.find("WAIT") == 0) {
				phase_ = Enemy::Phase::Wait;
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
	//move_object_->SetPosition(position);
	//move_object_->SetRotation(rot);
	Mash.reset(Mash_);

	Mash->PlayAnimation();

	LoadData();
	UpdateCommand();
	player = ActorManager::GetInstance()->SearchActor("Player");
}

void Enemy::OnUpda() {
	obj->SetRotation(XMFLOAT3{ 0,obj->GetRotation().y-1,0});
	Mash->Update();
	PhaseMove();
	LifeCommon();
	//Collide();
}

void Enemy::OnDraw(DirectXCommon* dxCommon) {
	Object3d::PreDraw();
	Mash->Draw(dxCommon->GetCmdList());

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
	switch (phase_) {
	case Enemy::Phase::Approch :
		ApprochUpdate();
		break;
	case Enemy::Phase::Leave:
		LeaveUpdate();
		break;
	case Enemy::Phase::Wait:
		WaitUpdate();
		break;
	default:
		break;
	}
}

void Enemy::ApprochUpdate() {
	XMFLOAT3 pos = obj->GetPosition();

}

void Enemy::LeaveUpdate() {
}

void Enemy::WaitUpdate() {
	//waitTimer++;
	//if (waitTimer == 300) {//150fps単位
	//	Mash->ResetAnimation();
	//}
}

void Enemy::LifeCommon() {
	if (hp<0) {
		isActive = false;
		//isRemove = true;
	}
}

