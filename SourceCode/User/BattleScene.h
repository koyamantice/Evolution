#pragma once
#include "SceneManager.h"
#include <Touch.h>
#include <string>
#include <fstream>
#include <iostream>



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
	/// 説明表示用の更新
	/// </summary>
	virtual void DescriptionUpdate() {};
protected:

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

	bool Intro = true;
	bool Result = false;
	bool GameOver = false;
};