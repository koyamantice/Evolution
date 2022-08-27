#pragma once

class SceneManager;
#include "DirectXCommon.h"
#include "DebugCamera.h"
#include "LightGroup.h"
#include<memory>
#include "Sprite.h"
#include "FBXObject3d.h"
#include "Object3d.h"
#include "Texture.h"
#include <DirectXMath.h>
#include "input.h"
#include "FPSManager.h"
#include<memory>
#include "ParticleManager.h"

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
	//�e�N���X�̂���
	DebugCamera* camera = { nullptr };
	bool pause = false;
	float ambientColor0[3] = { 1,1,1 };

	// �������������l
	float lightDir0[3] = { 0,0,1 };
	float lightColor0[3] = { 1,0,0 };

	float lightDir1[3] = { 0,1,0 };
	float lightColor1[3] = { 0,1,0 };

	float lightDir2[3] = { 1,0,0 };
	float lightColor2[3] = { 0,0,1 };

	float pointLightPos[3] = { 0,0,0 };
	float pointLightColor[3] = { 1,1,1 };
	float pointLightAtten[3] = { 0.3f,0.1f,0.1f };
	int EffectNum = 0;
	float distanceY = 12;
	float distanceZ = 12;

	//�n�܂�O�̉��o�̂��߂̕ϐ�
	bool bossstart = false;
	int appearanceNumber = 0;
	int appearanceTimer = 0;
	XMFLOAT3 cameraPos = { 0.0f,0.0f,0.0f };
	XMFLOAT3 cameratargetPos = { 0.0f,0.0f,0.0f };
	XMFLOAT3 Aftereyepos;
	XMFLOAT3 Aftertargetpos;
	ParticleManager* particleMan;
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

};

