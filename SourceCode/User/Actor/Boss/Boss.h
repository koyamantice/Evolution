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
	void ResultOnUpdate(const float timer)override;
	/// <summary>
	/// 共通初期化
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="scale">スケール</param>
	/// <param name="rotation">ローテーション</param>
	void InitCommon(FBXModel* model = nullptr, XMFLOAT3 scale = { 1,1,1 }, XMFLOAT3 rotation = { 0,0,0 });
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
	/// <param name="bossname">ボスネーム</param>
	void LoadData(const std::string& bossname);

protected:
	float pinchLife = 20;
	//レベルデータ
	float vel_ = 0;
	XMFLOAT3 baseScale_ = {};
	float coolTimeMax_ = 0;
	float predictTimeMax_ = 0;
	float attackTimeMax_ = 0;

	bool isResult_ = false;

	//関数ポインタ
	static void(Boss::*phaseFuncTable[])();
	//メンバ関数
	virtual void StartAction() {};
	virtual void AttackPredict() {};
	virtual void JumpAttack() {};
	virtual void PressAttack() {};
	virtual void ChasePlayer() {};
	virtual void FeedHoney() {};
	void DeadMotion();
	virtual void StopMotion() {};
	void PinchMotion();
	virtual void SpecialPinch() {};
	enum class E_Phase : int {
		kStartAction = 0,
		kAttackPredict,
		kJumpAttack,
		kPressAttack,
		kChasePlayer,
		kFeedHoney,
		kDeadMotion,
		kStopMotion,
		kPinchMotion,
	};
	//どの行動を取るか
	E_Phase phase_ = E_Phase::kStartAction;
	E_Phase old_phase_ = phase_;

	int animation_count_ = 0;
	float smash_scale_ = 0.0f;
	float smash_shadow_ = 0.0f;
	float smash_rot_ = 0.0f;

	float scale_frame_ = 0.0f;

	float waittimer_ = 0;

	float shadow_side_ = 1.5f;
	//player
	Actor* player_;
	//レベルデータ
	BossLevelLoader::LevelData levelData_ = {};
	//ユニークポインタ
	std::unique_ptr<FBXObject3d> fbxObject_;
	std::unique_ptr<Object2d> shadow_ = nullptr;
	std::unique_ptr <ParticleEmitter> particle_ = nullptr;
	std::list<std::unique_ptr<TimedInvoke>> timedInvokes_;
};

