#pragma once
#include"Actor.h"
#include"FBXObject3d.h"
#include"Object2d.h"
#include"ImageManager.h"

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
		Control,
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
	void SetsPlayActive(const bool& Play) { isPlayActive = Play; };
	const bool& GetIsPlayActive() { return isPlayActive; }
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
	void ResultUpdate(const int& Timer);
	void FirstDraw(DirectXCommon* dxCommon);
	void Draw(DirectXCommon* dxCommon);	//�`�揈��
	void LastDraw(DirectXCommon* dxCommon);
	void DemoDraw(DirectXCommon* dxCommon);	//�`�揈��

	void Finalize();	//�I������

	float DirRotation(const XMFLOAT3& target);
	
	void Navigation(const XMFLOAT3& target);

	void OnCollision(const std::string& Tag,const XMFLOAT3& pos);
	virtual void BulletCollision(const XMFLOAT3& pos, const int& Id) {};
protected:
	virtual void OnInit() {};
	virtual void OnUpda() {};
	virtual void OnFirstDraw(DirectXCommon* dxCommon) {};
	virtual void OnDraw(DirectXCommon* dxCommon) {};
	virtual void OnLastDraw(DirectXCommon* dxCommon) {};
	virtual void OnFinal() {};
	virtual void DebugUpdate() {};
	virtual void IntroOnUpdate(const int& Timer) {};
	virtual void ResultOnUpdate(const int& Timer) {};
	void SetAggregation();
	void LimitArea();
	void CommonUpda();

	void CommandUpda();

	void ControlUpda();
	void WaitUpda();
	void SlowUpda();
	void AttackUpda();
	void Follow2Enemy();

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
	float vanishHight = 0.1f;
	float vanishAlpha = 1.0f;
	int CoolTime = 0;
	bool wait = false;
	float vel = 0.8f;
	bool burning = false;
	void BurnOut();
	float scale = 0.0f;
	float effectRate = 0.0f;
	bool follow = false;
	XMFLOAT2 vel_follow{};



	bool clear_ease =true;
	float clear_frame = 0;
	XMFLOAT3 clear_s_pos{};
	XMFLOAT3 clear_e_pos{};
protected:
	//�o���b�g��p
	int ID;
	//�@�\����
	bool isActive = true;
	//�폜
	bool isRemove = false;
	//�R�}���h
	int command = 0;
	std::unique_ptr<FBXObject3d> fbxObj;
	std::unique_ptr<Object2d> Shadow = nullptr;
	std::unique_ptr<Object2d> Status = nullptr;
	std::unique_ptr<Object2d> Explo = nullptr;
	std::unique_ptr<Object2d> CharaDead = nullptr;

	enum DeathColor{
		Red=ImageManager::RedDead,
		Green,
	};
	int Color = 0;

	float angle = 0;
	bool DeadFlag = false;

	bool collide = false;
	XMFLOAT3 old_pos = {};
	bool navi = false;
	float navi_frame = 0;
	XMFLOAT3 before_pos{};
	XMFLOAT3 after_pos{};
	float follow_frame = 0.0f;
	XMFLOAT3 follow_pos{};

	Actor* enemy = nullptr;
	Actor* player = nullptr;
	Actor* ActionActor = nullptr;
	XMFLOAT3 landing{};

	bool isPlayActive = false;

	float margin;

	float hight = 100;
};

