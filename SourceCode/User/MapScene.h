#pragma once
#include "SceneManager.h"
#include "Actor.h"
#include "PauseUI.h"
#include <SourceCode/FrameWork/collision/TouchableObject.h>
#include "Gauge.h"
#include <PostEffect.h>

/// <summary>
/// �^�C�g���V�[��
/// </summary>

class MapScene : public BaseScene {
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
	const float PI = 3.1452f;
	XMFLOAT2 distance = { 0,10 };
	XMFLOAT2 dis = { 0,15 };
	bool clear = false;
	float Cframe = 0.0f;
	XMFLOAT2 clearPos = { 0,-720 };
private:
	Actor* player_shadow = nullptr;
	Actor* enemy_shadow = nullptr;
	Actor* crystal_shadow = nullptr;
	ActorComponent* PlayerComp = nullptr;
	std::unique_ptr<Sprite> Clear{};
	std::unique_ptr<Sprite>	Vignette{};
	std::unique_ptr<Gauge>	Demo;
	std::unique_ptr<Object3d> skydome{};
	std::unique_ptr<TouchableObject> ground{};
	std::unique_ptr<PauseUI> pauseUi{};
	PostEffect* postEffect = nullptr;
};

