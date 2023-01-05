#pragma once

#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>
#include "Camera.h"

class Object2d
{
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
	struct VertexPosNormalUv
	{
		XMFLOAT3 pos; // xyz���W
		XMFLOAT3 normal; // �@���x�N�g��
		XMFLOAT2 uv;  // uv���W
	};

	// �萔�o�b�t�@�p�f�[�^�\����
	struct ConstBufferData
	{
		XMFLOAT4 color;	// �F (RGBA)
		XMMATRIX mat;	// �R�c�ϊ��s��
	};
	enum blendtype{
		AlphaBlend=0,
		AddBlend
	};
private: // �萔
	static const int vertexCount = 4;		// ���_��
	static const int indexCount = 3 * 2;
public: // �ÓI�����o�֐�
	/// �ÓI������
	static bool StaticInitialize(ID3D12Device* device, ID3D12GraphicsCommandList* cmdList, int window_width, int window_height, Camera* camera=nullptr);

	/// �e�N�X�`���ǂݍ���
	static bool LoadTexture(UINT texnumber, const wchar_t* filename);

	/// �`��O����
	static void PreDraw(blendtype type = AlphaBlend);

	/// �`��㏈��
	static void PostDraw();
	//�e�N�X�`������
	static Object2d* Create(UINT texNumber, XMFLOAT3 position, XMFLOAT3 size={1.0f,1.0f,1.0f}, XMFLOAT4 color = { 1.0f,1.0f,1.0f,1.0f });

	/// <summary>
	/// �J�����Z�b�g
	/// </summary>
	/// <returns></returns>
	static void SetCamera(Camera* camera) {
		Object2d::camera = camera;
	}

private: // �ÓI�����o�ϐ�
	static const int srvCount = 213;
	// �f�o�C�X
	static ID3D12Device* device;
	// �f�X�N���v�^�T�C�Y
	static UINT descriptorHandleIncrementSize;
	// �R�}���h���X�g
	static ID3D12GraphicsCommandList* cmdList;
	// ���[�g�V�O�l�`��
	static ComPtr<ID3D12RootSignature> rootsignature[2];
	// �p�C�v���C���X�e�[�g�I�u�W�F�N�g
	static ComPtr<ID3D12PipelineState> pipelinestate[2];
	// �f�X�N���v�^�q�[�v
	static ComPtr<ID3D12DescriptorHeap> descHeap;
	// ���_�o�b�t�@
	static ComPtr<ID3D12Resource> vertBuff;//
	// �C���f�b�N�X�o�b�t�@
	static ComPtr<ID3D12Resource> indexBuff;//�A���J�[�쐬��static���O
	// �e�N�X�`���o�b�t�@
	static ComPtr<ID3D12Resource> texbuff[srvCount];
	// �V�F�[�_���\�[�X�r���[�̃n���h��(CPU)
	static CD3DX12_CPU_DESCRIPTOR_HANDLE cpuDescHandleSRV;
	// �V�F�[�_���\�[�X�r���[�̃n���h��(CPU)
	static CD3DX12_GPU_DESCRIPTOR_HANDLE gpuDescHandleSRV;
	// �r���[�s��
	static XMMATRIX matView;
	// �ˉe�s��
	static XMMATRIX matProjection;
	// ���_���W
	static XMFLOAT3 eye;
	// �����_���W
	static XMFLOAT3 target;
	// ������x�N�g��
	static XMFLOAT3 up;
	// �r���{�[�h�s��
	static XMMATRIX matBillboard;
	// Y�����r���{�[�h�s��
	static XMMATRIX matBillboardY;
	// �J����
	static Camera* camera;
	// ���_�o�b�t�@�r���[
	static D3D12_VERTEX_BUFFER_VIEW vbView;
	// �C���f�b�N�X�o�b�t�@�r���[
	static D3D12_INDEX_BUFFER_VIEW ibView;
	// ���_�f�[�^�z��
	static VertexPosNormalUv vertices[vertexCount];
	// ���_�C���f�b�N�X�z��
	static unsigned short indices[indexCount];
	// ���_�V�F�[�_�I�u�W�F�N�g
	static ComPtr<ID3DBlob> vsBlob;
	// �s�N�Z���V�F�[�_�I�u�W�F�N�g
	static ComPtr<ID3DBlob> psBlob;
	// �G���[�I�u�W�F�N�g
	static ComPtr<ID3DBlob> errorBlob; 
private:
	UINT texNumber = 0;

private:// �ÓI�����o�֐�
	/// �f�X�N���v�^�q�[�v�̏�����
	static bool InitializeDescriptorHeap();

	/// �J����������
	static void InitializeCamera(int window_width, int window_height);

	/// �O���t�B�b�N�p�C�v���C������
	static bool AlphaInitializeGraphicsPipeline();
	static bool AddInitializeGraphicsPipeline();

	/// �r���[�s����X�V
	static void UpdateViewMatrix();

public: // �����o�֐�
	void Object2dCreate();
	//�R���X�g���N�^
	Object2d(UINT texNumber, XMFLOAT3 position, XMFLOAT3 size = { 1.0f,1.0f,1.0f }, XMFLOAT4 color = { 1.0f,1.0f,1.0f,1.0f });
	//����������
	bool Initialize();
	/// ���t���[������
	void Update();
	/// �`��
	void Draw();
	static void Finalize();
	/// ���W�̎擾
	const XMFLOAT3& GetPosition() { return position; }

	/// ���W�̐ݒ�
	void SetPosition(XMFLOAT3 position) { this->position = position; }
	void SetPosition(float x,float y,float z) 
	{ this->position.x =x; 
	this->position.y = y;
	this->position.z = z;
	}
	void SetIsBillboard(const bool& isBillboard);
	void SetIsBillboardY(const bool& isBillboardY);

	void SetColor(XMFLOAT4 color);
	void SetRotation(const XMFLOAT3& rotation) { this->rotation = rotation; }
	const XMFLOAT3& GetRotation() { return this->rotation; }
	void SetScale(XMFLOAT3 scale) { this->scale = scale; }
	const XMFLOAT3& GetScale() { return scale; }
private: // �����o�ϐ�
	ComPtr<ID3D12Resource> constBuff; // �萔�o�b�t�@
	// �F
	XMFLOAT4 color = { 1,1,1,1 };
	// ���[�J���X�P�[��
	XMFLOAT3 scale = { 1,1,1 };
	// X,Y,Z�����̃��[�J����]�p
	XMFLOAT3 rotation = { 0,0,0 };
	// ���[�J�����W
	XMFLOAT3 position = { 0,0,0 };
	// ���[�J�����[���h�ϊ��s��
	XMMATRIX matWorld;
	// �r���{�[�h
	bool isBillboard = false;
	// Y���r���{�[�h
	bool isBillboardY = false;
};


