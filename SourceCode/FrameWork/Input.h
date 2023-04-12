#pragma once
#include "WinApp.h"

#define DIRECTINPUT_VERSION 0x0800

#include <Windows.h>
#include <wrl.h>
#include <dinput.h>
#include <Xinput.h>

/// <summary>
/// ����
/// </summary>
class Input {
public:
	enum XBOX {
		UP = XINPUT_GAMEPAD_DPAD_UP,
		DOWN = XINPUT_GAMEPAD_DPAD_DOWN,
		LEFT = XINPUT_GAMEPAD_DPAD_LEFT,
		RIGHT = XINPUT_GAMEPAD_DPAD_RIGHT,
		B = XINPUT_GAMEPAD_B,
		A = XINPUT_GAMEPAD_A,
		X = XINPUT_GAMEPAD_X,
		Y = XINPUT_GAMEPAD_Y,
		START = XINPUT_GAMEPAD_START,
		BACK = XINPUT_GAMEPAD_BACK,
		LB = XINPUT_GAMEPAD_LEFT_SHOULDER,
		RB = XINPUT_GAMEPAD_RIGHT_SHOULDER,
		LT, RT
	};
	//�X�e�B�b�N
	enum STICK {
		L_UP, L_DOWN, L_LEFT, L_RIGHT,
		R_UP, R_DOWN, R_LEFT, R_RIGHT, XBOX_STICK_NUM
	};


	enum MouseButton {
		MouseLEFT = 0,
		MouseRIGHT,
		MouseMIDDLE,
	};
	struct MouseMove {
		LONG    lX;
		LONG    lY;
		LONG    lZ;
	};
	struct MousePoint {
		LONG    lX;
		LONG    lY;
	};
	struct StickPos {
		float x;
		float y;
	};
private: // �G�C���A�X
	// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public: // �����o�֐�
	static Input* GetInstance();
	/// <summary>
	/// ������
	/// </summary>
	/// <returns>����</returns>
	bool Initialize(WinApp* winApp);

	/// <summary>
	/// ���t���[������
	/// </summary>
	void Update();

	/// <summary>
	///�I��
	/// </summary>
	void Finalize();
	/// <summary>
	/// �L�[�̉������`�F�b�N
	/// </summary>
	/// <param name="keyNumber">�L�[�ԍ�( DIK_0 ��)</param>
	/// <returns>������Ă��邩</returns>
	bool PushKey(BYTE keyNumber);

	/// <summary>
	/// �L�[�̃g���K�[���`�F�b�N
	/// </summary>
	/// <param name="keyNumber">�L�[�ԍ�( DIK_0 ��)</param>
	/// <returns>�g���K�[��</returns>
	bool TriggerKey(BYTE keyNumber);
	/// <summary>
	/// �L�[�̃����[�X���`�F�b�N
	/// </summary>
	/// <param name="keyNumber">�L�[�ԍ�( DIK_0 ��)</param>
	/// <returns>�����[�X��</returns>
	bool ReleaseKey(BYTE keyNumber);

	/// <summary>
	/// �L�[�̒��{�^���������`�F�b�N
	/// </summary>
	/// <returns>������Ă��邩</returns>
	bool PushMouse(MouseButton Button);

	/// <summary>
	/// �L�[�̍��{�^���g���K�[���`�F�b�N
	/// </summary>
	/// <returns>�g���K�[��</returns>
	bool TriggerMouse(MouseButton Button);

	//�{�^���z�u
	bool TriggerButton(XBOX Button);
	bool PushButton(XBOX Button);
	//�g���K�[
	bool TiltStick(STICK Stick);
	//�v�b�V��
	bool TiltPushStick(STICK Stick,float DeadZone=0.3f);

public:
	const float& GetPosX() { return mouse_posX_; }
	const float& GetPosY() { return mouse_posY_; }
	const float& GetLeftControllerX() { return left_stick_posX_; }
	const float& GetLeftControllerY() { return left_stick_posY_; }
	/// <summary>
	/// �}�E�X�ړ��ʂ��擾
	/// </summary>
	/// <returns>�}�E�X�ړ���</returns>
	MouseMove GetMouseMove();
	MousePoint GetPoint();


private: // �����o�ϐ�
	ComPtr<IDirectInput8> dinput_;
	ComPtr<IDirectInputDevice8> devKeyboard_;
	BYTE key_[256] = {};
	BYTE keyPre_[256] = {};
	ComPtr<IDirectInputDevice8> devMouse_;
	DIMOUSESTATE2 mouseState_ = {};
	DIMOUSESTATE2 mouseStatePre_ = {};
	POINT mousePoint_;

	//XINPUT(�R���g���[���[�p)
	XINPUT_STATE xinputState_;
	XINPUT_STATE xinputStatePre_;

	float shakePower_ = 0.0f;
	int shakeTimer_ = 0;

	bool CheckTrigger(XBOX Button);

	//�f�b�h�]�[���ɓ����Ă��邩(DeadRate : �f�b�h�]�[������̓x�����A1.0f���ƃf�t�H���g)
	bool StickInDeadZone(StickPos& Thumb);

	float mouse_posX_ = 0;
	float mouse_posY_ = 0;
	float left_stick_posX_ = 0;
	float left_stick_posY_ = 0;

	WinApp* winApp_;
};

