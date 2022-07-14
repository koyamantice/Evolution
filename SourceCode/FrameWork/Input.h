#pragma once

#include <Windows.h>
#include <wrl.h>
#include "WinApp.h"
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

/// <summary>
/// 入力
/// </summary>
class Input {
public:
	//ゲームパッド（ボタン）
	enum ButtonKind {
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

	//スティック
	enum StickKind {
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
	/// キーの左ボタン押下をチェック
	/// </summary>
	/// <returns>押されているか</returns>
	bool PushMouseLeft();

	/// <summary>
	/// キーの中ボタン押下をチェック
	/// </summary>
	/// <returns>押されているか</returns>
	bool PushMouseMiddle();

	/// <summary>
	/// キーの左ボタントリガーをチェック
	/// </summary>
	/// <returns>トリガーか</returns>
	bool TriggerMouseLeft();

	/// <summary>
	/// キーの中ボタントリガーをチェック
	/// </summary>
	/// <returns>トリガーか</returns>
	bool TriggerMouseMiddle();

	//ゲームパッド
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
	POINT p;
	//ゲームパッドデバイス
	DWORD g_dxNumForoce;
	ComPtr<IDirectInputDevice8> devGamePad;
	LPDIRECTINPUTEFFECT  g_lpDIEffect = NULL;
	DIJOYSTATE gamePadState = {};
	DIJOYSTATE oldGamePadState = {};
	bool is_push[32] = {};
	//スティックの無反応範囲
	LONG unresponsive_range = 650;
	float posX = 0;
	float posY = 0;
	WinApp* winApp;
};

