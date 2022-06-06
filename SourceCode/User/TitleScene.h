#pragma once
#include "SceneManager.h"
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
	Sprite* sprite[4]{};
	enum {
		back,
		button,
		select,
		ground,
		tutorial,
	};
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

