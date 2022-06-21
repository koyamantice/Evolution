#pragma once
#include<memory>
#include<string>

class Actor;

class ActorComponent {
protected:
	//�A�^�b�`�����A�N�^�[�i���L���͂����Ȃ��j
	std::weak_ptr<Actor> attachActor;

	//���z�֐�
	virtual void OnInitialize() {}	//����������(�p����)
	virtual void OnUpdate() {}	//�X�V����(�p����)
	virtual void OnFinalize() {}	//�I������(�p����)
	virtual void OnDraw() {}		//�`�揈��(�p����)
	virtual void OnAttached() {}	//�Q�[���I�u�W�F�N�g�ɃA�^�b�`���ꂽ�Ƃ��ɌĂ΂��֐�

	//�@�\���Ă��邩
	bool isActive = true;
	//�폜���邩
	bool isRemove = false;


public:
	virtual ~ActorComponent() {}

	void Initialize();	//����������(�A�N�^�[��)
	void Update();		//�X�V����(�A�N�^�[��)
	void Draw();	//�`�揈��(�A�N�^�[��)
	void Finalize();	//�I������(�A�N�^�[��)
		//�Q�[���I�u�W�F�N�g�ɃA�^�b�`
	void SetAttachActor(std::weak_ptr<Actor> Actor);

		//isAlive�Z�b�^���Q�b�^
	void SetIsActive(bool Active) { isActive = Active; }
	bool GetIsActive() { return isActive; }

	//isRemove�Z�b�^���Q�b�^
	void SetIsRemove(bool Remove) { isRemove = Remove; }
	bool GetIsRemove() { return isRemove; }

};