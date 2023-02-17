#include "Boss.h"
#include <ActorManager.h>


void(Boss::*Boss::phaseFuncTable[])() = {
	&Boss::Func1,//要素0
	&Boss::Func2 //要素1
};

void Boss::Func1() {
	if (fbxObject_->GetIsFinish()) { animation_count_++; }
	if (animation_count_ >= 2) {
		fbxObject_->StopAnimation();
		if (scaframe < 1.0f) {
			scaframe += 1.0f / (kPredictTime / kScaleCount);
		} else {
			scaframe = 0.0f;
		}
		scale = Ease(In, Quad, scaframe, 25.0f, 16.0f);
		fbxObject_->SetScale({ scale * 0.001f,scale * 0.001f,scale * 0.001f });
	}
}

void Boss::Func2() {
}

void Boss::LoadData(const std::string& _bossname) {
	BossLevelLoader& gFoo = Singleton<BossLevelLoader>::get_instance();
	//名前からステータスを取得
	levelData_= gFoo.takeData(_bossname);
	//体力
	hp = 40;
	max_hp = hp;
	//速度
	vel_ = levelData_.vel;
	//クールタイム
	cooltime_ = levelData_.coolTime;
	//最初のフェーズ
	std::string phase = levelData_.firstPhase;
	if (phase.find("Approch") == 0) {
		command = Actor::Phase::APPROCH;
	} else if(phase.find("UnGuard") == 0) {
		command = Actor::Phase::UNGUARD;
	} else if (phase.find("Leave") == 0) {
		command = Actor::Phase::LEAVE;
	} else if (phase.find("Wait") == 0) {
		command = Actor::Phase::WAIT;
	}

}

void Boss::InitCommon(FBXModel* _model,XMFLOAT3 _scale, XMFLOAT3 _rotation) {
	//アクターのobjの非表示
	isVisible = false;
	//fbxモデル生成
	fbxObject_= std::make_unique<FBXObject3d>();
	fbxObject_->Initialize(_model, _scale, _rotation);
	fbxObject_->LoadAnimation();
	//プレイヤーを参照
	player_ = ActorManager::GetInstance()->SearchActor("Player");
	//丸影
	Object2d* Shadow_ = Object2d::Create(ImageManager::Shadow, { 0,0,0 },
		{ 0.7f,0.7f,0.7f }, { 1,1,1,1 });
	Shadow_->SetRotation({ 90,0,0 });
	shadow_.reset(Shadow_);
	//パーティクル
	particle_ = std::make_unique<ParticleEmitter>(ImageManager::nul);
}

