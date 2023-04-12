#pragma once
#include"Actor.h"
#include"FBXObject3d.h"
#include"Object2d.h"
#include"ImageManager.h"
#include <AudioManager.h>
#include"Trace.h"
#include <random>

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
		XMFLOAT2 vel_;
		float weight;
		float aveAngle;
		float disvel;
		float dx = disvel * cosf(aveAngle * (XM_PI / 180));
		float dy = disvel * sinf(aveAngle * (XM_PI / 180));

	};
public:
	enum class BulletStatus : int {
		Wait = 0,
		WakeUp,
		Attack,
		Battle,
		Control,
		Slow,
		Dead,
		Smash,
		Ditch,
		Vanish,
		Scary,
	};
public:
	Bullet();
	virtual ~Bullet() = default;

	//座標の取得
	void SetPosition(const DirectX::XMFLOAT3& pos) { fbxobj_->SetPosition(pos); }
	const DirectX::XMFLOAT3& GetPosition() { return fbxobj_->GetPosition(); }
	//座標の取得
	const DirectX::XMFLOAT3& GetOldPosition() { return old_pos; }

	//角度の取得
	void SetRotation(const DirectX::XMFLOAT3& rot) { fbxobj_->SetRotation(rot); }
	const DirectX::XMFLOAT3& GetRotation() { return fbxobj_->GetRotation(); }

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
	//プレイアクティブ
	void SetsPlayActive(const bool& Play) { isPlayActive = Play; };
	const bool& GetIsPlayActive() { return isPlayActive; }
	//
	void SetCommand(const BulletStatus& command, XMFLOAT3 pos = { 0,0,0 });
	const BulletStatus& GetCommand() { return command_; }
	//
	void SetDeadFlag(const bool& DeadFlag) { this->DeadFlag = DeadFlag; }
	const bool& GetDeadFlag() { return DeadFlag; }
	//基本処理
	void Initialize(FBXModel* model, const std::string& tag = "None", ActorComponent* compornent = nullptr);	//初期化処理
	void Update();		//更新処理
	void IntroUpdate(const float& timer, const int& _stage);
	void ResultUpdate(const float& timer);
	void FirstDraw(DirectXCommon* dxCommon);
	void Draw(DirectXCommon* dxCommon);	//描画処理
	void LastDraw(DirectXCommon* dxCommon);
	void DemoDraw(DirectXCommon* dxCommon);	//描画処理

	void Finalize();	//終了処理

	float DirRotation(const XMFLOAT3& target);

	void Navigation(const XMFLOAT3& target);

	void OnCollision(const std::string& Tag, const XMFLOAT3& pos);
	virtual void BulletCollision(const XMFLOAT3& pos, const int& Id) {};
protected:
	virtual void OnInitialize() {};
	virtual void OnUpdate() {};
	virtual void OnFirstDraw(DirectXCommon* dxCommon) {};
	virtual void OnDraw(DirectXCommon* dxCommon) {};
	virtual void OnLastDraw(DirectXCommon* dxCommon) {};
	virtual void OnFinalize() {};
	virtual void DebugUpdate() {};
	virtual void IntroOnUpdate(const float& timer) {};
	virtual void ResultOnUpdate(const float& timer) {};
	void SetAggregation();
	void LimitArea();
	void CommonUpdate();

	void ShadowUpdate();
	void TraceUpdate();

	//enum class BulletStatus : int {
	//	Wait = 0,
	//	Attack,
	//  Battle,
	//	Control,
	//	Slow,
	//	Dead,
	//	Smash,
	//  Ditch,
	//};
	void WaitUpdate();
	void WakeUpUpdata();
	void AttackUpdate();
	void BattleUpdata();
	void ControlUpdate();
	void SlowUpdate();
	void DeadUpdate();
	void SmashUpdate();
	void DitchUpdate();
	void VanishUpdate();
	virtual bool DitchInit() { return false; };
	int ditch_count = 0;
	const int DitchCountMax = 180;
	virtual void VanishCommand() { command_ = BulletStatus::Wait; }
	void ScaryUpdate();
	//関数ポインタ
	static void(Bullet::* statusFuncTable[])();


	bool Follow2Position(const XMFLOAT3& _pos, const float& _radius = 2.0f);
	float follow_vel_ = 0.3f;


	void DamageInit(BulletStatus status= BulletStatus::Attack);
	void ScaryInit(const int& proba=5);
	XMFLOAT3 s_rebound_{};
	XMFLOAT3 e_rebound_{};

	XMFLOAT3 exploPos{};

	bool noBoss = false;
	float damageframe = 0.0f;
	const float kFallHeight = 1.0f;
	float fall = kFallHeight;
	const float kDeadFrameMax = 20.0f;
	float deadframe = 0;

	bool isFinish = false;
	float status_alpha_ = 1.0f;

	const float kScaryFrameMax = 50.0f;


	const float kVanishFrameMax = 50.0f;
	const float kVanishHight = 1.0f;
	float vanish_vel_ = kVanishHight;



	bool isLeft = false;
	bool throwReady = false;
	float frame = 0.0f;
	const float kSlowFrameMax = 50.0f;
	const float kSlowHight = 1.0f;
	float vel_ = kSlowHight;
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

	float shadow_side_ = 0.2f;

	bool clear_ease = true;
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
	BulletStatus command_ = BulletStatus::Wait;
	BulletStatus next_command_ = command_;

	std::unique_ptr<FBXObject3d> fbxobj_ = nullptr;
	std::unique_ptr<Object2d> shadow_ = nullptr;
	std::unique_ptr<Object2d> under_status_ = nullptr;

	std::list<std::unique_ptr<Trace>> traces_ = {};
	enum {
		BattleState=0,
		ScaryState,
		VanishState,
		ControlState,
		DitchState,
		StateMax
	};
	
	std::array<std::unique_ptr<Object2d>, StateMax> status_;
	std::unique_ptr<Object2d> explo_ = nullptr;
	std::unique_ptr<Object2d> chara_dead_ = nullptr;
	std::unique_ptr<AudioManager> audio_ = nullptr;

	const int kFootCountMax = 30;
	int foot_count_ = 0;
	int odd_count_ = 0;

	enum DeathColor {
		Red = ImageManager::RedDead,
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
	bool dig_action = false;
	Actor* enemy = nullptr;
	Actor* player = nullptr;
	Actor* ActionActor = nullptr;

	XMFLOAT3 landing{};

	bool isPlayActive = false;

	float margin;

	float hight;
};

