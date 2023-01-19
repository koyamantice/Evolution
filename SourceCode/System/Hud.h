#pragma once
#include"Actor.h"
#include <Sprite.h>
#include"Input.h"

class Hud {
private: // エイリアス
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	//入力関連
	Input* input = Input::GetInstance();
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Hud();
	
	/// <summary>
	///	デストラクタ
	/// </summary>
	~Hud();

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	///描画処理
	/// </summary>
	void Draw();
private:
	/// <summary>
	////初期化処理
	/// </summary>
	void Initialize();

	/// <summary>
	/// 終了処理
	/// </summary>
	void Finalize();

	/// <summary>
	///コントローラー説明の更新
	/// </summary>
	void CameraHudUpdate();

	/// <summary>
	///ボタンの操作説明の更新
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

	//コントローラーの説明
	std::unique_ptr<Sprite> camera_hud[CAMERACONTROLMAX]{};
	const XMFLOAT2 kCameraHudBase = { 640,530 };
	const XMFLOAT2 kVisibleBasicSize = { 128,128 };
	int visible_trigger = TRIGGERNORMAL;
	int visible_stick = STICKLEFT;
	int old_visible_stick = visible_stick;

	//アニメーション切り替え用
	int switch_frame = 0;
	const int kSwitchFrameMax = 30;

	//説明が終わると端まで寄せる
	bool explation_finish = false;
	const XMFLOAT2 kCameraHudPos2Edge = { 90,680 };
	const XMFLOAT2 kCameraHudSize2Edge = { 64,64 };
	float edge_frame = 0.0f;
	const float kEdgeFrameMax = 200.0f;

	//ボタンの説明
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
