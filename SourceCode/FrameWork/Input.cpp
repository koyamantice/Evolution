#include "Input.h"
#include <cassert>
#pragma comment(lib, "dinput8.lib")

Input* Input::GetInstance() {
	static Input instance;
	return &instance;
}

bool Input::Initialize(WinApp* winApp) {
	HRESULT result = S_FALSE;
	this->winApp = winApp;
	// DirectInput�I�u�W�F�N�g�̐���	
	result = DirectInput8Create(winApp->GetHInstance(), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&dinput, nullptr);
	if (FAILED(result)) {
		assert(0);
		return result;
	}

	// �L�[�{�[�h�f�o�C�X�̐���	
	result = dinput->CreateDevice(GUID_SysKeyboard, &devKeyboard, NULL);
	if (FAILED(result)) {
		assert(0);
		return result;
	}

	// �}�E�X�f�o�C�X�̐���	
	result = dinput->CreateDevice(GUID_SysMouse, &devMouse, NULL);
	if (FAILED(result)) {
		assert(0);
		return result;
	}

	// ���̓f�[�^�`���̃Z�b�g
	result = devKeyboard->SetDataFormat(&c_dfDIKeyboard); // �W���`��
	if (FAILED(result)) {
		assert(0);
		return result;
	}

	// �r�����䃌�x���̃Z�b�g
	result = devKeyboard->SetCooperativeLevel(winApp->GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	if (FAILED(result)) {
		assert(0);
		return result;
	}

	// ���̓f�[�^�`���̃Z�b�g
	result = devMouse->SetDataFormat(&c_dfDIMouse2); // �W���`��
	if (FAILED(result)) {
		assert(0);
		return result;
	}

	// �r�����䃌�x���̃Z�b�g
	result = devMouse->SetCooperativeLevel(winApp->GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	if (FAILED(result)) {
		assert(0);
		return result;
	}

	//�Q�[���p�b�h�̐���
	devGamePad = nullptr;
	result = dinput->CreateDevice(GUID_Joystick, &devGamePad, NULL);

	if (devGamePad != nullptr)
	{
		//���̓f�[�^�`���̃Z�b�g
		result = devGamePad->SetDataFormat(&c_dfDIJoystick);

		// �����[�h���Βl���[�h�Ƃ��Đݒ�
		DIPROPDWORD diprop;
		ZeroMemory(&diprop, sizeof(diprop));
		diprop.diph.dwSize = sizeof(diprop);
		diprop.diph.dwHeaderSize = sizeof(diprop.diph);
		diprop.diph.dwHow = DIPH_DEVICE;
		diprop.diph.dwObj = 0;
		diprop.dwData = DIPROPAXISMODE_ABS;

		// �����[�h��ύX
		devGamePad->SetProperty(DIPROP_AXISMODE, &diprop.diph);

		// X���̒l�͈̔͐ݒ�
		DIPROPRANGE diprg;
		ZeroMemory(&diprg, sizeof(diprg));
		diprg.diph.dwSize = sizeof(diprg);
		diprg.diph.dwHeaderSize = sizeof(diprg.diph);
		diprg.diph.dwHow = DIPH_BYOFFSET;
		diprg.lMin = -1000;
		diprg.lMax = 1000;

		// X���̒l�͈̔͐ݒ�
		diprg.diph.dwObj = DIJOFS_X;
		devGamePad->SetProperty(DIPROP_RANGE, &diprg.diph);

		// Y���̒l�͈̔͐ݒ�
		diprg.diph.dwObj = DIJOFS_Y;
		devGamePad->SetProperty(DIPROP_RANGE, &diprg.diph);

		//�U���֌W
		DICONSTANTFORCE cf;
		DIEFFECT eff;
		DWORD rgdwAxes[2] = { DIJOFS_X , DIJOFS_Y };
		LONG  rglDirection[2] = { 1 , 1 };

		ZeroMemory(&eff, sizeof(eff));
		eff.dwSize = sizeof(DIEFFECT);
		eff.dwFlags = DIEFF_CARTESIAN | DIEFF_OBJECTOFFSETS;
		eff.dwDuration = INFINITE;
		eff.dwSamplePeriod = 0;
		eff.dwGain = DI_FFNOMINALMAX;
		eff.dwTriggerButton = DIEB_NOTRIGGER;
		eff.dwTriggerRepeatInterval = 0;
		eff.rgdwAxes = rgdwAxes;
		eff.rglDirection = rglDirection;
		eff.lpEnvelope = 0;
		eff.cbTypeSpecificParams = sizeof(DICONSTANTFORCE);
		eff.lpvTypeSpecificParams = &cf;
		eff.dwStartDelay = 0;
		
		devGamePad->CreateEffect(GUID_ConstantForce, &eff, &g_lpDIEffect, NULL);
		//�r�����䃌�x���Z�b�g
		result = devGamePad->SetCooperativeLevel(winApp->GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	}

	return true;
}

void Input::Update() {
	HRESULT result;

	{// �L�[�{�[�h
		result = devKeyboard->Acquire();	// �L�[�{�[�h����J�n

		// �O��̃L�[���͂�ۑ�
		memcpy(keyPre, key, sizeof(key));

		// �L�[�̓���
		result = devKeyboard->GetDeviceState(sizeof(key), key);
	}

	{// �}�E�X
		result = devMouse->Acquire();	// �}�E�X����J�n

		// �O��̓��͂�ۑ�
		mouseStatePre = mouseState;
		// �}�E�X���W���擾����
		GetCursorPos(&p);
		// �X�N���[�����W���N���C�A���g���W�ɕϊ�����
		ScreenToClient(winApp->GetHwnd(), &p);


		// �}�E�X�̓���
		result = devMouse->GetDeviceState(sizeof(mouseState), &mouseState);
	}

	if (devGamePad != nullptr)
	{
		//�Q�[���p�b�h���̎擾�J�n
		result = devGamePad->Acquire();
		//�O�t���[���̔���
		oldGamePadState = gamePadState;
		//�S�{�^���̏����擾����
		result = devGamePad->GetDeviceState(sizeof(DIJOYSTATE), &gamePadState);
		//���Z�b�g
		for (int i = 0; i < 32; i++)
		{
			is_push[i] = false;
		}
		posX = (float)gamePadState.lX;
		posY = (float)gamePadState.lY;
	}
}

bool Input::PushKey(BYTE keyNumber) {
	// �ُ�Ȉ��������o
	assert(0 <= keyNumber && keyNumber <= 256);

	// 0�łȂ���Ή����Ă���
	if (key[keyNumber]) {
		return true;
	}

	// �����Ă��Ȃ�
	return false;
}

bool Input::TriggerKey(BYTE keyNumber) {
	// �ُ�Ȉ��������o
	assert(0 <= keyNumber && keyNumber <= 256);

	// �O��0�ŁA����0�łȂ���΃g���K�[
	if (!keyPre[keyNumber] && key[keyNumber]) {
		return true;
	}

	// �g���K�[�łȂ�
	return false;
}

bool Input::ReleaseKey(BYTE keyNumber) {
	// �ُ�Ȉ��������o
	assert(0 <= keyNumber && keyNumber <= 256);

	// �O��1�ŁA����0�Ȃ烊���[�X
	if (keyPre[keyNumber] && !key[keyNumber]) {
		return true;
	}

	// �g���K�[�łȂ�
	return false;
}

bool Input::PushMouseLeft() {
	// 0�łȂ���Ή����Ă���
	if (mouseState.rgbButtons[0]) {
		return true;
	}

	// �����Ă��Ȃ�
	return false;
}

bool Input::PushMouseMiddle() {
	// 0�łȂ���Ή����Ă���
	if (mouseState.rgbButtons[2]) {
		return true;
	}

	// �����Ă��Ȃ�
	return false;
}

bool Input::TriggerMouseLeft() {
	// �O��0�ŁA����0�łȂ���΃g���K�[
	if (!mouseStatePre.rgbButtons[0] && mouseState.rgbButtons[0]) {
		return true;
	}

	// �g���K�[�łȂ�
	return false;
}

bool Input::TriggerMouseMiddle() {
	// �O��0�ŁA����0�łȂ���΃g���K�[
	if (!mouseStatePre.rgbButtons[2] && mouseState.rgbButtons[2]) {
		return true;
	}

	// �g���K�[�łȂ�
	return false;
}

Input::MouseMove Input::GetMouseMove() {
	MouseMove tmp{};
	tmp.lX = mouseState.lX;
	tmp.lY = mouseState.lY;
	tmp.lZ = mouseState.lZ;
	return tmp;
}

Input::MousePoint Input::GetPoint() {
	MousePoint tmp{};
	tmp.lX = p.x;
	tmp.lY = p.y;
	return tmp;
}

bool Input::LeftTiltStick(int stick)
{
	//��
	if (gamePadState.lX < -unresponsive_range && stick == Left)
	{
		return true;
	}
	//�E
	else if (gamePadState.lX > unresponsive_range && stick == Right)
	{
		return true;
	}
	//���
	if (gamePadState.lY > unresponsive_range && stick == Down)
	{
		return true;
	}
	//�O
	else if (gamePadState.lY < -unresponsive_range && stick == Up)
	{
		return true;
	}

	return false;
}

bool Input::LeftTriggerStick(int stick)
{
	//��
	if (gamePadState.lX < -unresponsive_range && !(oldGamePadState.lX < -unresponsive_range) && stick == Left)
	{
		return true;
	}
	//�E
	else if (gamePadState.lX > unresponsive_range && !(oldGamePadState.lX > unresponsive_range) && stick == Right)
	{
		return true;
	}
	//���
	if (gamePadState.lY > unresponsive_range && !(oldGamePadState.lY > unresponsive_range) && stick == Down)
	{
		return true;
	}
	//�O
	else if (gamePadState.lY < -unresponsive_range && !(oldGamePadState.lY < -unresponsive_range) && stick == Up)
	{
		return true;
	}

	return false;
}

bool Input::PushButton(int Button)
{
	for (int i = 0; i < 32; i++)
	{
		if (!(gamePadState.rgbButtons[i] & 0x80))
		{
			continue;
		}

		switch (i)
		{
		case 0:
			is_push[ButtonKind::Button_A] = true;
			break;
		case 1:
			is_push[ButtonKind::Button_B] = true;
			break;
		case 2:
			is_push[ButtonKind::Button_X] = true;
			break;
		case 3:
			is_push[ButtonKind::Button_Y] = true;
			break;
		case 4:
			is_push[ButtonKind::Button_LB] = true;
			break;
		case 5:
			is_push[ButtonKind::Button_RB] = true;
			break;
		case 6:
			is_push[ButtonKind::Select] = true;
			break;
		case 7:
			is_push[ButtonKind::Start] = true;
			break;
		case 8:
			is_push[ButtonKind::Button_LS] = true;
			break;
		case 9:
			is_push[ButtonKind::Button_RS] = true;
			break;
		default:
			break;
		}
	}

	for (int i = 0; i < Cross_Up; i++)
	{
		if (is_push[i] == true)
		{
			if (is_push[i] == is_push[Button])
			{
				return true;
			}
		}
	}

	return false;
}

bool Input::TriggerButton(int Button)
{
	for (int i = 0; i < 32; i++)
	{
		if (!(gamePadState.rgbButtons[i] & 0x80))
		{
			continue;
		}
		if (oldGamePadState.rgbButtons[i] & 0x80)
		{
			continue;
		}

		switch (i)
		{
		case 0:
			is_push[ButtonKind::Button_A] = true;
			break;
		case 1:
			is_push[ButtonKind::Button_B] = true;
			break;
		case 2:
			is_push[ButtonKind::Button_X] = true;
			break;
		case 3:
			is_push[ButtonKind::Button_Y] = true;
			break;
			is_push[ButtonKind::Button_Y] = true;
			break;
		case 4:
			is_push[ButtonKind::Button_LB] = true;
			break;
		case 5:
			is_push[ButtonKind::Button_RB] = true;
			break;
		case 6:
			is_push[ButtonKind::Select] = true;
			break;
		case 7:
			is_push[ButtonKind::Start] = true;
			break;
		case 8:
			is_push[ButtonKind::Button_LS] = true;
			break;
		case 9:
			is_push[ButtonKind::Button_RS] = true;
			break;
		default:
			break;
		}
	}

	for (int i = 0; i < Cross_Up; i++)
	{
		if (is_push[i] == true)
		{
			if (is_push[i] == is_push[Button])
			{
				return true;
			}
		}
	}

	return false;
}

bool Input::PushCrossKey(int CrossKey)
{
	if (gamePadState.rgdwPOV[0] != 0xFFFFFFFF)
	{
		switch (gamePadState.rgdwPOV[0])
		{
		case 0:
			is_push[ButtonKind::Cross_Up] = true;
			break;
		case 4500:
			is_push[ButtonKind::Cross_Up] = true;
			is_push[ButtonKind::Cross_Right] = true;
			break;
		case 9000:
			is_push[ButtonKind::Cross_Right] = true;
			break;
		case 13500:
			is_push[ButtonKind::Cross_Right] = true;
			is_push[ButtonKind::Cross_Down] = true;
			break;
		case 18000:
			is_push[ButtonKind::Cross_Down] = true;
			break;
		case 22500:
			is_push[ButtonKind::Cross_Down] = true;
			is_push[ButtonKind::Cross_Left] = true;
			break;
		case 27000:
			is_push[ButtonKind::Cross_Left] = true;
			break;
		case 31500:
			is_push[ButtonKind::Cross_Left] = true;
			is_push[ButtonKind::Cross_Up] = true;
			break;
		}

		for (int i = Cross_Up; i < ButtonMax; i++)
		{
			if (is_push[i] == true)
			{
				if (is_push[i] == is_push[CrossKey])
				{
					return true;
				}
			}
		}
	}


	return false;
}

bool Input::TriggerCrossKey(int CrossKey)
{
	if (gamePadState.rgdwPOV[0] != 0xFFFFFFFF && oldGamePadState.rgdwPOV[0] == 0xFFFFFFFF)
	{
		switch (gamePadState.rgdwPOV[0])
		{
		case 0:
			is_push[ButtonKind::Cross_Up] = true;
			break;
		case 4500:
			is_push[ButtonKind::Cross_Up] = true;
			is_push[ButtonKind::Cross_Right] = true;
			break;
		case 9000:
			is_push[ButtonKind::Cross_Right] = true;
			break;
		case 13500:
			is_push[ButtonKind::Cross_Right] = true;
			is_push[ButtonKind::Cross_Down] = true;
			break;
		case 18000:
			is_push[ButtonKind::Cross_Down] = true;
			break;
		case 22500:
			is_push[ButtonKind::Cross_Down] = true;
			is_push[ButtonKind::Cross_Left] = true;
			break;
		case 27000:
			is_push[ButtonKind::Cross_Left] = true;
			break;
		case 31500:
			is_push[ButtonKind::Cross_Left] = true;
			is_push[ButtonKind::Cross_Up] = true;
			break;
		}

		for (int i = Cross_Up; i < ButtonMax; i++)
		{
			if (is_push[i] == true)
			{
				if (is_push[i] == is_push[CrossKey])
				{
					return true;
				}
			}
		}
	}

	return false;
}

bool Input::ReleaseButton(int Button) {
	for (int i = 0; i < 32; i++) {
		if (!(gamePadState.rgbButtons[i] & 0x80)) {
			continue;
		}
		if (oldGamePadState.rgbButtons[i] & 0x80) {
			continue;
		}

		switch (i) {
		case 0:
			is_push[ButtonKind::Button_A] = true;
			break;
		case 1:
			is_push[ButtonKind::Button_B] = true;
			break;
		case 2:
			is_push[ButtonKind::Button_X] = true;
			break;
		case 3:
			is_push[ButtonKind::Button_Y] = true;
			break;
			is_push[ButtonKind::Button_Y] = true;
			break;
		case 4:
			is_push[ButtonKind::Button_LB] = true;
			break;
		case 5:
			is_push[ButtonKind::Button_RB] = true;
			break;
		case 6:
			is_push[ButtonKind::Select] = true;
			break;
		case 7:
			is_push[ButtonKind::Start] = true;
			break;
		case 8:
			is_push[ButtonKind::Button_LS] = true;
			break;
		case 9:
			is_push[ButtonKind::Button_RS] = true;
			break;
		default:
			break;
		}
	}

	for (int i = 0; i < Cross_Up; i++) {
		if (is_push[i] == true) {
			if (is_push[i] == is_push[Button]) {
				return false;
			}
		}
	}

	return false;
}

bool Input::AllNoPush() {
	for (int i = 0; i < 32; i++) {
		if (!(gamePadState.rgbButtons[i] & 0x80)) {
			continue;
		}

		switch (i) {
		case 0:
			is_push[ButtonKind::Button_A] = true;
			break;
		case 1:
			is_push[ButtonKind::Button_B] = true;
			break;
		case 2:
			is_push[ButtonKind::Button_X] = true;
			break;
		case 3:
			is_push[ButtonKind::Button_Y] = true;
			break;
		case 4:
			is_push[ButtonKind::Button_LB] = true;
			break;
		case 5:
			is_push[ButtonKind::Button_RB] = true;
			break;
		case 6:
			is_push[ButtonKind::Select] = true;
			break;
		case 7:
			is_push[ButtonKind::Start] = true;
			break;
		case 8:
			is_push[ButtonKind::Button_LS] = true;
			break;
		case 9:
			is_push[ButtonKind::Button_RS] = true;
			break;
		default:
			break;
		}
	}

	for (int i = 0; i < Cross_Up; i++) {
		if (is_push[i] == true) {
			if (is_push[i] == is_push[i]) {
				return false;
			}
		}
	}
	for (int i = 0; i < 4; i++) {
		if (LeftTiltStick(i)) {
			return false;
			//break;
		}
	}
	return true;
}


