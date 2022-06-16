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
	//メンバ変数
	enum {
		Player,
		Enemy,
		Chr_Max
	};
	std::unique_ptr<Actor> actor[Chr_Max]{};

	enum {
		back,
		button,
		select,
		ground,
		tutorial,
		SpriteMax
	};
	Sprite* sprite[SpriteMax]{};
	//各クラスのもの
	DebugCamera* camera = { nullptr };
	enum Select {
		Start,
		Select,
		Exit,
	};
	XMFLOAT2 buttonPos[2]{
		{ 320.0f,600.0f },
		{ 440.0f,600.0f }
	};
	int SelectNumber = 0;
};

