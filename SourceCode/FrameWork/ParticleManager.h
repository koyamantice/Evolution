#pragma once

#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>
#include <forward_list>

#include "Camera.h"

/// <summary>
/// �p�[�e�B�N���}�l�[�W��
/// </summary>
class ParticleManager {
private: // �G�C���A�X
	// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public: // �T�u�N���X
	// ���_�f�[�^�\����
	struct VertexPos {
		XMFLOAT3 pos; // xyz���W
		float scale; // �X�P�[��
	};

	// �萔�o�b�t�@�p�f�[�^�\����
	struct ConstBufferData {
		XMMATRIX mat;	// �r���[�v���W�F�N�V�����s��
		XMMATRIX matBillboard;	// �r���{�[�h�s��
	};

	//�^�C�v
	enum blendType {
		alphaBle= 0,
		addBle,
		subBle
	};

	// �p�C�v���C���Z�b�g
	struct PipelineSet
	{
		// ���[�g�V�O�l�`��
		ComPtr<ID3D12RootSignature> rootsignature;
		// �p�C�v���C���X�e�[�g�I�u�W�F�N�g
		ComPtr<ID3D12PipelineState> pipelinestate;
	};
	// �p�[�e�B�N��1��
	class Particle {
		// Microsoft::WRL::���ȗ�
		template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
		// DirectX::���ȗ�
		using XMFLOAT2 = DirectX::XMFLOAT2;
		using XMFLOAT3 = DirectX::XMFLOAT3;
		using XMFLOAT4 = DirectX::XMFLOAT4;
		using XMMATRIX = DirectX::XMMATRIX;

	public:
		// ���W
		XMFLOAT3 position = {};
		// ���x
		XMFLOAT3 velocity = {};
		// �����x
		XMFLOAT3 accel = {};
		// �F
		XMFLOAT3 color = {};
		// �X�P�[��
		float scale = 1.0f;
		// ��]
		float rotation = 0.0f;
		// �����l
		XMFLOAT3 s_color = {};
		float s_scale = 1.0f;
		float s_rotation = 0.0f;
		// �ŏI�l
		XMFLOAT3 e_color = {};
		float e_scale = 0.0f;
		float e_rotation = 0.0f;
		// ���݃t���[��
		int frame = 0;
		// �I���t���[��
		int num_frame = 0;

	};

private: // �萔
	static const int vertexCount = 65536;		// ���_��

public:// �ÓI�����o�֐�
	static ParticleManager* GetInstance();

public: // �����o�֐�	
	/// <summary>
	/// ������
	/// </summary>
	/// <returns></returns>
	void CreateCommon(ID3D12Device* device, Camera* camera, ID3D12GraphicsCommandList* cmdList);
	
	/// <summary>
	/// ������
	/// </summary>
	/// <returns></returns>
	void Initialize(const std::string& filename);
	
	/// <summary>
	/// ���t���[������
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw(blendType type=addBle);

	///// <summary>
	///// �J�����̃Z�b�g
	///// </summary>
	///// <param name="camera">�J����</param>
	//inline void SetCamera(Camera* camera) { this->camera = camera; }

	/// <summary>
	/// �p�[�e�B�N���̒ǉ�
	/// </summary>
	/// <param name="life">��������</param>
	/// <param name="position">�������W</param>
	/// <param name="velocity">���x</param>
	/// <param name="accel">�����x</param>
	/// <param name="start_scale">�J�n���X�P�[��</param>
	/// <param name="end_scale">�I�����X�P�[��</param>
	void Add(int life, XMFLOAT3 position, XMFLOAT3 velocity, XMFLOAT3 accel, float start_scale, float end_scale);

	/// <summary>
	/// �f�X�N���v�^�q�[�v�̏�����
	/// </summary>
	/// <returns></returns>
	void InitializeDescriptorHeap();

	/// <summary>
	/// �O���t�B�b�N�p�C�v���C������
	/// </summary>
	/// <returns>����</returns>
	static void InitializeGraphicsPipeline();
	/// <summary>
	/// ���Z�����p�C�v���C������
	/// </summary>
	static void CreateAddBlendPipeline();

	/// <summary>
	/// ���Z�����p�C�v���C������
	/// </summary>
	static void CreateSubBlendPipeline();

	/// <summary>
	/// �e�N�X�`���ǂݍ���
	/// </summary>
	/// <returns>����</returns>
	void LoadTexture(const std::string& filename);

	/// <summary>
	/// ���f���쐬
	/// </summary>
	void CreateModel();

private: // �����o�ϐ�
	// �f�o�C�X
	static ID3D12Device* device;
	//�R�}���h���X�g
	static ID3D12GraphicsCommandList* cmdList;
	// �f�X�N���v�^�T�C�Y
	UINT descriptorHandleIncrementSize = 0u;
	//���Z�����p�C�v���C���Z�b�g
	static PipelineSet addBlendPipelineSet;
	//���Z�����p�C�v���C���Z�b�g
	static PipelineSet subBlendPipelineSet;
	//�����������p�C�v���C���Z�b�g
	static PipelineSet alphaBlendPipelineSet;
	// �J����
	static Camera* camera;
	//�摜�ǂݍ���
	static std::string directoryPath;
	//�g���q
	static std::string extensionPath;
	// �f�X�N���v�^�q�[�v
	ComPtr<ID3D12DescriptorHeap> descHeap;
	// ���_�o�b�t�@
	ComPtr<ID3D12Resource> vertBuff;
	// �e�N�X�`���o�b�t�@
	ComPtr<ID3D12Resource> texbuff;
	// �V�F�[�_���\�[�X�r���[�̃n���h��(CPU)
	CD3DX12_CPU_DESCRIPTOR_HANDLE cpuDescHandleSRV;
	// �V�F�[�_���\�[�X�r���[�̃n���h��(CPU)
	CD3DX12_GPU_DESCRIPTOR_HANDLE gpuDescHandleSRV;
	// ���_�o�b�t�@�r���[
	D3D12_VERTEX_BUFFER_VIEW vbView;
	// �萔�o�b�t�@
	ComPtr<ID3D12Resource> constBuff;
	// �p�[�e�B�N���z��
	std::forward_list<Particle> particles;


private:
	ParticleManager() = default;
	ParticleManager(const ParticleManager&) = delete;
	~ParticleManager() = default;
	ParticleManager& operator=(const ParticleManager&) = delete;
};

