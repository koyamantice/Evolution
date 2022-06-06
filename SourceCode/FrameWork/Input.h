#pragma once

#include <Windows.h>
#include <wrl.h>
#include "WinApp.h"
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

/// <summary>
/// ����
/// </summary>
class Input {
public:
	//�Q�[���p�b�h�i�{�^���j
	enum ButtonKind
	{
		Button_A,
		Button_B,
		Button_X,
		Button_Y,
		Button_LB,
		Button_RB,
		Select,
		Start,
		Button_LS,
		Button_RS,
		Cross_Up,
		Cross_Down,
		Cross_Right,
		Cross_Left,
		ButtonMax
	};

	//�X�e�B�b�N
	enum StickKind
	{
		Up, Down, Right, Left
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

private: // �G�C���A�X
	// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public: // �����o�֐�
	static Input* GetInstance();
	/// <summary>
	/// ������
	/// </summary>
	/// <returns>����</returns>
	bool Initialize(WinApp*winApp);

	/// <summary>
	/// ���t���[������
	/// </summary>
	void Update();

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
	/// �L�[�̍��{�^���������`�F�b�N
	/// </summary>
	/// <returns>������Ă��邩</returns>
	bool PushMouseLeft();

	/// <summary>
	/// �L�[�̒��{�^���������`�F�b�N
	/// </summary>
	/// <returns>������Ă��邩</returns>
	bool PushMouseMiddle();

	/// <summary>
	/// �L�[�̍��{�^���g���K�[���`�F�b�N
	/// </summary>
	/// <returns>�g���K�[��</returns>
	bool TriggerMouseLeft();

	/// <summary>
	/// �L�[�̒��{�^���g���K�[���`�F�b�N
	/// </summary>
	/// <returns>�g���K�[��</returns>
	bool TriggerMouseMiddle();

	//�Q�[���p�b�h
	bool LeftTiltStick(int stick);
	bool LeftTriggerStick(int stick);
	bool PushButton(int Button);
	bool TriggerButton(int Button);
	bool PushCrossKey(int CrossKey);
	bool TriggerCrossKey(int CrossKey);
	bool ReleaseButton(int Button);

	bool AllNoPush();
public:
	const float& GetPosX() { return posX; }
	const float& GetPosY() { return posY; }
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
	POINT p;
	//�Q�[���p�b�h�f�o�C�X
	DWORD g_dxNumForoce;
	ComPtr<IDirectInputDevice8> devGamePad;
	LPDIRECTINPUTEFFECT  g_lpDIEffect = NULL;
	DIJOYSTATE gamePadState = {};
	DIJOYSTATE oldGamePadState = {};
	bool is_push[32] = {};
	//�X�e�B�b�N�̖������͈�
	LONG unresponsive_range = 650;
	float posX = 0;
	float posY = 0;
	WinApp* winApp;
};

