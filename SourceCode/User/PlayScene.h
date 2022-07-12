#pragma once
#include "SceneManager.h"
#include "Actor.h"
#include"PauseUI.h"
#include <SourceCode/FrameWork/collision/TouchableObject.h>

/// <summary>
/// �^�C�g���V�[��
/// </summary>

class PlayScene : public BaseScene {
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
		MEnemy,
		Chr_Max
	};
	std::unique_ptr<Actor> actor[Chr_Max]{};
	std::unique_ptr<Object3d> skydome{};
	std::unique_ptr<TouchableObject> ground{};
	std::unique_ptr<PauseUI> pauseUi{};
	int SelectNumber = 0;
};

