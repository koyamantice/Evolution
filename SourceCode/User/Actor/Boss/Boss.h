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
	/// ���ʏ�����
	/// </summary>
	/// <param name="model">���f��</param>
	/// <param name="scale">�X�P�[��</param>
	/// <param name="rotation">���[�e�[�V����</param>
	void InitCommon(FBXModel* model = nullptr, XMFLOAT3 scale = { 1,1,1 }, XMFLOAT3 rotation = { 0,0,0 });
	/// <summary>
	/// ���C�t����
	/// </summary>
	void LifeCommon();
	/// <summary>
	/// �e�̍X�V
	/// </summary>
	void ShadowUpdate();
	/// <summary>
	/// ���x���f�[�^�ǂݍ���
	/// </summary>
	/// <param name="bossname">�{�X�l�[��</param>
	void LoadData(const std::string& bossname);

protected:
	float pinchLife = 20;
	//���x���f�[�^
	float vel_ = 0;
	XMFLOAT3 baseScale_ = {};
	float coolTimeMax_ = 0;
	float predictTimeMax_ = 0;
	float attackTimeMax_ = 0;

	bool isResult_ = false;

	//�֐��|�C���^
	static void(Boss::*phaseFuncTable[])();
	//�����o�֐�
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
	//�ǂ̍s������邩
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
	//���x���f�[�^
	BossLevelLoader::LevelData levelData_ = {};
	//���j�[�N�|�C���^
	std::unique_ptr<FBXObject3d> fbxObject_;
	std::unique_ptr<Object2d> shadow_ = nullptr;
	std::unique_ptr <ParticleEmitter> particle_ = nullptr;
	std::list<std::unique_ptr<TimedInvoke>> timedInvokes_;
};

