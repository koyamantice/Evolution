#pragma once
#include"ActorComponent.h"
#include <Input.h>

class Player: public ActorComponent {
public :
	void OnInitialize();	//����������(�p����)
	void OnUpdate();	//�X�V����(�p����)
	void OnFinalize();	//�I������(�p����)
	void OnDraw();	//�`�揈��(�p����)
	void OnAttached();	//�Q�[���I�u�W�F�N�g�ɃA�^�b�`���ꂽ�Ƃ��ɌĂ΂��֐�
	Input* input = Input::GetInstance();
	


};