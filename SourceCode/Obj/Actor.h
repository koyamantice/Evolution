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
private:
	//�I�u�W�F�N�g���ʔԍ�(��΂ɔ�邱�Ƃ͂Ȃ�����)
	int id;
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
	void SetIsActive(bool Active) {isActive = Active;};
	bool GetIsActive() { return isActive; }

	//isRemove�Z�b�^���Q�b�^
	void SetIsRemove(bool Remove) {isRemove = Remove;};
	bool GetIsRemove() { return isRemove; }
	
	bool Collide(Actor* actor);

	//virtual�ɂ��悤���Y�ݒ�
	void Initialize(Model* model,ActorComponent* compornent=nullptr);	//����������
	void Update();		//�X�V����
	void Draw();	//�`�揈��
	void Finalize();	//�I������

	virtual void OnInit() {};
	virtual void OnUpda() {};
	virtual void OnDraw() {};
	virtual void OnFinal() {};

protected:
	std::unique_ptr<Object3d> obj;
	//�R���|�[�l���g
	ActorComponent* compornent = nullptr;
};