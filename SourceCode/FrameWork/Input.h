#pragma once

#include <Windows.h>
#include <wrl.h>
#include "WinApp.h"
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>


#include <Xinput.h>

/// <summary>
/// 入力
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
	//スティック
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
	const float& GetPosX() { return posX; }
	const float& GetPosY() { return posY; }
	const float& GetRPosX() { return (float)xinputState.Gamepad.sThumbLX; }
	const float& GetRPosY() { return (float)xinputState.Gamepad.sThumbLY; }
	/// <summary>
	/// マウス移動量を取得
	/// </summary>
	/// <returns>マウス移動量</returns>
	MouseMove GetMouseMove();
	MousePoint GetPoint();


private: // メンバ変数
	ComPtr<IDirectInput8> dinput;
	ComPtr<IDirectInputDevice8> devKeyboard;
	BYTE key[256] = {};
	BYTE keyPre[256] = {};
	ComPtr<IDirectInputDevice8> devMouse;
	DIMOUSESTATE2 mouseState = {};
	DIMOUSESTATE2 mouseStatePre = {};
	POINT mousePoint;

	//XINPUT(コントローラー用)
	XINPUT_STATE xinputState;
	XINPUT_STATE oldXinputState;
	float shakePower = 0.0f;
	int shakeTimer = 0;
	bool CheckTrigger(XBOX Button);
	bool CheckTilt(STICK Stick);

	//デッドゾーンに入っているか(DeadRate : デッドゾーン判定の度合い、1.0fだとデフォルト)
	bool StickInDeadZone(StickPos& Thumb);


	float posX = 0;
	float posY = 0;
	float RposX = 0;
	float RposY = 0;

	WinApp* winApp;
};

