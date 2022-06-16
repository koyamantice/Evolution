#pragma once
#include "SceneManager.h"
#include "Actor.h"

/// <summary>
/// �^�C�g���V�[��
/// </summary>

class TitleScene : public BaseScene {
public:

	/// <summary>
	/// ������
	/// </summary>
	void Initialize(DirectXCommon* dxCommon) override;
	/// <summary>
	/// �I��
	/// </summary>
	void Finalize() override;

	/// <summary>
	/// ���t���[���X�V
	/// </summary>
	void Update(DirectXCommon* dxCommon) override;
	/// <summary>
	/// �`��
	/// </summary>
	void Draw(DirectXCommon* dxCommon) override;

private:
	//�����o�ϐ�
	enum {
		Player,
		Enemy,
		Chr_Max
	};
	std::unique_ptr<Actor> actor[Chr_Max]{};

	enum {
		back,
		button,
		select,
		ground,
		tutorial,
		SpriteMax
	};
	Sprite* sprite[SpriteMax]{};
	//�e�N���X�̂���
	DebugCamera* camera = { nullptr };
	enum Select {
		Start,
		Select,
		Exit,
	};
	XMFLOAT2 buttonPos[2]{
		{ 320.0f,600.0f },
		{ 440.0f,600.0f }
	};
	int SelectNumber = 0;
};

