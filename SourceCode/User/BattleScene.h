#pragma once
#include "SceneManager.h"

#include "Actor.h"
#include "ActorManager.h"
#include "PauseUI.h"

#include <Touch.h>
#include <PostEffect.h>


class BattleScene : public BaseScene {
public:
	virtual ~BattleScene() = default;

protected:
	/// <summary>
	/// �o�g���X�e�[�W��p�̏���������
	/// </summary>
	void BattleInit();

	/// <summary>
	/// �t�B�[���h�̍X�V����
	/// </summary>
	void FieldUpdate();

	/// <summary>
	/// ������csv�ǂݍ���
	/// </summary>
	/// <param name="color">�J���[</param>
	void TorchSetup(int color=0);

	/// <summary>
	///	�J��������
	/// </summary>
	void CameraUpda();

	/// <summary>
	/// �J��������w���Ɏ����Ă��܂�
	/// </summary>
	void ResetCamera();

	/// <summary>
	/// �����\���p�̍X�V
	/// </summary>
	virtual void DescriptionUpdate() {};

	/// <summary>
	///	���������̍X�V����
	/// </summary>
	/// <returns></returns>
	virtual bool IntroUpdate() { return false; };

	/// <summary>
	/// �|�[�YUI�̍X�V����
	/// </summary>
	/// <returns></returns>
	bool PauseUpdate();

	/// <summary>
	/// �Q�[���I�[�o�[�̍X�V����
	/// </summary>
	virtual void GameOverUpdate() {};

	/// <summary>
	/// �N���A�X�V����
	/// </summary>
	virtual bool ClearUpdate() { return false; };
protected:
	Input* input = Input::GetInstance();

	Actor* player_shadow = nullptr;
	Actor* enemy_shadow = nullptr;
	Actor* goal_shadow = nullptr;

	std::unique_ptr<PauseUI> pauseUi{};
	std::unique_ptr<Sprite>	screens[2]{};

	std::unique_ptr<Sprite> filter_first{};
	float filter_alpha = 0.0f;

	std::unique_ptr<Object3d> skydome{};
	std::unique_ptr<Object3d> ground{};

	std::stringstream torch_pop;
	std::list<std::unique_ptr<Touch>> torchs;


	//kill�N���b�v
	int finishTime = 0;


	bool battle_intro = true;

	float feedin_frame = 0;
	const float feedin_frame_max = 20;

	bool battle_result = false;
	bool gameover = false;

	bool stage_clear = false;
	float clear_frame = 0.0f;

	//�J�����֌W
	bool reset_camera = false;
	float angle_frame = 0.0f;
	const float angle_frame_max = 60.0f;

	bool camera_explanation = false;

	const float camera_hight = 18.0f;
	const float camera_radius = 15.0f;

	float camera_angle = 0.0f;
	const float camera_target = 7.0f;
	const float camera_vel = 3.0f;
	float s_camera_angle = 0.0f;
	float e_camera_angle = 0.0f;

	XMFLOAT3 camera_distance = {};
	XMFLOAT3 s_camera_distance = {};
	XMFLOAT3 e_camera_distance = {};
};