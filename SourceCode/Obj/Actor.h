#pragma once
#include<vector>
#include <memory>
#include<string>
#include"Object3d.h"
#include<DirectXMath.h>
#include"ParticleManager.h"
#include "DirectXCommon.h"
#include "FBXObject3d.h"

class ActorComponent;

class Actor 
{
protected: // �G�C���A�X
// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
protected:
	//�t�^����Ă���^�O�ւ̃|�C���^
	std::string tag = "None";
public:

	enum Phase {
		APPROCH = 0,
		LEAVE,
		WAIT,
		ATTACK,
	};
protected:
	//�I�u�W�F�N�g���ʔԍ�
	static int id;
	//�@�\����
	bool isActive = true;
	//�폜
	bool isRemove = false;
	//obj�͕K�v��
	bool isVisible = true;
public:
	virtual ~Actor() = default;
	//���W�̎擾
	void SetPosition(const DirectX::XMFLOAT3& pos) { obj->SetPosition(pos); }
	const DirectX::XMFLOAT3& GetPosition() { return obj->GetPosition(); }
	//���W�̎擾
	void SetCameraPos(const DirectX::XMFLOAT3& pos) { this->cameraPos=cameraPos; }
	const DirectX::XMFLOAT3& GetCameraPos() { return cameraPos; }

	//�p�x�̎擾
	void SetRotation(const DirectX::XMFLOAT3& rot) { obj->SetRotation(rot); }
	const DirectX::XMFLOAT3& GetRotation() { return obj->GetRotation(); }
	//
	void SetCommand(const int& command);
	const int& GetCommand() { return command; }
	//isAlive�Z�b�^���Q�b�^
	void SetHp(const float& hp) { this->hp = hp; };
	const float& GetHp() { return hp; }

	//Player�ŗL�̏���
	virtual const XMFLOAT3& GetLockPos() { return {0,0,0}; }
	
	//OBJ�T�C�Y�擾�iX���̂������Ă���̂Ŋg��͂܂Ƃ߂Ă���̐����j
	const float& GetSize()const { return obj->GetSize(); }

	//�^�O�̃Z�b�g
	void SetTag(const std::string& Tag){tag = Tag;};
	//�^�O����v���邩
	const std::string& GetTag() { return tag; }

	//isAlive�Z�b�^���Q�b�^
	void SetIsActive(const bool& Active) {isActive = Active;};
	const bool& GetIsActive() { return isActive; }

	//isRemove�Z�b�^���Q�b�^
	void SetIsRemove(const bool& Remove) {isRemove = Remove;};
	const bool& GetIsRemove() { return isRemove; }

	//canMove�Z�b�^���Q�b�^
	void SetCanMove(const bool& canMove) { this->canMove = canMove; };
	const bool& GetCanMove() { return canMove; }

	//�J�����p�x�̃Z�b�^���Q�b�^
	void SetAngle(const float& angle) { this->angle = angle; }
	const float& GetAngle() { return angle; }
	//
	void SetStock(const int& Stock) {this->stock = Stock;};
	const int& GetStock() { return stock; }
	//
	void SetPause(const bool& pause) { this->pause = pause; }
	const bool& GetPause() { return pause; }
	//virtual�ɂ��悤���Y�ݒ�//�����ς�
	void Initialize(Model* model,const std::string& tag="None", ActorComponent* compornent = nullptr);	//����������
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

	virtual void OnCollision(const std::string& Tag){};
protected:
	int command = 0;
	std::unique_ptr<Object3d> obj;
	XMFLOAT3 cameraPos{};
	float hp = 0;
	float angle = 0;
	bool canMove = true;
	bool pause = false;
	bool first = false;
	bool DeadFlag = false;
	XMFLOAT3 oldPos = {};
	int stock = 0;
	//�R���|�[�l���g
	ActorComponent* compornent = nullptr;
};