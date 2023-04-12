#include "Input.h"
#include <cassert>
#pragma comment(lib, "dinput8.lib")

#define STICK_MAX 32768.0f


Input* Input::GetInstance() {
	static Input instance;
	return &instance;
}

bool Input::Initialize(WinApp* winApp_) {
	HRESULT result = S_FALSE;
	this->winApp_ = winApp_;
	// DirectInputオブジェクトの生成	
	result = DirectInput8Create(winApp_->GetHInstance(), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&dinput_, nullptr);
	if (FAILED(result)) {
		assert(0);
		return result;
	}

	// キーボードデバイスの生成	
	result = dinput_->CreateDevice(GUID_SysKeyboard, &devKeyboard_, NULL);
	if (FAILED(result)) {
		assert(0);
		return result;
	}

	// マウスデバイスの生成	
	result = dinput_->CreateDevice(GUID_SysMouse, &devMouse_, NULL);
	if (FAILED(result)) {
		assert(0);
		return result;
	}

	// 入力データ形式のセット
	result = devKeyboard_->SetDataFormat(&c_dfDIKeyboard); // 標準形式
	if (FAILED(result)) {
		assert(0);
		return result;
	}

	// 排他制御レベルのセット
	result = devKeyboard_->SetCooperativeLevel(winApp_->GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	if (FAILED(result)) {
		assert(0);
		return result;
	}

	// 入力データ形式のセット
	result = devMouse_->SetDataFormat(&c_dfDIMouse2); // 標準形式
	if (FAILED(result)) {
		assert(0);
		return result;
	}

	// 排他制御レベルのセット
	result = devMouse_->SetCooperativeLevel(winApp_->GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	if (FAILED(result)) {
		assert(0);
		return result;
	}


	return true;
}

void Input::Update() {
	HRESULT result;

	{// キーボード
		result = devKeyboard_->Acquire();	// キーボード動作開始

		// 前回のキー入力を保存
		memcpy(keyPre_, key_, sizeof(key_));

		// キーの入力
		result = devKeyboard_->GetDeviceState(sizeof(key_), key_);
	}

	{// マウス
		result = devMouse_->Acquire();	// マウス動作開始

		// 前回の入力を保存
		mouseStatePre_ = mouseState_;
		// マウス座標を取得する
		GetCursorPos(&mousePoint_);
		// スクリーン座標をクライアント座標に変換する
		ScreenToClient(winApp_->GetHwnd(), &mousePoint_);


		// マウスの入力
		result = devMouse_->GetDeviceState(sizeof(mouseState_), &mouseState_);
	}

	//コントローラー
	xinputStatePre_ = xinputState_;
//	ZeroMemory(&xinputState_, sizeof(XINPUT_STATE));

	DWORD dwResult = XInputGetState(0, &xinputState_);
	if (dwResult == ERROR_SUCCESS) {
		left_stick_posX_ = (float)xinputState_.Gamepad.sThumbLX;
		left_stick_posY_ = (float)xinputState_.Gamepad.sThumbLY;

		//コントローラーが接続されている
		if (0 < shakeTimer_) {
			//shakeTimer_--;
			//XINPUT_VIBRATION vibration;
			//ZeroMemory(&vibration, sizeof(XINPUT_VIBRATION));

			//if (shakeTimer_ == 0) {
			//	vibration.wLeftMotorSpeed = (WORD) 0.0f; // use any value between 0-65535 here
			//	vibration.wRightMotorSpeed =(WORD) 0.0f; // use any value between 0-65535 here
			//} else {
			//	vibration.wLeftMotorSpeed =  (WORD)(65535.0f * shakePower_); // use any value between 0-65535 here
			//	vibration.wRightMotorSpeed = (WORD)(65535.0f * shakePower_); // use any value between 0-65535 here
			//}
			//XInputSetState(dwResult, &vibration);
		}
	} else {
		//コントローラーが接続されていない
	}
}

void Input::Finalize() {
	dinput_.Reset();
	devKeyboard_.Reset();
	devMouse_.Reset();
}

bool Input::PushKey(BYTE keyNumber) {
	// 異常な引数を検出
	assert(0 <= keyNumber && keyNumber <= 256);
	// 0でなければ押している
	return key_[keyNumber];
}

bool Input::TriggerKey(BYTE keyNumber) {
	// 異常な引数を検出
	assert(0 <= keyNumber && keyNumber <= 256);
	// 前回が0で、今回が0でなければトリガー
	return !keyPre_[keyNumber] && key_[keyNumber];
}

bool Input::ReleaseKey(BYTE keyNumber) {
	// 異常な引数を検出
	assert(0 <= keyNumber && keyNumber <= 256);
	// 前回が1で、今回が0ならリリース
	return keyPre_[keyNumber] && !key_[keyNumber];
}

bool Input::PushMouse(MouseButton Button) {
	int key_ = Button;
	// 0でなければ押している
	return mouseState_.rgbButtons[key_];
}

bool Input::TriggerMouse(MouseButton Button) {
	int key_ = Button;
	// 前回が0で、今回が0でなければトリガー
	return !mouseStatePre_.rgbButtons[key_] && mouseState_.rgbButtons[key_];
}

Input::MouseMove Input::GetMouseMove() {
	MouseMove tmp{};
	tmp.lX = mouseState_.lX;
	tmp.lY = mouseState_.lY;
	tmp.lZ = mouseState_.lZ;
	return tmp;
}

Input::MousePoint Input::GetPoint() {
	MousePoint tmp{};
	tmp.lX = mousePoint_.x;
	tmp.lY = mousePoint_.y;
	return tmp;
}

bool Input::TiltStick(STICK Stick) {
	StickPos oldVec;
	StickPos vec;
	//右か左か
	bool isLeftStick = Stick <= L_RIGHT;
	if (isLeftStick) {
		oldVec.x = xinputStatePre_.Gamepad.sThumbLX;
		oldVec.y = xinputStatePre_.Gamepad.sThumbLY;
		vec.x = xinputState_.Gamepad.sThumbLX;
		vec.y = xinputState_.Gamepad.sThumbLY;
	} else {
		oldVec.x = xinputStatePre_.Gamepad.sThumbRX;
		oldVec.y = xinputStatePre_.Gamepad.sThumbRY;
		vec.x = xinputState_.Gamepad.sThumbRX;
		vec.y = xinputState_.Gamepad.sThumbRY;
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
		vec.x = xinputState_.Gamepad.sThumbLX;
		vec.y = xinputState_.Gamepad.sThumbLY;
	} else {
		vec.x = xinputState_.Gamepad.sThumbRX;
		vec.y = xinputState_.Gamepad.sThumbRY;
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
		return XINPUT_GAMEPAD_TRIGGER_THRESHOLD < xinputState_.Gamepad.bLeftTrigger;
	} else if (Button == RT) {
		return XINPUT_GAMEPAD_TRIGGER_THRESHOLD < xinputState_.Gamepad.bRightTrigger;
	} else {
		return xinputState_.Gamepad.wButtons & Button;
	}
	assert(0);
	return false;
}

bool Input::TriggerButton(XBOX Button) {
	//トリガー
	if (Button == LT) {
		return xinputStatePre_.Gamepad.bLeftTrigger <= XINPUT_GAMEPAD_TRIGGER_THRESHOLD
		&& CheckTrigger(Button);
	} else if (Button == RT) {
		return xinputStatePre_.Gamepad.bRightTrigger <= XINPUT_GAMEPAD_TRIGGER_THRESHOLD
		&& CheckTrigger(Button);
	} else {
		return !(xinputStatePre_.Gamepad.wButtons & Button)
		&& CheckTrigger(Button);
	}
	assert(0);
	return false;
}

bool Input::CheckTrigger(XBOX Button) {

	if (Button == LT) {
		return XINPUT_GAMEPAD_TRIGGER_THRESHOLD < xinputState_.Gamepad.bLeftTrigger;
	} else if (Button == RT) {
		return XINPUT_GAMEPAD_TRIGGER_THRESHOLD < xinputState_.Gamepad.bRightTrigger;
	} else {
		return xinputState_.Gamepad.wButtons & Button;
	}
	assert(0);
	return false;

}

bool Input::StickInDeadZone(StickPos& Thumb) {
	bool x,y = false;
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
