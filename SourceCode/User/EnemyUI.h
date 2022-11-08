#pragma once
#include"ActorComponent.h"
#include <Player.h>
#include <SourceCode/Obj/2d/Sprite.h>

class EnemyUI : public  ActorComponent {
public:
	EnemyUI();
	~EnemyUI();
	void OnInitialize() override;	//����������(�p����)
	void OnUpdate() override;	//�X�V����(�p����)
	void OnFinalize() override;	//�I������(�p����)
	void OnDraw() override;		//�`�揈��(�p����)
	void OnAttached() override;	//�Q�[���I�u�W�F�N�g�ɃA�^�b�`���ꂽ�Ƃ��ɌĂ΂��֐�

private:
	std::unique_ptr<Sprite> Hp;
	std::unique_ptr<Sprite> Slash;
	Sprite* num[2][10]{};
	std::unique_ptr<Sprite> numBack[2];
	Sprite* numBullet[2][10]{};

	int Stock = 0;
	std::vector<int> stock;

	int OnLive = 0;
	std::vector<int> onLive;
	float frame = 0;
	float c = 0;
	float a = 0.01f;
	std::unique_ptr<Sprite> Chara;
	std::unique_ptr<Sprite> Bullet;
};