#pragma once
#include "SceneManager.h"
#include "Actor.h"
#include <SourceCode/FrameWork/collision/TouchableObject.h>
#include <PostEffect.h>
#include"Touch.h"

/// <summary>
/// �^�C�g���V�[��
/// </summary>

class FirstStage : public BattleScene {
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
	//�����J����
	void IntroCamera(int Timer);
	//�N���A��J����
	void ResultCamera(int Timer);
	//��ʂ�w����
	void ResetCamera();

	/// <summary>
	/// �����\���p�̍X�V
	/// </summary>
	void DescriptionUpdate() override;

	int count = 0;
	int speed = 1;
	float angle = 0;
	float firstangle = 0;	
	float endangle = 0;

	XMFLOAT2 firstdis{};
	float angleframe = 0;
	bool Reset = false;
	const float hight = 18.0f;

	XMFLOAT2 distance={0,0};
	XMFLOAT2 dis = { 16,7 };
	bool clear = false;
	float Cframe = 0.0f;
private:
	Actor* player_shadow = nullptr;
	Actor* enemy_shadow = nullptr;
	Actor* goal_shadow = nullptr;


	std::unique_ptr<Sprite> Clear{};
	std::unique_ptr<Sprite> Over{};

	//�R���g���[���[�̐���
	static const int intro_word_max = 6;
	std::unique_ptr<Sprite> IntroWord[intro_word_max];
	std::unique_ptr<Sprite> Camecon[6]{};
	std::unique_ptr<Sprite> con_vis[2]{};




	XMFLOAT2 base = { 640,530 };
	float camera_frame = 0.0f;
	int animation = 0;
	int tapanima = 3;
	int animafrate = 0;
	int vec = 1;
	int nowWord = 0;

	bool cameraExplanation = false;
	float IntroHight = 50.0f;
	float introFrame = 0;
	PostEffect* postEffect = nullptr;
};

