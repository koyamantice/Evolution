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
	/// <summary>
	///	�J��������
	/// </summary>
	void CameraUpda();
	float angle = 0;
	const float PI=3.1452f;
	XMFLOAT2 distance={0,10};
	XMFLOAT2 dis = { 0,15 };
private:
	//�����o�ϐ�
	enum {
		MPlayer,
		MEnemy,
		Chr_Max
	};

	std::unique_ptr<Actor> actor[Chr_Max]{};
	std::unique_ptr<TouchableObject> skydome{};
	std::unique_ptr<TouchableObject> ground{};
	std::unique_ptr<PauseUI> pauseUi{};
	



};

