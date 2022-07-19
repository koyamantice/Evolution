#pragma once
#include "SceneManager.h"
#include "Actor.h"

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
		MBullet,
		MEnemy,
		Chr_Max
	};
	Actor* actor[Chr_Max]{};
	XMFLOAT3 rot;
	//調整するキャラ
	int Select = 0;

	//共通ステ
	int hp;
	float vel;
	//敵ステ
	float waitTimer;
	int phase=0;

	bool save = false;
};

