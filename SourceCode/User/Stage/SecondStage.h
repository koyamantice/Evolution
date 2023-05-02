#pragma once
#include "SceneManager.h"


/// <summary>
/// ボス：蜂　第二ステージ
/// </summary>
class SecondStage : public  BattleScene {
private:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(DirectXCommon* dxCommon) override;
	/// <summary>
	/// 毎フレーム更新
	/// </summary>
	void Update(DirectXCommon* dxCommon) override;
	/// <summary>
	/// 描画
	/// </summary>
	void Draw(DirectXCommon* dxCommon) override;
	/// <summary>
	/// /// 終了
	/// </summary>
	void Finalize() override;
private:
	/// <summary>
	/// 導入カメラ
	/// </summary>
	/// <param name="timer"></param>
	void IntroCamera(const float timer);

	/// <summary>
	/// クリア後カメラ
	/// </summary>
	/// <param name="timer"></param>
	void ResultCamera(const float timer);

	/// <summary>
	/// 撃破後カメラ
	/// </summary>
	/// <param name="timer"></param>
	void SmashCamera(const float timer) override;

	/// <summary>
	///	導入部分の更新処理
	/// </summary>
	/// <returns></returns>
	bool IntroUpdate()override;

	/// <summary>
	/// クリア更新処理
	/// </summary>
	bool ClearUpdate() override;

	/// <summary>
	/// ゲームオーバーの更新処理
	/// </summary>
	bool GameOverUpdate()override;



	float intro_count = 0;
	const float intro_count_max = 600;
	float intro_speed = 1;
	float intro_speed_max = 2;

	const float first_hight = 10;
	const XMFLOAT3 first_target = {0,5,0};

	
	PostEffect* postEffect = nullptr;
};

