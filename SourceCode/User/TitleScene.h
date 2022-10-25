#pragma once
#include "SceneManager.h"
#include "Actor.h"
#include"TitleText.h"

/// <summary>
/// タイトルシーン
/// </summary>

class TitleScene : public BaseScene {
public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(DirectXCommon* dxCommon) override;
	/// <summary>
	/// 終了
	/// </summary>
	void Finalize() override;

	/// <summary>
	/// 毎フレーム更新
	/// </summary>
	void Update(DirectXCommon* dxCommon) override;
	/// <summary>
	/// 描画
	/// </summary>
	void Draw(DirectXCommon* dxCommon) override;

private:

	void Feed(const std::string& sceneName);

	//メンバ変数
	enum {
		BackScene,
		Button,
		moveTitle,
		moveDebug,
		SpriteMax,
	};
	std::unique_ptr<Sprite> UI[SpriteMax];
	std::unique_ptr<Sprite> Effect;
	std::unique_ptr<TitleText> text;
	bool Change = false;
	float alpha = 0.0f;
	float frame = 0;

	enum Portal {
		Title=0,
		Debug,

	};

	int nextScene = Portal::Title;

	XMFLOAT2 SetPos[3]={ { 640.0f,300.0f } ,{ 640.0f,360.0f } ,{ 640.0f,460.0f } };
	
	float moveText[2]{};
	int moveTitleNow = 1;
	int moveDebugNow = 2;

	std::string SceneName = "null";
};

