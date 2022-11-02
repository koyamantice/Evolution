#pragma once
#include"Actor.h"
#include"FBXObject3d.h"
#include"Texture.h"
using namespace DirectX;
class Bullet {
protected: // �G�C���A�X
// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

	struct FlockSystem {
		XMFLOAT3 average{};
		//����
		float isolateRadius = 2.0f;
	
		float ctrDirX;
		float ctrDirY;
		float contX;
		float contY;
		XMFLOAT2 vel;
		float weight;
		float aveAngle;
		float disvel=(float)(rand()%5);
		float dx = disvel* cosf(aveAngle * (XM_PI / 180));
		float dy = disvel* sinf(aveAngle * (XM_PI / 180));

	};
public:
	enum command {
		Wait = 0,
		Attack,
		Slow,
		Dead,
	};
public:
	Bullet();
	virtual ~Bullet() = default;

	//���W�̎擾
	void SetPosition(const DirectX::XMFLOAT3& pos) { fbxObj->SetPosition(pos); }
	const DirectX::XMFLOAT3& GetPosition() { return fbxObj->GetPosition(); }
	//�p�x�̎擾
	void SetRotation(const DirectX::XMFLOAT3& rot) { fbxObj->SetRotation(rot); }
	const DirectX::XMFLOAT3& GetRotation() { return fbxObj->GetRotation(); }

	//ID�擾
	const int& GetID()const { return ID; }

	//isAlive�Z�b�^���Q�b�^
	void SetIsActive(const bool& Active) { isActive = Active; };
	const bool& GetIsActive() { return isActive; }

	//isRemove�Z�b�^���Q�b�^
	void SetIsRemove(const bool& Remove) { isRemove = Remove; };
	const bool& GetIsRemove() { return isRemove; }

	//�J�����p�x�̃Z�b�^���Q�b�^
	void SetAngle(const float& angle) { this->angle = angle; }
	const float& GetAngle() { return angle; }
	//
	void SetContX(const float& contX) { this->flocking.contX = contX; }
	const float& GetContX() { return flocking.contX; }
	//
	void SetContY(const float& contY) { this->flocking.contY = contY; }
	const float& GetContY() { return flocking.contY; }

	void SetVel(const XMFLOAT2& vel) { this->flocking.vel= vel; }
	const XMFLOAT2& GetVel() { return this->flocking.vel; }



	//
	void SetCommand(const int& command, XMFLOAT3 pos = { 0,0,0 });
	const int& GetCommand() { return command; }
	//
	void SetDeadFlag(const bool& DeadFlag) { this->DeadFlag = DeadFlag; }
	//��{����
	void Initialize(FBXModel* model, const std::string& tag = "None", ActorComponent* compornent = nullptr);	//����������
	void Update();		//�X�V����
	void Demo();		//�X�V����
	void IntroUpdate(const int& Timer);
	void Draw(DirectXCommon* dxCommon);	//�`�揈��

	void DemoDraw(DirectXCommon* dxCommon);	//�`�揈��

	void Finalize();	//�I������

	virtual void OnCollision(const std::string& Tag) {};

protected:
	virtual void OnInit() {};
	virtual void OnUpda() {};
	virtual void OnDraw(DirectXCommon* dxCommon) {};
	virtual void OnFinal() {};
	virtual void DebugUpdate() {};
	virtual void IntroOnUpdate(const int& Timer) {};


	void BoidAverage();
	void Move();

	void WaitUpda();
	void SlowUpda();
	void AttackUpda();
	void Follow2Enemy();
	void Follow2Player();
	void WaitBullet();

	void KnockBack();
	bool knockBacking = false;
	void DamageInit();
	XMFLOAT3 rebound{};
	XMFLOAT3 distance{};
	XMFLOAT3 exploPos{};
	float damageframe = 0.0f;
	float fall = 0.4f;
	bool isLeft = false;
	bool throwReady = false;
	float frame = 0.0f;


	void DeadEnd();
	float deadframe = 0;


	int CoolTime = 0;

	float vel = 0.8f;
	std::unique_ptr<Texture> Status{};
	std::unique_ptr<Texture> Explo = nullptr;
	bool burning = false;
	void BurnOut();
	float scale = 0.0f;
	float effectRate = 0.0f;
	bool follow = false;
	XMFLOAT2 vel_follow{};
protected:
	//�o���b�g��p
	int ID;
	//�@�\����
	bool isActive = true;
	//�폜
	bool isRemove = false;
	//
	int command = 0;
	std::unique_ptr<FBXObject3d> fbxObj;
	std::unique_ptr<Texture> Shadow = nullptr;

	float angle = 0;
	bool DeadFlag = false;

	XMFLOAT3 oldPos = {};
	XMFLOAT3 AftaerPos{};

	Actor* enemy = nullptr;
	Actor* player = nullptr;

	XMFLOAT3 landing{};

	FlockSystem flocking;
	float dx=0;
	float dy=0;

	float hight = 100;
};

