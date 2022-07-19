#pragma once
#include "SceneManager.h"
#include "Actor.h"

/// <summary>
/// �^�C�g���V�[��
/// </summary>

class DebugScene : public BaseScene {
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

	void Save();

	void EnemySave();

private:
	enum :int {
		Approch,
		Leave,
		Wait,
	};
	//�����o�ϐ�
	enum {
		MPlayer,
		MBullet,
		MEnemy,
		Chr_Max
	};
	Actor* actor[Chr_Max]{};
	XMFLOAT3 rot;
	//��������L����
	int Select = 0;

	//���ʃX�e
	int hp;
	float vel;
	//�G�X�e
	float waitTimer;
	int phase=0;

	bool save = false;
};

