#pragma once
class SceneManager;
#include "FPSManager.h"
#include "input.h"
#include "DirectXCommon.h"

#include "DebugCamera.h"
#include "LightGroup.h"

#include<string>
#include <fstream>
#include <iostream>

#include <memory>
#include <DirectXMath.h>

#include "FBXObject3d.h"
#include "Object3d.h"
#include "Sprite.h"

#include "ModelManager.h"
#include "ImageManager.h"
#include "AudioManager.h"
#include "ActorManager.h"

#include <SourceCode/Common/Easing.h>
#include <SourceCode/User/PauseUI.h>
#include <SceneChanger.h>
#include <ParticleEmitter.h>


enum SceneNum{
	kFirstScene=0,
	kMSecondScene,
	kSecondScene,
};


/// <summary>
/// ���V�[��
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
	std::unique_ptr<DebugCamera> camera =  nullptr ;
	std::unique_ptr<LightGroup> lightGroup = nullptr;

	std::unique_ptr<SceneChanger> sceneChanger_ = nullptr;
	std::unique_ptr <ParticleEmitter> particleEmitter = nullptr;
	std::unique_ptr<AudioManager> audioManager = nullptr;

	bool isDebug = false;
	bool pause = false;
	bool scene_first_change = true;
	bool intro_skip = false;
public:
	virtual ~BaseScene()=default;
	/// <summary>
	/// ������
	/// </summary>
	virtual void Initialize(DirectXCommon* dxCommon) = 0;
	/// <summary>
	/// ���t���[���X�V
	/// </summary>
	virtual void Update(DirectXCommon* dxCommon) = 0;
	/// <summary>
	/// �`��
	/// </summary>
	virtual void Draw(DirectXCommon* dxCommon) = 0;
	/// <summary>
	/// �f�o�b�N�`��
	/// </summary>
	/// <returns>��</returns>
	virtual bool DebugDraw() { return false; };
	/// <summary>
	/// �I��
	/// </summary>
	virtual void Finalize() = 0;
	/// <summary>
	/// ���ʂ̏�����
	/// </summary>
	/// <param name="dxCommon"></param>
	void InitCommon(DirectXCommon* dxCommon);
};

