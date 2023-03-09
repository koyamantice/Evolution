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
	/// ���ʏ�����
	/// </summary>
	/// <param name="_model">���f��</param>
	/// <param name="_scale">�X�P�[��</param>
	/// <param name="_rotation">���[�e�[�V����</param>
	void InitCommon(FBXModel* _model = nullptr, XMFLOAT3 _scale = { 1,1,1 }, XMFLOAT3 _rotation = { 0,0,0 });
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
	/// <param name="_bossname">�{�X�l�[��</param>
	void LoadData(const std::string& _bossname);
	//���x���f�[�^
	float vel_ = 0;
	XMFLOAT3 baseScale_ = {};
	float coolTimeMax_ = 0;
	float predictTimeMax_ = 0;
	float attackTimeMax_ = 0;


	//�֐��|�C���^
	static void(Boss::*phaseFuncTable[])();
	//�����o�֐�
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
	//�ǂ̍s������邩
	E_Phase phase_ = E_Phase::kStartAction;

	int animation_count_ = 0;

	float scale_frame_ = 0.0f;

	float waittimer_ = 0;


	//���x���f�[�^
	BossLevelLoader::LevelData levelData_ = {};
	//���j�[�N�|�C���^
	std::unique_ptr<FBXObject3d> fbxObject_;
	std::unique_ptr<Object2d> shadow_ = nullptr;
	std::unique_ptr <ParticleEmitter> particle_ = nullptr;
	std::list<std::unique_ptr<TimedInvoke>> timedInvokes_;
};

