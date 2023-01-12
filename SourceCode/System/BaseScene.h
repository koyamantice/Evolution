#pragma once
class SceneManager;
#include "DirectXCommon.h"
#include "DebugCamera.h"
#include "LightGroup.h"
#include<memory>
#include "Sprite.h"
#include "FBXObject3d.h"
#include "Object3d.h"
#include <DirectXMath.h>
#include "input.h"
#include "FPSManager.h"
#include "ParticleManager.h"
#include"ImageManager.h"
#include <SourceCode/User/PauseUI.h>
/// <summary>
/// �V�[���K��
/// </summary>
class BaseScene {
protected:
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;
protected:
	LightGroup* lightGroup = nullptr;
	DebugCamera* camera = { nullptr };
	ParticleManager* partMan = nullptr;
	bool pause = false;

	std::unique_ptr<PauseUI> pauseUi{};

	std::unique_ptr<Sprite>	Screen[2]{};
	std::unique_ptr<Sprite> FeedBlack{};
	std::unique_ptr<Sprite> Effect;
	bool Change = true;
	float alpha = 0.0f;
	float frame = 0.0f;

	float feedAlpha = 0.0f;

	//kill�N���b�v
	int finishTime = 0;

	bool Intro = false;
	bool Result = false;
	bool GameOver = false;

public:
	virtual ~BaseScene()=default;
	/// <summary>
	/// ������
	/// </summary>
	virtual void Initialize(DirectXCommon* dxCommon) = 0;
	/// <summary>
	/// 
	/// </summary>
	/// <param name="dxCommon"></param>
	void InitCommon(DirectXCommon* dxCommon);

	/// <summary>
	/// �I��
	/// </summary>
	virtual void Finalize()=0;
	/// <summary>
	/// ���t���[���X�V
	/// </summary>
	virtual void Update(DirectXCommon* dxCommon)=0;
	/// <summary>
	/// �`��
	/// </summary>
	virtual void Draw(DirectXCommon* dxCommon)=0;

	void Feed(const std::string& sceneName);
};

