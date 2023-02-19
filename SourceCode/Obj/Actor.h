#pragma once
#include<vector>
#include <memory>
#include<string>
#include <DirectXMath.h>
#include <algorithm>

#include "DirectXCommon.h"
#include "FBXObject3d.h"
#include "Object3d.h"
#include "Object2d.h"


#include"ParticleEmitter.h"

class ActorComponent;

#define AFTIMAGENUM	8
#define DEGREE_MAX 360.0f
#define DEGREE_HALF 180.0f
#define DEGREE_QUARTER 90.0f

class Actor {
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
	////�e�t�F�C�Y
	//enum Phase {
	//	APPROCH = 0,
	//	UNGUARD,
	//	LEAVE,
	//	WAIT,
	//	DEAD,
	//	ATTACK,
	//};
	struct HitBound {
		//�������Ă��邩
		bool isHit;
		//�����������W
		XMFLOAT3 HitingPos;
	};
protected:
	//�I�u�W�F�N�g���ʔԍ�
	static int id;
	//�@�\����
	bool isActive = true;
	//�폜
	bool isRemove = false;
	//�Q�Ƃ���Ă��邩
	bool isRefer = false;
	//obj�͕K�v��
	bool isVisible = true;
	//�����Ă�������
	bool canMove = true;
	//���G��
	bool isUnrivaled = false;


public:
	virtual ~Actor() = default;
	//���W�̎擾
	void SetPosition(const DirectX::XMFLOAT3& pos) { obj->SetPosition(pos); }
	const DirectX::XMFLOAT3& GetPosition() { return obj->GetPosition(); }
	//�̂̍��W�̎擾
	const DirectX::XMFLOAT3& GetOldPosition() { return old_pos; }
	//�v���C���[�̎c��
	DirectX::XMFLOAT3 GetAFTIMAGE(const int& num) { return XMFLOAT3{ PlayerX[num + 1],RotY[num + 1],PlayerZ[num + 1] }; }
	//���W�̎擾
	virtual DirectX::XMFLOAT3 GetCameraPos(const float& angle, const float& str = 10) { return {0,0,0}; };
	//�p�x�̎擾
	void SetRotation(const DirectX::XMFLOAT3& rot) { obj->SetRotation(rot); }
	const DirectX::XMFLOAT3& GetRotation() { return obj->GetRotation(); }
	//�R�}���h�擾
	void SetCommand(const int& command) { this->command = command; };
	const int& GetCommand() { return command; }
	
	//hp�Z�b�^���Q�b�^
	void SetHp(const float& hp) { this->hp = hp; };
	const float& GetHp() { return hp; }
	//maxHp�Z�b�^���Q�b�^
	void SetMaxHp(const float& _maxhp) { this->max_hp = _maxhp; };
	const float& GetMaxHp() { return max_hp; }

	//Player�ŗL�̏���
	virtual const XMFLOAT3& GetLockPos() { return obj->GetPosition(); };
	void SetHitBound(const XMFLOAT3& pos) { hitBound.isHit = true; hitBound.HitingPos = pos; };
	const bool& GetHitBound() { return  hitBound.isHit; }
	//�����蔻��T�C�Y�擾
	const float& GetSize()const { return collide_size; }

	//�^�O�̃Z�b�g
	void SetTag(const std::string& Tag) { tag = Tag; }
	//�^�O����v���邩
	const std::string& GetTag() { return tag; }

	//isAlive�Z�b�^���Q�b�^
	void SetIsActive(const bool& Active) { isActive = Active; };
	const bool& GetIsActive() { return isActive; }

	//isRemove�Z�b�^���Q�b�^
	void SetIsRemove(const bool& Remove) { isRemove = Remove; };
	const bool& GetIsRemove() { return isRemove; }
		
	//isRefer�Z�b�^���Q�b�^
	void SetIsRefer(const bool& isRefer) { this->isRefer = isRefer; };
	const bool& GetIsRefer() { return isRefer; }

	//canMove�Z�b�^���Q�b�^
	void SetCanMove(const bool& canMove) { this->canMove = canMove; };
	const bool& GetCanMove() { return canMove; }

	//�J�����p�x�̃Z�b�^���Q�b�^
	void SetAngle(const float& angle) { this->angle = angle; }
	const float& GetAngle() { return angle; }
	//���C���h�C���g����
	void SetStock(const int& Stock) { this->stock = Stock; };
	const int& GetStock() { return stock; }
	//���C���h�u�[������
	void SetPause(const bool& pause) { this->pause = pause; }
	const bool& GetPause() { return pause; }
	//�Փ˔���
	void SetCollide(const bool& collide) { this->collide = collide; }
	const bool& GetCollide() { return collide; }

	//�e����
	void Initialize(Model* model, const std::string& tag = "None", ActorComponent* compornent = nullptr);	//����������
	void Update();		//�X�V����
	/// <summary>
	/// �����A�b�v�f�[�g
	/// </summary>
	/// <param name="Timer"></param>
	void IntroUpdate(const float& Timer);
	/// <summary>
	/// �I���A�b�v�f�[�g
	/// </summary>
	/// <param name="Timer"></param>
	void ResultUpdate(const float& Timer);

	void Demo();		//�X�V����

	void FirstDraw(DirectXCommon* dxCommon);	//��ɕ`�揈��
	void Draw(DirectXCommon* dxCommon);	//�`�揈��
	void LastDraw(DirectXCommon* dxCommon);	//��ŕ`�揈��

	void DemoDraw(DirectXCommon* dxCommon);	//�`�揈��
	void Finalize();	//�I������
	//���z�֐�
	virtual void OnInit() {};
	virtual void OnUpda() {};
	virtual void OnFirstDraw(DirectXCommon* dxCommon) {};
	virtual void OnDraw(DirectXCommon* dxCommon) {};
	virtual void OnLastDraw(DirectXCommon* dxCommon) {};
	virtual void OnFinal() {};
	virtual void DebugUpdate() {};
	virtual void IntroOnUpdate(const float& Timer) {};
	virtual void ResultOnUpdate(const float& Timer) {};
	virtual void OnCollision(const std::string& Tag) {};
protected:
	int command = 0;
	std::unique_ptr<Object3d> obj;
	float collide_size = 1.0f;
	// �c���f�[�^�̐�
	float PlayerX[AFTIMAGENUM], RotY[AFTIMAGENUM],PlayerZ[AFTIMAGENUM];
	HitBound hitBound;

	float max_hp = 0;
	float hp = 0;
	
	float angle = 0;
	bool pause = false;
	bool first = false;
	bool DeadFlag = false;
	
	bool collide = false;

	XMFLOAT3 old_pos = {};
	int stock = 0;
	//�R���|�[�l���g
	ActorComponent* compornent = nullptr;
};