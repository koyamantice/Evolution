#pragma once
#include"Actor.h"
#include"FBXObject3d.h"
#include"Texture.h"
//#include""
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
		float isolateRadius = 2.0f;
		XMFLOAT2 vel;
		float weight;


	};
public:
	enum command {
		Wait = 0,
		Attack,
		Follow,
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
	void SetCommand(const int& command, XMFLOAT3 pos = { 0,0,0 });
	const int& GetCommand() { return command; }
	//
	void SetDeadFlag(const bool& DeadFlag) { this->DeadFlag = DeadFlag; }
	//��{����
	void Initialize(FBXModel* model, const std::string& tag = "None", ActorComponent* compornent = nullptr);	//����������
	void Update();		//�X�V����
	void Demo();		//�X�V����
	void Draw(DirectXCommon* dxCommon);	//�`�揈��

	void DemoDraw(DirectXCommon* dxCommon);	//�`�揈��

	void Finalize();	//�I������

	virtual void OnInit() {};
	virtual void OnUpda() {};
	virtual void OnDraw(DirectXCommon* dxCommon) {};
	virtual void OnFinal() {};
	virtual void DebugUpdate() {};

	virtual void OnCollision(const std::string& Tag) {};
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
	float angle = 0;
	bool DeadFlag = false;

	XMFLOAT3 oldPos = {};
	XMFLOAT3 AftaerPos{};

	Actor* enemy = nullptr;
	Actor* player = nullptr;

	XMFLOAT3 landing{};









};

