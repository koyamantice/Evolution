#pragma once
#include "SceneManager.h"


/// <summary>
/// ボス：キノコ　第一ステージ
/// </summary>
class FirstStage : public BattleScene {
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


	bool DebugDraw();
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
	/// 撃破後カメラ
	/// </summary>
	/// <param name="Timer"></param>
	void SmashCamera(const float& Timer) override;



	/// <summary>
	///	導入部分の更新処理
	/// </summary>
	/// <returns></returns>
	bool IntroUpdate()override;

	/// <summary>
	/// 説明表示用の更新
	/// </summary>
	void DescriptionUpdate() override;

	/// <summary>
	/// クリア更新処理
	/// </summary>
	bool ClearUpdate() override;

	/// <summary>
	/// ゲームオーバーの更新処理
	/// </summary>
	bool GameOverUpdate()override;

	float intro_count = 0;
	const float intro_count_max = 800;

	XMFLOAT3 pointLightPos[3] = { {},{},{} };
	XMFLOAT3 dirLightLight[3] = { {1,0,0},{0,1,0},{0,0,1} };

	float dirLightColor[3] = { 1,1,1 };
	float dirLightAtten[3] = { 0.3f,0.1f,0.1f };

	float intro_speed = 1;
	float intro_speed_max = 2;

	const float first_hight = 50.0f;
	//導入部分変数
	static const int intro_word_max = 6;
	int nowWord = 0;
	std::unique_ptr<Sprite> IntroWord[intro_word_max];
private:

	PostEffect* postEffect = nullptr;
};

