#pragma once
#include "SceneManager.h"

#include "Actor.h"
#include "ActorManager.h"
#include "PauseUI.h"

#include <Touch.h>
#include <PostEffect.h>
#include <Hud.h>


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
	virtual bool IntroUpdate() { return false; }

	/// <summary>
	/// ポーズUIの更新処理
	/// </summary>
	/// <returns></returns>
	bool PauseUpdate();

	/// <summary>
	/// ゲームオーバーの更新処理
	/// </summary>
	virtual bool GameOverUpdate() { return false; }

	/// <summary>
	/// クリア更新処理
	/// </summary>
	virtual bool ClearUpdate() { return false; }
	/// <summary>
	/// 
	/// </summary>
	/// <param name="base"></param>
	/// <returns></returns>
	float RandHeight(const float& base);
	/// <summary>
	/// バトルステージ専用の描画処理(前)
	/// </summary>
	void BattleBackDraw();
	/// <summary>
	/// バトルステージ専用の描画処理(後ろ)
	/// </summary>
	void BattleFrontDraw(blendType _blendType = alphaBle, Sprite* _introwo = nullptr);

	virtual void DrawLocal() {};

protected:
	Input* input = Input::GetInstance();

	Actor* player_shadow = nullptr;
	Actor* enemy_shadow = nullptr;
	Actor* goal_shadow = nullptr;
	//
	const int kGnormNum = 10;
	//ポーズUI
	std::unique_ptr<PauseUI> pauseUi{};
	//クリア文字
	std::unique_ptr<Sprite> clear_comment_{};
	//ゲームオーバー文字
	std::unique_ptr<Sprite> over_comment_{};

	//導入部分のスクリーン
	std::unique_ptr<Sprite>	screens[2]{};
	//スキップUI
	std::unique_ptr<Sprite>	skip_ui_{};

	//暗闇からフェードします
	std::unique_ptr<Sprite> filter_first{};
	float filter_alpha = 0.0f;
	float feedin_frame = 0;
	const float feedin_frame_max = 20;

	//ステージ関連
	std::unique_ptr<Object3d> skydome{};
	std::unique_ptr<Object3d> ground{};
	std::stringstream torch_pop;
	std::list<std::unique_ptr<Touch>> torchs;

	//コントローラーの説明
	std::unique_ptr<Hud> hud=nullptr;

	//killクリップ
	int finish_time = 0;
	const int finish_time_Max = 220;
	
	bool battle_intro = true;
	bool battle_result = false;

	//クリア・ゲームオーバー
	bool stage_clear = false;
	float clear_frame = 0.0f;

	bool gameover = false;

	//カメラ関係
	bool reset_camera = false;
	float angle_frame = 0.0f;
	const float angle_frame_max = 30.0f;

	bool camera_explanation = false;

	const float kCameraHight = 18.0f;
	float camera_hight = kCameraHight;
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