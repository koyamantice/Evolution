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
		//��͂��₷������
		std::istringstream line_stream(line);

		std::string word;
		//���p�X�y�[�X��؂�ōs�̐擪��������擾
		getline(line_stream, word, ',');
		//"//"����n�܂�s�̓R�����g
		if (word.find("//") == 0) {
			//��΂�
			continue;
		}
		//�e�R�}���h
		if (word.find("HP") == 0) {
			getline(line_stream, word, ',');
			hp = (float)std::atof(word.c_str());
		} else if (word.find("VEL") == 0) {
			getline(line_stream, word, ',');
			vel = (float)std::atof(word.c_str());
			if (vel>5) {
				vel = 5;
			}
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

void Player::OnInit() {
	obj->SetRotation(XMFLOAT3{ 0,0,0 });
	obj->SetPosition({0,0,15});
	LoadData();
	UpdateCommand();

	vel /= 5.0f;

	//obj->SetScale({ 0.1f, 0.1f, 0.1f });
	//obj->SetColor({ 0.0f, 1.0f, 0.0f,1.0f });

	float radius = 1.0f;
	obj->SetCollider(new SphereCollider(XMVECTOR({ 0,radius,0,0 }), radius));
	obj->collider->SetAttribute(COLLISION_ATTR_ALLIES);

	Aim* LockOn_=new Aim();
	LockOn.reset(LockOn_);
	LockOn->Init();

	compornent = new PlayerUI();
	compornent->Initialize();

}

void Player::OnUpda() {
	if (!first) {
		LockOn->FirstSet();


		first = true;
	}
	if (canMove) {
		Move();
	}
	Shot();
	ContactObj();
	LockOn->Upda(angle);
}

void Player::OnDraw(DirectXCommon* dxCommon) {
	//
	LockOn->Draw();

	//int a=ActorManager::GetInstance()->SearchNum("Bullet");
	//	ImGui::Begin("test");
	//	ImGui::SliderInt("bullet", &a, 0, 360);
	////	ImGui::SliderFloat("Anglet", &angle, 0, 360);
	//	ImGui::Unindent();
	//	ImGui::End();

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
	if (input->TiltPushStick(Input::L_UP, 0.0f)||
		input->TiltPushStick(Input::L_DOWN, 0.0f)||
		input->TiltPushStick(Input::L_RIGHT, 0.0f)||
		input->TiltPushStick(Input::L_LEFT, 0.0f)) {
		if (input->TiltPushStick(Input::L_UP, 0.0f)) {
			XMFLOAT3 vecvel = MoveVECTOR(XMVECTOR{ 0,0,vel,0 }, angle);
			pos.x -= vecvel.x * (StickY / STICK_MAX);
			pos.z -= vecvel.z * (StickY / STICK_MAX);
			rot.y = angle;
		}
		if (input->TiltPushStick(Input::L_DOWN, 0.0f)) {
			XMFLOAT3 vecvel = MoveVECTOR(XMVECTOR{ 0,0,-vel,0 }, angle);
			pos.x += vecvel.x * (StickY / STICK_MAX);
			pos.z += vecvel.z * (StickY / STICK_MAX);
			rot.y = angle - 180;
		}
		if (input->TiltPushStick(Input::L_RIGHT, 0.0f)) {
			XMFLOAT3 vecvel = MoveVECTOR(XMVECTOR{ vel,0,0,0 }, angle);
			pos.x -= vecvel.x * (StickX / STICK_MAX);
			pos.z -= vecvel.z * (StickX / STICK_MAX);
			rot.y = angle + 90;
		}
		if (input->TiltPushStick(Input::L_LEFT, 0.0f)) {
			XMFLOAT3 vecvel = MoveVECTOR(XMVECTOR{ -vel,0,0,0 }, angle);
			pos.x += vecvel.x * (StickX / STICK_MAX);
			pos.z += vecvel.z * (StickX / STICK_MAX);
			rot.y = angle - 90;
		}
		const float rnd_vel = 0.1f;
		XMFLOAT3 vel{};
		vel.x = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.y = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.z = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		ParticleManager::GetInstance()->Add(15, oldPos, vel, XMFLOAT3(), 1.2f, 0.6f);


		obj->SetPosition(pos);
		obj->SetRotation(rot);
	}

}



void Player::OnCollision(const std::string& Tag) {
	if (Tag == "Enemy") {
		//hp--;
		int a = 0;
		a++;
	}
	if (Tag == "Bullet") {
		
	}
	if (Tag == "Crystal") {
		int a = 0;
		a++;
	}

}


XMFLOAT3 Player::MoveVECTOR(XMVECTOR v, float angle) {
	rot2 = XMMatrixRotationY(XMConvertToRadians(angle));
	v = XMVector3TransformNormal(v, rot2);
	XMFLOAT3 pos = { v.m128_f32[0],v.m128_f32[1] ,v.m128_f32[2] };
	return pos;
}



void Player::Shot() {
	//LockOn->Move();
}

void Player::ContactObj() {
	XMFLOAT3 pos = obj->GetPosition();

	if (!onGround) {
		// �����������x
		const float fallAcc = -0.01f;
		const float fallVYMin = -0.5f;
		// ����
		fallV.m128_f32[1] = max(fallV.m128_f32[1] + fallAcc, fallVYMin);
		// �ړ�
		pos.x += fallV.m128_f32[0];
		pos.y += fallV.m128_f32[1];
		pos.z += fallV.m128_f32[2];
	}
	// ���[���h�s��X�V
	obj->UpdateWorldMatrix();
	obj->SetPosition(pos);
	obj->collider->Update();
	SphereCollider* sphereCollider = dynamic_cast<SphereCollider*>(obj->collider);
	assert(sphereCollider);

	// �N�G���[�R�[���o�b�N�N���X
	class PlayerQueryCallback : public QueryCallback {
	public:
		PlayerQueryCallback(Sphere* sphere) : sphere(sphere) {};

		// �Փˎ��R�[���o�b�N�֐�
		bool OnQueryHit(const QueryHit& info) {

			const XMVECTOR up = { 0,1,0,0 };

			XMVECTOR rejectDir = XMVector3Normalize(info.reject);
			float cos = XMVector3Dot(rejectDir, up).m128_f32[0];

			// �n�ʔ��肵�����l
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

	// ���ƒn�`�̌�����S����
	CollisionManager::GetInstance()->QuerySphere(*sphereCollider, &callback, COLLISIONSHAPE_MESH);
	// �����ɂ��r�˕�������
	pos.x += callback.move.m128_f32[0];
	pos.y += callback.move.m128_f32[1];
	pos.z += callback.move.m128_f32[2];
	// ���[���h�s��X�V
	obj->UpdateWorldMatrix();
	obj->SetPosition(pos);
	obj->collider->Update();

	// ���̏�[���狅�̉��[�܂ł̃��C�L���X�g
	Ray ray;
	ray.start = sphereCollider->center;
	ray.start.m128_f32[1] += sphereCollider->GetRadius();
	ray.dir = { 0,-1,0,0 };
	RaycastHit raycastHit;
	// �ڒn���
	if (onGround) {
		// �X���[�Y�ɍ������ׂ̋z������
		const float adsDistance = 0.2f;
		// �ڒn���ێ�
		if (CollisionManager::GetInstance()->Raycast(ray, COLLISION_ATTR_LANDSHAPE, &raycastHit, sphereCollider->GetRadius() * 2.0f + adsDistance)) {
			onGround = true;
			pos.y -= (raycastHit.distance - sphereCollider->GetRadius() * 2.0f);
		}
		// �n�ʂ��Ȃ��̂ŗ���
		else {
			onGround = false;
			fallV = {};
		}
	}
	// �������
	else if (fallV.m128_f32[1] <= 0.0f) {
		if (CollisionManager::GetInstance()->Raycast(ray, COLLISION_ATTR_LANDSHAPE, &raycastHit, sphereCollider->GetRadius() * 2.0f)) {
			// ���n
			onGround = true;
			pos.y -= (raycastHit.distance - sphereCollider->GetRadius() * 2.0f);
		}
	}


}
