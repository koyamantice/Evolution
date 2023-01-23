#pragma once

#include <memory>
#include "ParticleManager.h"

class ParticleEmitter {
private: // �G�C���A�X
	// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public: // �ÓI�����o�֐�
	//

public: // �����o�֐�
	ParticleEmitter(const UINT& texnumber);
	/// <summary>
	/// �ǉ�
	/// </summary>
	/// <param name="num">��</param>
	/// <param name="life">��������</param>
	/// <param name="position">���W</param>
	/// <param name="model">���f��</param>
	void Add(const int& num, const int& life,
		const XMFLOAT3& position, const XMFLOAT3& average_vel, const XMFLOAT3& accel,
		const float& start_scale, const float& end_scale,
		const XMFLOAT4& start_color, const XMFLOAT4& end_color);

	/// <summary>
	/// ��{�I�ȃp�[�e�B�N�����[�V����
	/// </summary>
	/// <param name="life"></param>
	/// <param name="position"></param>
	/// <param name="average_vel"></param>
	/// <param name="accel"></param>
	/// <param name="start_scale"></param>
	/// <param name="end_scale"></param>
	/// <param name="start_color"></param>
	/// <param name="end_color"></param>
	void AddCommon(const int& life,
		const XMFLOAT3& position, const float& average_vel, const float& average_accel,
		const float& start_scale=1.0f, const float& end_scale = 1.0f,
		const XMFLOAT4& start_color={1,1,1,1}, const XMFLOAT4& end_color={1,1,1,1});


	void AddInNest(const int& life,
		const XMFLOAT3& position, const float& average_margin, const float& average_vel,
		const float& start_scale = 1.0f, const float& end_scale = 0.0f,
		const XMFLOAT4& start_color = { 1.0f,1.0f,0.0f,0.8f }, const XMFLOAT4& end_color = { 1.0f,1.0f,0.0f,0.0f });

	void AddContraction(const int& _life,
		const XMFLOAT3& _position,
		const float& _area,
		const float& _average_vel,
		const float& _start_scale = 1.5f, const float& _end_scale = 0.0f,
		const XMFLOAT4& _start_color = { 0.5f,1.0f,0.5f,1.0f }, const XMFLOAT4& _end_color = { 0.5f,1.0f,0.5f,1.0f });

	void AddCrystalMove(const int& _life,
		const XMFLOAT3& _position,
		const float& _area,
		const float& _average_vel,
		const float& _start_scale = 1.5f, const float& _end_scale = 0.0f,
		const XMFLOAT4& _start_color = { 0.5f,1.0f,0.5f,1.0f }, const XMFLOAT4& _end_color = { 0.5f,1.0f,0.5f,1.0f });

	/// <summary>
	/// ���t���[������
	/// </summary>
	void Update();
	/// <summary>
	/// �`��
	/// </summary>
	/// <param name="cmdList">�`��R�}���h���X�g</param>
	void Draw(blendType type = alphaBle);

	

private: // �����o�ϐ�
	//���W
	XMFLOAT3 position_ = {};
	//���x
	XMFLOAT3 velocity_ = {};
	//�����x
	XMFLOAT3 accel_ = {};
	// �X�P�[�������l
	float startScale_ = 1.0f;
	XMFLOAT3 objStartScale_ = { 1.0f, 1.0f, 1.0f };
	// �X�P�[���ŏI�l
	float endScale_ = 0.0f;
	XMFLOAT3 objEndScale_ = { 0.0f, 0.0f, 0.0f };
	// �F(RGBA)�����l
	XMFLOAT4 startColor_ = { 1, 1, 1, 1 };
	// �F(RGBA)�ŏI�l
	XMFLOAT4 endColor_ = {};
	//X,Y,Z�S��[-5.0,+5.0]�Ń����_���ɕ��z
	float mdPos_ = 10.0f;
	XMFLOAT3 objMdPos_ = { 10.0f, 10.0f, 10.0f };
	//X,Y,Z�S��[-0.05,+0.05]�Ń����_���ɕ��z
	float mdVel_ = 0.1f;
	//�d�͂Ɍ����Ă�Y�̂�[-0.001f,0]�Ń����_���ɕ��z
	float mdAcc_ = 0.001f;
	std::unique_ptr<ParticleManager> particleManager = nullptr;
};