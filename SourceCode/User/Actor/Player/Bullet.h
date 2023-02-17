#pragma once
#include"Actor.h"
#include"FBXObject3d.h"
#include"Object2d.h"
#include"ImageManager.h"
#include <AudioManager.h>

using namespace DirectX;
class Bullet {
protected: // エイリアス
// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

	struct FlockSystem {
		XMFLOAT3 average{};
		//分離
		float isolateRadius = 2.0f;
	
		float ctrDirX;
		float ctrDirY;
		float contX;
		float contY;
		XMFLOAT2 vel;
		float weight;
		float aveAngle;
		float disvel=(float)(rand()%5);
		float dx = disvel* cosf(aveAngle * (XM_PI / 180));
		float dy = disvel* sinf(aveAngle * (XM_PI / 180));

	};
public:
	enum command {
		Wait = 0,
		Attack,
		Control,
		Slow,
		Dead,
	};
public:
	Bullet();
	virtual ~Bullet() = default;

	//座標の取得
	void SetPosition(const DirectX::XMFLOAT3& pos) { fbxObj->SetPosition(pos); }
	const DirectX::XMFLOAT3& GetPosition() { return fbxObj->GetPosition(); }
	//角度の取得
	void SetRotation(const DirectX::XMFLOAT3& rot) { fbxObj->SetRotation(rot); }
	const DirectX::XMFLOAT3& GetRotation() { return fbxObj->GetRotation(); }

	//ID取得
	const int& GetID()const { return ID; }

	//isAliveセッタ＆ゲッタ
	void SetIsActive(const bool& Active) { isActive = Active; };
	const bool& GetIsActive() { return isActive; }

	//isRemoveセッタ＆ゲッタ
	void SetIsRemove(const bool& Remove) { isRemove = Remove; };
	const bool& GetIsRemove() { return isRemove; }

	//カメラ角度のセッタ＆ゲッタ
	void SetAngle(const float& angle) { this->angle = angle; }
	const float& GetAngle() { return angle; }
	//
	void SetsPlayActive(const bool& Play) { isPlayActive = Play; };
	const bool& GetIsPlayActive() { return isPlayActive; }
	//
	void SetCommand(const int& command, XMFLOAT3 pos = { 0,0,0 });
	const int& GetCommand() { return command; }
	//
	void SetDeadFlag(const bool& DeadFlag) { this->DeadFlag = DeadFlag; }
	//基本処理
	void Initialize(FBXModel* model, const std::string& tag = "None", ActorComponent* compornent = nullptr);	//初期化処理
	void Update();		//更新処理
	void Demo();		//更新処理
	void IntroUpdate(const float& Timer, const int& _stage);
	void ResultUpdate(const float& Timer);
	void FirstDraw(DirectXCommon* dxCommon);
	void Draw(DirectXCommon* dxCommon);	//描画処理
	void LastDraw(DirectXCommon* dxCommon);
	void DemoDraw(DirectXCommon* dxCommon);	//描画処理

	void Finalize();	//終了処理

	float DirRotation(const XMFLOAT3& target);
	
	void Navigation(const XMFLOAT3& target);

	void OnCollision(const std::string& Tag,const XMFLOAT3& pos);
	virtual void BulletCollision(const XMFLOAT3& pos, const int& Id) {};
protected:
	virtual void OnInit() {};
	virtual void OnUpda() {};
	virtual void OnFirstDraw(DirectXCommon* dxCommon) {};
	virtual void OnDraw(DirectXCommon* dxCommon) {};
	virtual void OnLastDraw(DirectXCommon* dxCommon) {};
	virtual void OnFinal() {};
	virtual void DebugUpdate() {};
	virtual void IntroOnUpdate(const float& Timer) {};
	virtual void ResultOnUpdate(const float& Timer) {};
	void SetAggregation();
	void LimitArea();
	void CommonUpda();

	void CommandUpda();

	void ControlUpda();
	void WaitUpda();
	void SlowUpda();
	void AttackUpda();
	bool Follow2Position(const XMFLOAT3& _pos,const float& _radius = 2.0f);
	float follow_vel_ = 0.3f;
	void KnockBack();
	bool knockBacking = false;

	void DamageInit();
	XMFLOAT3 s_rebound_{};
	XMFLOAT3 e_rebound_{};

	XMFLOAT3 exploPos{};
	float damageframe = 0.0f;
	const float kFallHeight = 1.0f;
	float fall = kFallHeight;
	const float kDeadFrameMax = 20.0f;
	float deadframe = 0;

	bool isLeft = false;
	bool throwReady = false;
	float frame = 0.0f;
	const float kSlowFrameMax = 50.0f;
	const float kSlowHight = 1.0f;
	float vel = kSlowHight;
	void DeadEnd();
	float vanishHight = 0.1f;
	float vanishAlpha = 1.0f;
	int CoolTime = 0;
	bool wait = false;
	bool burning = false;
	void BurnOut();
	float scale = 0.0f;
	float effectRate = 0.0f;
	bool follow = false;
	XMFLOAT2 vel_follow{};



	bool clear_ease =true;
	float clear_frame = 0;
	XMFLOAT3 clear_s_pos{};
	XMFLOAT3 clear_e_pos{};
protected:
	//バレット専用
	int ID;
	//機能中か
	bool isActive = true;
	//削除
	bool isRemove = false;
	//コマンド
	int command = 0;
	std::unique_ptr<FBXObject3d> fbxObj;
	std::unique_ptr<Object2d> Shadow = nullptr;
	std::unique_ptr<Object2d> Status = nullptr;
	std::unique_ptr<Object2d> Explo = nullptr;
	std::unique_ptr<Object2d> CharaDead = nullptr;
	std::unique_ptr<AudioManager> audioManager = nullptr;

	enum DeathColor{
		Red=ImageManager::RedDead,
		Green,
	};
	int Color = 0;

	float angle = 0;
	bool DeadFlag = false;

	bool collide = false;
	XMFLOAT3 old_pos = {};
	bool navi = false;
	float navi_frame = 0;
	XMFLOAT3 before_pos{};
	XMFLOAT3 after_pos{};
	float follow_frame = 0.0f;
	XMFLOAT3 follow_pos{};

	Actor* enemy = nullptr;
	Actor* player = nullptr;
	Actor* ActionActor = nullptr;
	XMFLOAT3 landing{};

	bool isPlayActive = false;

	float margin;

	float hight;
};

