#pragma once
#include "SceneManager.h"
#include "Actor.h"
#include <SourceCode/User/Bullet.h>

/// <summary>
/// タイトルシーン
/// </summary>

class DebugScene : public BaseScene {
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

	void Save();

	void EnemySave();

private:
	enum :int {
		Approch,
		Leave,
		Wait,
	};
	//メンバ変数
	enum {
		MPlayer,
		MEnemy,
		Chr_Max
	};
	Actor* Chara[Chr_Max]{};
	Bullet* bullet=nullptr;
	std::unique_ptr<Sprite>back=nullptr;
	XMFLOAT3 rot;
	//調整するキャラ
	int Select = 0;

	//共通ステ
	int hp;
	float vel;
	int stock;
	//敵ステ
	float waitTimer;
	int phase = 0;

	bool save = false;
};

