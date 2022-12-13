#pragma once

#include <string>
#include <vector>
#include <DirectXMath.h>

// ���x���f�[�^
struct LevelData {

	struct ObjectData {
		// �t�@�C����
		std::string fileName;
		// ���s�ړ�
		DirectX::XMVECTOR translation;
		// ��]�p
		DirectX::XMVECTOR rotation;
		// �X�P�[�����O
		DirectX::XMVECTOR scaling;
	};

	struct CameraData {

		DirectX::XMFLOAT3 eye;
		DirectX::XMFLOAT3 target = { 0,0,0 };
	};
	// �I�u�W�F�N�g�z��
	std::vector<ObjectData> objects;
	CameraData camera;

};


// ���x���f�[�^�̃��[�_�[
class LevelLoader {

public:// �萔
	// �f�t�H���g�̓ǂݍ��݃f�B���N�g��
	static const std::string kDefaultBaseDirectory;
	// �t�@�C���g���q
	static const std::string kExtension;

public:// �����o�֐�

	/// <summary>
	/// ���x���f�[�^�t�@�C���̓ǂݍ���
	/// </summary>
	/// <param name="fileName">�t�@�C����</param>
	static LevelData* LoadFile(const std::string& fileName);

	/// <summary>
	/// ���x���f�[�^�t�@�C���̓ǂݍ���
	/// </summary>
	/// <param name="fileName">�t�@�C����</param>
	static LevelData* LoadCamera(const std::string& fileName);

};

