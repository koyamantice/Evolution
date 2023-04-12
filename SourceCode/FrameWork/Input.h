#pragma once
#include "WinApp.h"

#define DIRECTINPUT_VERSION 0x0800

#include <Windows.h>
#include <wrl.h>
#include <dinput.h>
#include <Xinput.h>

/// <summary>
/// 入力
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
	//スティック
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
private: // エイリアス
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public: // メンバ関数
	static Input* GetInstance();
	/// <summary>
	/// 初期化
	/// </summary>
	/// <returns>成否</returns>
	bool Initialize(WinApp* winApp);

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	///終了
	/// </summary>
	void Finalize();
	/// <summary>
	/// キーの押下をチェック
	/// </summary>
	/// <param name="keyNumber">キー番号( DIK_0 等)</param>
	/// <returns>押されているか</returns>
	bool PushKey(BYTE keyNumber);

	/// <summary>
	/// キーのトリガーをチェック
	/// </summary>
	/// <param name="keyNumber">キー番号( DIK_0 等)</param>
	/// <returns>トリガーか</returns>
	bool TriggerKey(BYTE keyNumber);
	/// <summary>
	/// キーのリリースをチェック
	/// </summary>
	/// <param name="keyNumber">キー番号( DIK_0 等)</param>
	/// <returns>リリースか</returns>
	bool ReleaseKey(BYTE keyNumber);

	/// <summary>
	/// キーの中ボタン押下をチェック
	/// </summary>
	/// <returns>押されているか</returns>
	bool PushMouse(MouseButton Button);

	/// <summary>
	/// キーの左ボタントリガーをチェック
	/// </summary>
	/// <returns>トリガーか</returns>
	bool TriggerMouse(MouseButton Button);

	//ボタン配置
	bool TriggerButton(XBOX Button);
	bool PushButton(XBOX Button);
	//トリガー
	bool TiltStick(STICK Stick);
	//プッシュ
	bool TiltPushStick(STICK Stick,float DeadZone=0.3f);

public:
	const float& GetPosX() { return mouse_posX_; }
	const float& GetPosY() { return mouse_posY_; }
	const float& GetLeftControllerX() { return left_stick_posX_; }
	const float& GetLeftControllerY() { return left_stick_posY_; }
	/// <summary>
	/// マウス移動量を取得
	/// </summary>
	/// <returns>マウス移動量</returns>
	MouseMove GetMouseMove();
	MousePoint GetPoint();


private: // メンバ変数
	ComPtr<IDirectInput8> dinput_;
	ComPtr<IDirectInputDevice8> devKeyboard_;
	BYTE key_[256] = {};
	BYTE keyPre_[256] = {};
	ComPtr<IDirectInputDevice8> devMouse_;
	DIMOUSESTATE2 mouseState_ = {};
	DIMOUSESTATE2 mouseStatePre_ = {};
	POINT mousePoint_;

	//XINPUT(コントローラー用)
	XINPUT_STATE xinputState_;
	XINPUT_STATE xinputStatePre_;

	float shakePower_ = 0.0f;
	int shakeTimer_ = 0;

	bool CheckTrigger(XBOX Button);

	//デッドゾーンに入っているか(DeadRate : デッドゾーン判定の度合い、1.0fだとデフォルト)
	bool StickInDeadZone(StickPos& Thumb);

	float mouse_posX_ = 0;
	float mouse_posY_ = 0;
	float left_stick_posX_ = 0;
	float left_stick_posY_ = 0;

	WinApp* winApp_;
};

