#pragma once
#include"Actor.h"

#include "Player.h"
#include "EnemyUI.h"
#include "EnemyAttack.h"

#include "BossLevelLoader.h"
#include "TimedInvoke.h"

#include "Singleton.h"

#include <SourceCode/Common/Easing.h>

#include<fstream>
#include <sstream>
#include <cstddef>
#include <iostream>

class Boss :public Actor {
public:
	virtual ~Boss() = default;
protected:
	void ResultOnUpdate(const float& Timer)override;
	/// <summary>
	/// 共通初期化
	/// </summary>
	/// <param name="_model">モデル</param>
	/// <param name="_scale">スケール</param>
	/// <param name="_rotation">ローテーション</param>
	void InitCommon(FBXModel* _model = nullptr, XMFLOAT3 _scale = { 1,1,1 }, XMFLOAT3 _rotation = { 0,0,0 });
	/// <summary>
	/// ライフ処理
	/// </summary>
	void LifeCommon();
	/// <summary>
	/// 影の更新
	/// </summary>
	void ShadowUpdate();
	/// <summary>
	/// レベルデータ読み込み
	/// </summary>
	/// <param name="_bossname">ボスネーム</param>
	void LoadData(const std::string& _bossname);
	//レベルデータ
	float vel_ = 0;
	XMFLOAT3 baseScale_ = {};
	float coolTimeMax_ = 0;
	float predictTimeMax_ = 0;
	float attackTimeMax_ = 0;


	//関数ポインタ
	static void(Boss::*phaseFuncTable[])();
	//メンバ関数
	virtual void StartAction() {};
	virtual void AttackPredict() {};
	virtual void PressAttack() {};
	virtual void ChasePlayer() {};
	virtual void FeedHoney() {};
	void DeadMotion();
	virtual void StopMotion() {};

	enum class E_Phase : int {
		kStartAction = 0,
		kAttackPredict,
		kPressAttack,
		kChasePlayer,
		kFeedHoney,
		kDeadMotion,
		kStopMotion,
	};
	//どの行動を取るか
	E_Phase phase_ = E_Phase::kStartAction;

	int animation_count_ = 0;

	float scale_frame_ = 0.0f;

	float waittimer_ = 0;


	//レベルデータ
	BossLevelLoader::LevelData levelData_ = {};
	//ユニークポインタ
	std::unique_ptr<FBXObject3d> fbxObject_;
	std::unique_ptr<Object2d> shadow_ = nullptr;
	std::unique_ptr <ParticleEmitter> particle_ = nullptr;
	std::list<std::unique_ptr<TimedInvoke>> timedInvokes_;
};

