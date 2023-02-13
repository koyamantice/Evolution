#pragma once

#include "FBXModel.h"
#include "Camera.h"
#include "LightGroup.h"

#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <d3dx12.h>

#include <string>

class FBXObject3d {
protected: // �G�C���A�X
	// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
public:	//�萔
	//�{�[���̍ő吔
	static const int MAX_BONES = 32;

	//�萔�o�b�t�@�p�f�[�^�\���́i�X�L�j���O�j
	struct ConstBufferDataSkin {
		XMMATRIX bones[MAX_BONES];
	};
public: // �T�u�N���X
	// �萔�o�b�t�@�p�f�[�^�\���́i���W�ϊ��s��p�j
	struct ConstBufferDataTransform {
		XMMATRIX viewproj;    // �r���[�v���W�F�N�V�����s��
		XMMATRIX world; // ���[���h�s��
		XMFLOAT3 cameraPos; // �J�������W�i���[���h���W�j
		XMFLOAT4 color;	// �F (RGBA)
	};
	// �萔�o�b�t�@�p�f�[�^�\����B1
	struct ConstBufferDataB1 {
		XMFLOAT3 ambient; // �A���r�G���g�W��
		float pad1; // �p�f�B���O
		XMFLOAT3 diffuse; // �f�B�t���[�Y�W��
		float pad2; // �p�f�B���O
		XMFLOAT3 specular; // �X�y�L�����[�W��
		float alpha;	// �A���t�@
	};
	// �萔�o�b�t�@
	ComPtr<ID3D12Resource> constBuff;

	// �A�j���[�V�����p�f�[�^�\����
	struct AnimationInfo {
		std::string name;
		FbxAnimStack* stack;
		FbxTakeInfo* fbxinfo;
		float start;
		float end;
	};
public: // �ÓI�����o�֐�
	/// <summary>
	/// �O���t�B�b�N�p�C�v���C���̐���
	/// </summary>
	static void CreateGraphicsPipeline();
	/// <summary>
	///�f�o�C�X�Z�b�g
	/// </summary>
	/// <returns>�f�o�C�X</returns>
	static void SetDevice(ID3D12Device* device) { FBXObject3d::device = device; }
	/// <summary>
	///�J�����Z�b�g
	/// </summary>
	/// <returns>�J����</returns>
	static void SetCamera(Camera* camera) { FBXObject3d::camera = camera; }
	/// <summary>
	///���C�g�Z�b�g
	/// </summary>
	/// <returns>���C�g�O���[�v</returns>
	static void SetLightGroup(LightGroup* lightGroup) {
		FBXObject3d::lightGroup = lightGroup;
	}

	/// <summary>
	/// ���ʂ̏���������
	/// </summary>
	/// <param name="device">�f�o�C�X</param>
	/// <param name="camera">�J����</param>
	/// <param name="lightGroup">���C�g�O���[�v</param>
	static void StaticInitializeCommon(ID3D12Device* device, Camera* camera, LightGroup* lightGroup);

private: // �ÓI�����o�ϐ�
	// �f�o�C�X
	static ID3D12Device* device;
	// �J����
	static Camera* camera;
	// ���[�g�V�O�l�`��
	static ComPtr<ID3D12RootSignature> rootsignature;
	// �p�C�v���C���X�e�[�g�I�u�W�F�N�g
	static ComPtr<ID3D12PipelineState> pipelinestate;
	// ���C�g
	static LightGroup* lightGroup;

public: // �����o�֐�
	/// <summary>
	/// ������
	/// </summary>
	void Initialize();

	/// <summary>
	/// ���t���[������
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw(ID3D12GraphicsCommandList* cmdList);

	void SetModel(FBXModel* model) { this->model = model; }


	/// <summary>
	/// ���W�̎擾
	/// </summary>
	/// <returns>���W</returns>
	const XMFLOAT3& GetPosition() { return position; }

	const XMFLOAT3& GetRotation() { return rotation; }

	/// <summary>
	/// ���W�̐ݒ�
	/// </summary>
	/// <param name="position">���W</param>
	void SetPosition(XMFLOAT3 position) { this->position = position; }

	void SetRotation(XMFLOAT3 rotation) { this->rotation = rotation; }
	void SetColor(XMFLOAT4 color) { this->color = color; }
	/// <summary>
	/// �X�P�[���̐ݒ�
	/// </summary>
	/// <param name="position">�X�P�[��</param>
	void SetScale(XMFLOAT3 scale) { this->scale = scale; }
	const XMFLOAT3& GetScale() { return scale; }

	/// <summary>
	/// �X�P�[���̐ݒ�
	/// </summary>
	/// <param name="position">�X�P�[��</param>
	const bool& GetIsFinish() { return isFinish; }


	/// <summary>
	/// �A�j���[�V����
	/// </summary>
	void PlayAnimation(const int& num = 0);
	void ReverseAnimation(const int& num = 0);
	void StopAnimation();
	void ResetAnimation();
	//����fbx�̓ǂݍ���
	void LoadAnimation();
protected: // �����o�ϐ�
	// �萔�o�b�t�@
	ComPtr<ID3D12Resource> constBuffTransform;
	// ���[�J���X�P�[��
	XMFLOAT3 scale = { 1,1,1 };
	// X,Y,Z�����̃��[�J����]�p
	XMFLOAT3 rotation = { 0,0,0 };
	// ���[�J�����W
	XMFLOAT3 position = { 0,0,0 };
	//�F
	XMFLOAT4 color = { 1,1,1,1 };

	XMFLOAT3 ambient;	// �A���r�G���g�e���x
	XMFLOAT3 diffuse;	// �f�B�t���[�Y�e���x
	XMFLOAT3 specular;	// �X�y�L�����[�e���x
	float alpha;		// �A���t�@

	// ���[�J�����[���h�ϊ��s��
	XMMATRIX matWorld;
	// ���f��
	FBXModel* model = nullptr;
	//�萔�o�b�t�@�i�X�L���j
	ComPtr<ID3D12Resource> constBuffSkin;
	//1�t���[��
	FbxTime frameTime;
	//�A�j���[�V�����J�n����
	FbxTime startTime;
	//�A�j���[�V�����I������
	FbxTime endTime;
	//���ݎ��ԁi�A�j���[�V�����j
	FbxTime currentTime;
	//�A�j���[�V�����Đ���
	bool isPlay = false;
	bool isReverse = false;
	//�A�j���[�V�����I���t���[��
	bool isFinish = false;
	//�A�j���[�V�����i�[
	std::vector<AnimationInfo> Animations;
};

