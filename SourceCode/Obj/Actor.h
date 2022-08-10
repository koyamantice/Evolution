#pragma once
#include<vector>
#include <memory>
#include<string>
#include"Object3d.h"
#include<DirectXMath.h>

class ActorComponent;

class Actor 
{
private:
	static int ID;
protected: // �G�C���A�X
// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
protected:
	//���O
	std::string name;
	//�t�^����Ă���^�O�ւ̃|�C���^
	std::string tag = "None";
protected:
	//�I�u�W�F�N�g���ʔԍ�(��΂ɔ�邱�Ƃ͂Ȃ�����)
	static int id;
	//�@�\����
	bool isActive = true;
	//�폜
	bool isRemove = false;
public:
	virtual ~Actor() = default;
	//���W�̎擾
	void SetPosition(const DirectX::XMFLOAT3& pos) { obj->SetPosition(pos); }
	const DirectX::XMFLOAT3& GetPosition() { return obj->GetPosition(); }
	//�p�x�̎擾
	void SetRotation(const DirectX::XMFLOAT3& rot) { obj->SetRotation(rot); }
	const DirectX::XMFLOAT3& GetRotation() { return obj->GetRotation(); }

	//isAlive�Z�b�^���Q�b�^
	void SetHp(const float& hp) { this->hp = hp; };
	const float& GetHp() { return hp; }

	//
	virtual const XMFLOAT3& GetLockPos() { return obj->GetPosition(); }
	//ID�擾
	const int& GetID()const { return id; }

	//���O�Z�b�^���Q�b�^
	void SetName(const std::string& Name) { name = Name; }
	const std::string& GetName() { return name; }

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

	//virtual�ɂ��悤���Y�ݒ�//�����ς�
	void Initialize(Model* model,const std::string& tag="None", ActorComponent* compornent = nullptr);	//����������
	void Update();		//�X�V����
	void Demo();		//�X�V����
	void Draw();	//�`�揈��
	void DemoDraw();	//�`�揈��
	void Finalize();	//�I������

	virtual void OnInit() {};
	virtual void OnUpda() {};
	virtual void OnDraw() {};
	virtual void OnFinal() {};
	virtual void DebugUpdate() {};

	virtual void OnCollision(){};
protected:
	std::unique_ptr<Object3d> obj;
	float hp = 0;
	float angle = 0;
	bool canMove = true;

	
	int stock = 30;
	//�R���|�[�l���g
	ActorComponent* compornent = nullptr;
};