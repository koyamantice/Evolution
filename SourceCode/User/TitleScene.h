#pragma once
#include "SceneManager.h"
#include "Actor.h"

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
		Button,
		SpriteMax,
	};
	std::unique_ptr<Sprite> UI[SpriteMax];
	std::unique_ptr<Sprite> Effect;
	bool Change = false;
	float alpha = 0.0f;
	float frame = 0;
	std::string SceneName = "null";
};

