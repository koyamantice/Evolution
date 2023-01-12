#pragma once
#include "SceneManager.h"
#include "Actor.h"
#include "PauseUI.h"
#include <SourceCode/FrameWork/collision/TouchableObject.h>
#include <PostEffect.h>
#include"Touch.h"

/// <summary>
/// �^�C�g���V�[��
/// </summary>

class MapScene : public BaseScene {
private:
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

	//
	void SkydomeUpdate();
	//
	void SkydomeSunny(int time);


	//
	void LoadData();
	//
	void UpdateCommand();

	float RandHeight(const float& base);
	int count = 0;
	int speed = 1;
	float angle = 0;
	float firstangle = 0;
	float endangle = 0;

	XMFLOAT2 firstdis{};
	float angleframe = 0;
	bool Reset = false;
	XMFLOAT2 distance = { 0,0 };
	XMFLOAT2 dis = { 16,7 };
	bool clear = false;
	float Cframe = 0.0f;

	Actor* player_shadow = nullptr;
	Actor* enemy_shadow = nullptr;
	Actor* goal_shadow = nullptr;


	std::stringstream touch_pop;
	std::list<std::unique_ptr<Touch>> touchs;
	const float hight = 18.0f;

	std::unique_ptr<Sprite> Clear{};
	std::unique_ptr<Sprite> Over{};
	std::unique_ptr<Sprite> IntroWord[10];

	PostEffect* postEffect = nullptr;

	std::unique_ptr<Object3d> kSkydome{};

	std::unique_ptr<TouchableObject> ground{};

	float CameraAlpha = 1.0f;
	int animation = 0;
	int tapanima = 3;
	int animafrate = 0;
	int vec = 1;
	int nowWord = 0;

	bool cameraExplanation = false;
	float IntroHight = 50.0f;
	float introFrame = 0;
};

