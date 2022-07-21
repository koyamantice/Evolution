#include"Player.h"
#include"ModelManager.h"
#include"ImageManager.h"
#include"Collision.h"
#include <SourceCode/FrameWork/collision/CollisionManager.h>
#include <SourceCode/FrameWork/collision/SphereCollider.h>
#include "CollisionAttribute.h"
#include<fstream>
#include <SourceCode/FrameWork/ActorManager.h>
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
			break;
		}
	}
}

void Player::DebugUpdate() {
}

void Player::OnInit() {
	obj->SetRotation(XMFLOAT3{ 0,0,0 });
	
	LoadData();
	UpdateCommand();

	vel /= 5.0f;


	float radius = 1.0f;
	obj->SetCollider(new SphereCollider(XMVECTOR({ 0,radius,0,0 }), radius));
	obj->collider->SetAttribute(COLLISION_ATTR_ALLIES);

	Texture* Lock_ = Texture::Create(ImageManager::Lock, obj->GetPosition(), { 0.5f,0.5f,0.5f }, { 1,1,1,1 });
	Lock_->TextureCreate();
	Lock_->SetRotation({ 90,0,0 });
	LockOn.reset(Lock_);
}

void Player::OnUpda() {
	Move();
	Shot();

	ContactObj();
	LockOn->Update();
}

void Player::OnDraw() {
	Texture::PreDraw();
	LockOn->Draw();
}

void Player::OnFinal() {
}

void Player::Move() {
	XMFLOAT3 pos = obj->GetPosition();
	XMFLOAT3 rot = obj->GetRotation();

	float StickX = input->GetPosX();
	float StickY = input->GetPosY();
	const float PI = 3.14159f;

	if (input->PushKey(DIK_W)||input->LeftTiltStick(input->Up)) {
		XMFLOAT3 vecvel = MoveVECTOR(XMVECTOR{ 0,0,vel,0 }, angle);
		pos.x -= vecvel.x;
		pos.y -= vecvel.y;
		pos.z -= vecvel.z;
		rot.y = angle;
	}
	if (input->PushKey(DIK_S) || input->LeftTiltStick(input->Down)) {
		XMFLOAT3 vecvel = MoveVECTOR(XMVECTOR{ 0,0,-vel,0 }, angle);
		pos.x -= vecvel.x;
		pos.y -= vecvel.y;
		pos.z -= vecvel.z;
		rot.y = angle-180;
	}
	if (input->PushKey(DIK_D) || input->LeftTiltStick(input->Right)) {
		XMFLOAT3 vecvel = MoveVECTOR(XMVECTOR{ vel,0,0,0 }, angle);
		pos.x -= vecvel.x;
		pos.y -= vecvel.y;
		pos.z -= vecvel.z;
		rot.y = angle+90;
	}
	if (input->PushKey(DIK_A) || input->LeftTiltStick(input->Left)) {
		XMFLOAT3 vecvel = MoveVECTOR(XMVECTOR{-vel,0,0,0 }, angle);
		pos.x -= vecvel.x;
		pos.y -= vecvel.y;
		pos.z -= vecvel.z;
		rot.y = angle-90;
	}


	obj->SetPosition(pos);
	obj->SetRotation(rot);
}

void Player::RightMove() {
}

void Player::LeftMove() {
}


XMFLOAT3 Player::MoveVECTOR(XMVECTOR v, float angle) {

	rot2 = XMMatrixRotationY(XMConvertToRadians(angle));
	v = XMVector3TransformNormal(v, rot2);
	XMFLOAT3 pos = { v.m128_f32[0],v.m128_f32[1] ,v.m128_f32[2] };

	return pos;
}



void Player::Shot() {
	if (input->ReleaseKey(DIK_SPACE)|| input->ReleaseButton(input->Button_A)) {
		ActorManager::GetInstance()->AttachActor("Bullet");
		LockOn->SetPosition(obj->GetPosition());
	}
	if (input->PushKey(DIK_SPACE) || input->PushButton(input->Button_A)) {
		rockpos=LockOn->GetPosition();
		XMFLOAT3 vecvel = MoveVECTOR(XMVECTOR{ 0,0,1,0 }, obj->GetRotation().y);
		rockpos.x -= vecvel.x;
		rockpos.y = 0.1f;
		rockpos.z -= vecvel.z;
		LockOn->SetPosition(rockpos);
	} else {
		LockOn->SetPosition(obj->GetPosition());
	}

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
