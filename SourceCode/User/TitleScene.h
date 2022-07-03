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
		MPlayer,
		Enemy,
		Chr_Max
	};
	std::unique_ptr<Actor> actor[Chr_Max]{};
	std::unique_ptr<Object3d> skydome{};

	//�e�N���X�̂���
	DebugCamera* camera = { nullptr };
	int SelectNumber = 0;
};

