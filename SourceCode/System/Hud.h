#pragma once
#include"Actor.h"
#include <Sprite.h>
#include"Input.h"

class Hud {
private: // �G�C���A�X
	// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	//���͊֘A
	Input* input = Input::GetInstance();
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	Hud();
	
	/// <summary>
	///	�f�X�g���N�^
	/// </summary>
	~Hud();

	/// <summary>
	/// �X�V����
	/// </summary>
	void Update();

	/// <summary>
	///�`�揈��
	/// </summary>
	void Draw();
private:
	/// <summary>
	////����������
	/// </summary>
	void Initialize();

	/// <summary>
	/// �I������
	/// </summary>
	void Finalize();

	/// <summary>
	///�R���g���[���[�����̍X�V
	/// </summary>
	void CameraHudUpdate();

	/// <summary>
	///�{�^���̑�������̍X�V
	/// </summary>
	void BottonHudUpdate();

private:
	enum CameraControl {
		TRIGGERNORMAL = 0,
		TRIGGERPUSH,
		COMMENT,
		STICKLEFT,
		STICK,
		STICKRIGHT,
		CAMERACONTROLMAX
	};

	enum ButtonControl {
		GNOMESHOT=0,
		GNOMERECOVERY,
		BUTTONCONTROLMAX,
	};

	//�R���g���[���[�̐���
	std::unique_ptr<Sprite> camera_hud[CAMERACONTROLMAX]{};
	const XMFLOAT2 kCameraHudBase = { 640,530 };
	const XMFLOAT2 kVisibleBasicSize = { 128,128 };
	int visible_trigger = TRIGGERNORMAL;
	int visible_stick = STICKLEFT;
	int old_visible_stick = visible_stick;

	//�A�j���[�V�����؂�ւ��p
	int switch_frame = 0;
	const int kSwitchFrameMax = 30;

	//�������I���ƒ[�܂Ŋ񂹂�
	bool explation_finish = false;
	const XMFLOAT2 kCameraHudPos2Edge = { 90,680 };
	const XMFLOAT2 kCameraHudSize2Edge = { 64,64 };
	float edge_frame = 0.0f;
	const float kEdgeFrameMax = 200.0f;

	//�{�^���̐���
	std::unique_ptr<Sprite> button_hud[BUTTONCONTROLMAX]{};

	struct ButtonEasingSystem {
		bool isvisible = false;
		float ease_frame = 0.0f;
		float ease_frame_max = 100.0f;
		XMFLOAT2 s_pos = {};
		XMFLOAT2 e_pos = {};
	} ;


	ButtonEasingSystem button_system[BUTTONCONTROLMAX];	

};
