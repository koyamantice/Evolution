#pragma once
#include "SceneManager.h"

#include "Actor.h"
#include "ActorManager.h"
#include "PauseUI.h"

#include <Touch.h>
#include <PostEffect.h>


class BattleScene : public BaseScene {
public:
	virtual ~BattleScene() = default;

protected:
	/// <summary>
	/// バトルステージ専用の初期化処理
	/// </summary>
	void BattleInit();

	/// <summary>
	/// フィールドの更新処理
	/// </summary>
	void FieldUpdate();

	/// <summary>
	/// 松明のcsv読み込み
	/// </summary>
	/// <param name="color">カラー</param>
	void TorchSetup(int color=0);

	/// <summary>
	///	カメラ周り
	/// </summary>
	void CameraUpda();

	/// <summary>
	/// カメラをを背中に持ってきます
	/// </summary>
	void ResetCamera();

	/// <summary>
	/// 説明表示用の更新
	/// </summary>
	virtual void DescriptionUpdate() {};

	/// <summary>
	///	導入部分の更新処理
	/// </summary>
	/// <returns></returns>
	virtual bool IntroUpdate() { return false; };

	/// <summary>
	/// ポーズUIの更新処理
	/// </summary>
	/// <returns></returns>
	bool PauseUpdate();

	/// <summary>
	/// ゲームオーバーの更新処理
	/// </summary>
	virtual void GameOverUpdate() {};

	/// <summary>
	/// クリア更新処理
	/// </summary>
	virtual bool ClearUpdate() { return false; };
protected:
	Input* input = Input::GetInstance();

	Actor* player_shadow = nullptr;
	Actor* enemy_shadow = nullptr;
	Actor* goal_shadow = nullptr;

	std::unique_ptr<PauseUI> pauseUi{};
	std::unique_ptr<Sprite>	screens[2]{};

	std::unique_ptr<Sprite> filter_first{};
	float filter_alpha = 0.0f;

	std::unique_ptr<Object3d> skydome{};
	std::unique_ptr<Object3d> ground{};

	std::stringstream torch_pop;
	std::list<std::unique_ptr<Touch>> torchs;


	//killクリップ
	int finishTime = 0;


	bool battle_intro = true;

	float feedin_frame = 0;
	const float feedin_frame_max = 20;

	bool battle_result = false;
	bool gameover = false;

	bool stage_clear = false;
	float clear_frame = 0.0f;

	//カメラ関係
	bool reset_camera = false;
	float angle_frame = 0.0f;
	const float angle_frame_max = 60.0f;

	bool camera_explanation = false;

	const float camera_hight = 18.0f;
	const float camera_radius = 15.0f;

	float camera_angle = 0.0f;
	const float camera_target = 7.0f;
	const float camera_vel = 3.0f;
	float s_camera_angle = 0.0f;
	float e_camera_angle = 0.0f;

	XMFLOAT3 camera_distance = {};
	XMFLOAT3 s_camera_distance = {};
	XMFLOAT3 e_camera_distance = {};
};