#pragma once
#include "SceneManager.h"

#include "Actor.h"
#include "ActorManager.h"
#include "PauseUI.h"

#include <Touch.h>
#include <PostEffect.h>
#include <Hud.h>


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
	virtual bool IntroUpdate() { return false; }

	/// <summary>
	/// �|�[�YUI�̍X�V����
	/// </summary>
	/// <returns></returns>
	bool PauseUpdate();

	/// <summary>
	/// �Q�[���I�[�o�[�̍X�V����
	/// </summary>
	virtual bool GameOverUpdate() { return false; }

	/// <summary>
	/// �N���A�X�V����
	/// </summary>
	virtual bool ClearUpdate() { return false; }
	/// <summary>
	/// 
	/// </summary>
	/// <param name="base"></param>
	/// <returns></returns>
	float RandHeight(const float& base);
	/// <summary>
	/// �o�g���X�e�[�W��p�̕`�揈��(�O)
	/// </summary>
	void BattleBackDraw();
	/// <summary>
	/// �o�g���X�e�[�W��p�̕`�揈��(���)
	/// </summary>
	void BattleFrontDraw(blendType _blendType = alphaBle, Sprite* _introwo = nullptr);

	virtual void DrawLocal() {};

protected:
	Input* input = Input::GetInstance();

	Actor* player_shadow = nullptr;
	Actor* enemy_shadow = nullptr;
	Actor* goal_shadow = nullptr;
	//
	const int kGnormNum = 10;
	//�|�[�YUI
	std::unique_ptr<PauseUI> pauseUi{};
	//�N���A����
	std::unique_ptr<Sprite> clear_comment_{};
	//�Q�[���I�[�o�[����
	std::unique_ptr<Sprite> over_comment_{};

	//���������̃X�N���[��
	std::unique_ptr<Sprite>	screens[2]{};
	//�X�L�b�vUI
	std::unique_ptr<Sprite>	skip_ui_{};

	//�Èł���t�F�[�h���܂�
	std::unique_ptr<Sprite> filter_first{};
	float filter_alpha = 0.0f;
	float feedin_frame = 0;
	const float feedin_frame_max = 20;

	//�X�e�[�W�֘A
	std::unique_ptr<Object3d> skydome{};
	std::unique_ptr<Object3d> ground{};
	std::stringstream torch_pop;
	std::list<std::unique_ptr<Touch>> torchs;

	//�R���g���[���[�̐���
	std::unique_ptr<Hud> hud=nullptr;

	//kill�N���b�v
	int finish_time = 0;
	const int finish_time_Max = 220;
	
	bool battle_intro = true;
	bool battle_result = false;

	//�N���A�E�Q�[���I�[�o�[
	bool stage_clear = false;
	float clear_frame = 0.0f;

	bool gameover = false;

	//�J�����֌W
	bool reset_camera = false;
	float angle_frame = 0.0f;
	const float angle_frame_max = 30.0f;

	bool camera_explanation = false;

	const float kCameraHight = 18.0f;
	float camera_hight = kCameraHight;
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