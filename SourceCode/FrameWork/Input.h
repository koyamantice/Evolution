#pragma once

#include <Windows.h>
#include <wrl.h>
#include "WinApp.h"
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>


#include <Xinput.h>

/// <summary>
/// ����
/// </summary>
class Input {
public:
	enum XBOX {
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
		LEFT = 0,
		RIGHT,
		MIDDLE,
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
	const float& GetPosX() { return posX; }
	const float& GetPosY() { return posY; }
	const float& GetRPosX() { return (float)xinputState.Gamepad.sThumbLX; }
	const float& GetRPosY() { return (float)xinputState.Gamepad.sThumbLY; }
	/// <summary>
	/// �}�E�X�ړ��ʂ��擾
	/// </summary>
	/// <returns>�}�E�X�ړ���</returns>
	MouseMove GetMouseMove();
	MousePoint GetPoint();


private: // �����o�ϐ�
	ComPtr<IDirectInput8> dinput;
	ComPtr<IDirectInputDevice8> devKeyboard;
	BYTE key[256] = {};
	BYTE keyPre[256] = {};
	ComPtr<IDirectInputDevice8> devMouse;
	DIMOUSESTATE2 mouseState = {};
	DIMOUSESTATE2 mouseStatePre = {};
	POINT mousePoint;

	//XINPUT(�R���g���[���[�p)
	XINPUT_STATE xinputState;
	XINPUT_STATE oldXinputState;
	float shakePower = 0.0f;
	int shakeTimer = 0;
	bool CheckTrigger(XBOX Button);
	bool CheckTilt(STICK Stick);

	//�f�b�h�]�[���ɓ����Ă��邩(DeadRate : �f�b�h�]�[������̓x�����A1.0f���ƃf�t�H���g)
	bool StickInDeadZone(StickPos& Thumb);


	float posX = 0;
	float posY = 0;
	float RposX = 0;
	float RposY = 0;

	WinApp* winApp;
};

