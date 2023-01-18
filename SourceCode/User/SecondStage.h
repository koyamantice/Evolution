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
	/// <param name="Timer"></param>
	void IntroCamera(const float& Timer);

	/// <summary>
	/// クリア後カメラ
	/// </summary>
	/// <param name="Timer"></param>
	void ResultCamera(const float& Timer);

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
	void GameOverUpdate()override;

	float RandHeight(const float& base);


	float intro_count = 0;
	const float intro_count_max = 1200;
	float intro_speed = 1;
	float intro_speed_max = 2;

	const float first_hight = 10;
	const XMFLOAT3 first_target = {0,5,0};

	std::unique_ptr<Sprite> Clear{};
	std::unique_ptr<Sprite> Over{};

	
	PostEffect* postEffect = nullptr;
};

