#include "Input.h"
#include <cassert>
#pragma comment(lib, "dinput8.lib")

#define STICK_MAX 32768.0f


Input* Input::GetInstance() {
	static Input instance;
	return &instance;
}

bool Input::Initialize(WinApp* winApp) {
	HRESULT result = S_FALSE;
	this->winApp = winApp;
	// DirectInputオブジェクトの生成	
	result = DirectInput8Create(winApp->GetHInstance(), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&dinput, nullptr);
	if (FAILED(result)) {
		assert(0);
		return result;
	}

	// キーボードデバイスの生成	
	result = dinput->CreateDevice(GUID_SysKeyboard, &devKeyboard, NULL);
	if (FAILED(result)) {
		assert(0);
		return result;
	}

	// マウスデバイスの生成	
	result = dinput->CreateDevice(GUID_SysMouse, &devMouse, NULL);
	if (FAILED(result)) {
		assert(0);
		return result;
	}

	// 入力データ形式のセット
	result = devKeyboard->SetDataFormat(&c_dfDIKeyboard); // 標準形式
	if (FAILED(result)) {
		assert(0);
		return result;
	}

	// 排他制御レベルのセット
	result = devKeyboard->SetCooperativeLevel(winApp->GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	if (FAILED(result)) {
		assert(0);
		return result;
	}

	// 入力データ形式のセット
	result = devMouse->SetDataFormat(&c_dfDIMouse2); // 標準形式
	if (FAILED(result)) {
		assert(0);
		return result;
	}

	// 排他制御レベルのセット
	result = devMouse->SetCooperativeLevel(winApp->GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	if (FAILED(result)) {
		assert(0);
		return result;
	}


	return true;
}

void Input::Update() {
	HRESULT result;

	{// キーボード
		result = devKeyboard->Acquire();	// キーボード動作開始

		// 前回のキー入力を保存
		memcpy(keyPre, key, sizeof(key));

		// キーの入力
		result = devKeyboard->GetDeviceState(sizeof(key), key);
	}

	{// マウス
		result = devMouse->Acquire();	// マウス動作開始

		// 前回の入力を保存
		mouseStatePre = mouseState;
		// マウス座標を取得する
		GetCursorPos(&mousePoint);
		// スクリーン座標をクライアント座標に変換する
		ScreenToClient(winApp->GetHwnd(), &mousePoint);


		// マウスの入力
		result = devMouse->GetDeviceState(sizeof(mouseState), &mouseState);
	}

	//コントローラー
	oldXinputState = xinputState;
//	ZeroMemory(&xinputState, sizeof(XINPUT_STATE));

	DWORD dwResult = XInputGetState(0, &xinputState);
	if (dwResult == ERROR_SUCCESS) {
		LeftControllerX = (float)xinputState.Gamepad.sThumbLX;
		LeftControllerY = (float)xinputState.Gamepad.sThumbLY;

		//コントローラーが接続されている
		if (0 < shakeTimer) {
			//shakeTimer--;
			//XINPUT_VIBRATION vibration;
			//ZeroMemory(&vibration, sizeof(XINPUT_VIBRATION));

			//if (shakeTimer == 0) {
			//	vibration.wLeftMotorSpeed = (WORD) 0.0f; // use any value between 0-65535 here
			//	vibration.wRightMotorSpeed =(WORD) 0.0f; // use any value between 0-65535 here
			//} else {
			//	vibration.wLeftMotorSpeed =  (WORD)(65535.0f * shakePower); // use any value between 0-65535 here
			//	vibration.wRightMotorSpeed = (WORD)(65535.0f * shakePower); // use any value between 0-65535 here
			//}
			//XInputSetState(dwResult, &vibration);
		}
	} else {
		//コントローラーが接続されていない
	}
}

void Input::Finalize() {
	dinput.Reset();
	devKeyboard.Reset();
	devMouse.Reset();
}

bool Input::PushKey(BYTE keyNumber) {
	// 異常な引数を検出
	assert(0 <= keyNumber && keyNumber <= 256);
	// 0でなければ押している
	return key[keyNumber];
}

bool Input::TriggerKey(BYTE keyNumber) {
	// 異常な引数を検出
	assert(0 <= keyNumber && keyNumber <= 256);
	// 前回が0で、今回が0でなければトリガー
	return !keyPre[keyNumber] && key[keyNumber];
}

bool Input::ReleaseKey(BYTE keyNumber) {
	// 異常な引数を検出
	assert(0 <= keyNumber && keyNumber <= 256);
	// 前回が1で、今回が0ならリリース
	return keyPre[keyNumber] && !key[keyNumber];
}

bool Input::PushMouse(MouseButton Button) {
	int key = Button;
	// 0でなければ押している
	return mouseState.rgbButtons[key];
}

bool Input::TriggerMouse(MouseButton Button) {
	int key = Button;
	// 前回が0で、今回が0でなければトリガー
	return !mouseStatePre.rgbButtons[key] && mouseState.rgbButtons[key];
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
	tmp.lX = mousePoint.x;
	tmp.lY = mousePoint.y;
	return tmp;
}

bool Input::TiltStick(STICK Stick) {
	StickPos oldVec;
	StickPos vec;
	//右か左か
	bool isLeftStick = Stick <= L_RIGHT;
	if (isLeftStick) {
		oldVec.x = oldXinputState.Gamepad.sThumbLX;
		oldVec.y = oldXinputState.Gamepad.sThumbLY;
		vec.x = xinputState.Gamepad.sThumbLX;
		vec.y = xinputState.Gamepad.sThumbLY;
	} else {
		oldVec.x = oldXinputState.Gamepad.sThumbRX;
		oldVec.y = oldXinputState.Gamepad.sThumbRY;
		vec.x = xinputState.Gamepad.sThumbRX;
		vec.y = xinputState.Gamepad.sThumbRY;
	}
	if (!StickInDeadZone(oldVec))return false;
	if (StickInDeadZone(vec))return false;

	bool result = false;

	if (Stick % 4 == L_UP) {
		result =  0.3f < (vec.y / STICK_MAX);
	} else if (Stick % 4 == L_DOWN) {
		result = vec.y / STICK_MAX < -0.3f;
	} else if (Stick % 4 == L_RIGHT) {
		result = 0.3f < (vec.x / STICK_MAX);
	} else if (Stick % 4 == L_LEFT) {
		result =  vec.x / STICK_MAX < -0.3f;
	} else {
		assert(0);
	}

	return result;
}

bool Input::TiltPushStick(STICK Stick, float DeadZone) {
	StickPos vec{};
	//右か左か
	bool isLeftStick = Stick <= L_RIGHT;
	if (isLeftStick) {
		vec.x = xinputState.Gamepad.sThumbLX;
		vec.y = xinputState.Gamepad.sThumbLY;
	} else {
		vec.x = xinputState.Gamepad.sThumbRX;
		vec.y = xinputState.Gamepad.sThumbRY;
	}

	bool result = false;
	if (Stick % 4 == L_UP) {
		result = DeadZone < (vec.y / STICK_MAX);
	} else if (Stick % 4 == L_DOWN) {
		result = vec.y / STICK_MAX < -DeadZone;
	} else if (Stick % 4 == L_RIGHT) {
		result = DeadZone < (vec.x / STICK_MAX);
	} else if (Stick % 4 == L_LEFT) {
		result = vec.x / STICK_MAX < -DeadZone;
	} else {
		assert(0);
	}

	return result;
}

bool Input::PushButton(XBOX Button) {
	if (Button == LT) {
		return XINPUT_GAMEPAD_TRIGGER_THRESHOLD < xinputState.Gamepad.bLeftTrigger;
	} else if (Button == RT) {
		return XINPUT_GAMEPAD_TRIGGER_THRESHOLD < xinputState.Gamepad.bRightTrigger;
	} else {
		return xinputState.Gamepad.wButtons & Button;
	}
	assert(0);
	return false;
}

bool Input::TriggerButton(XBOX Button) {
	//トリガー
	if (Button == LT) {
		return oldXinputState.Gamepad.bLeftTrigger <= XINPUT_GAMEPAD_TRIGGER_THRESHOLD
		&& CheckTrigger(Button);
	} else if (Button == RT) {
		return oldXinputState.Gamepad.bRightTrigger <= XINPUT_GAMEPAD_TRIGGER_THRESHOLD
		&& CheckTrigger(Button);
	} else {
		return !(oldXinputState.Gamepad.wButtons & Button)
		&& CheckTrigger(Button);
	}
	assert(0);
	return false;
}

bool Input::CheckTrigger(XBOX Button) {

	if (Button == LT) {
		return XINPUT_GAMEPAD_TRIGGER_THRESHOLD < xinputState.Gamepad.bLeftTrigger;
	} else if (Button == RT) {
		return XINPUT_GAMEPAD_TRIGGER_THRESHOLD < xinputState.Gamepad.bRightTrigger;
	} else {
		return xinputState.Gamepad.wButtons & Button;
	}
	assert(0);
	return false;

}

bool Input::StickInDeadZone(StickPos& Thumb) {
	bool x = false, y = false;
	if ((Thumb.x < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE * 1.0f
		&& Thumb.x > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE * 1.0f)) {
		Thumb.x = 0.0f;
		x = true;
	}
	if ((Thumb.y < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE * 1.0f
		&& Thumb.y > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE * 1.0f)) {
		Thumb.y = 0.0f;
		y = true;
	}
	if (x && y)return true;
	return false;
}
