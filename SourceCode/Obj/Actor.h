#pragma once
#include<vector>
#include <memory>
#include<string>
#include <DirectXMath.h>
#include <algorithm>

#include "DirectXCommon.h"
#include "FBXObject3d.h"
#include "Object3d.h"
#include "Object2d.h"


#include"ParticleEmitter.h"

class ActorComponent;

#define AFTIMAGENUM	8
#define DEGREE_MAX 360.0f
#define DEGREE_HALF 180.0f
#define DEGREE_QUARTER 90.0f

class Actor {
protected: // エイリアス
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
protected:
	//付与されているタグへのポインタ
	std::string tag = "None";
public:
	////各フェイズ
	//enum Phase {
	//	APPROCH = 0,
	//	UNGUARD,
	//	LEAVE,
	//	WAIT,
	//	DEAD,
	//	ATTACK,
	//};
	struct HitBound {
		//あたっているか
		bool isHit;
		//あたった座標
		XMFLOAT3 HitingPos;
	};
protected:
	//オブジェクト識別番号
	static int id;
	//機能中か
	bool isActive = true;
	//削除
	bool isRemove = false;
	//参照されているか
	bool isRefer = false;
	//objは必要化
	bool isVisible = true;
	//動いてもいいか
	bool canMove = true;
	//無敵か
	bool isUnrivaled = false;


public:
	virtual ~Actor() = default;
	//座標の取得
	void SetPosition(const DirectX::XMFLOAT3& pos) { obj->SetPosition(pos); }
	const DirectX::XMFLOAT3& GetPosition() { return obj->GetPosition(); }
	//昔の座標の取得
	const DirectX::XMFLOAT3& GetOldPosition() { return old_pos; }
	//プレイヤーの残像
	DirectX::XMFLOAT3 GetAFTIMAGE(const int& num) { return XMFLOAT3{ PlayerX[num + 1],RotY[num + 1],PlayerZ[num + 1] }; }
	//座標の取得
	virtual DirectX::XMFLOAT3 GetCameraPos(const float& angle, const float& str = 10) { return {0,0,0}; };
	//角度の取得
	void SetRotation(const DirectX::XMFLOAT3& rot) { obj->SetRotation(rot); }
	const DirectX::XMFLOAT3& GetRotation() { return obj->GetRotation(); }
	//コマンド取得
	void SetCommand(const int& command) { this->command = command; };
	const int& GetCommand() { return command; }
	
	//hpセッタ＆ゲッタ
	void SetHp(const float& hp) { this->hp = hp; };
	const float& GetHp() { return hp; }
	//maxHpセッタ＆ゲッタ
	void SetMaxHp(const float& _maxhp) { this->max_hp = _maxhp; };
	const float& GetMaxHp() { return max_hp; }

	//Player固有の処理
	virtual const XMFLOAT3& GetLockPos() { return obj->GetPosition(); };
	void SetHitBound(const XMFLOAT3& pos) { hitBound.isHit = true; hitBound.HitingPos = pos; };
	const bool& GetHitBound() { return  hitBound.isHit; }
	//当たり判定サイズ取得
	const float& GetSize()const { return collide_size; }

	//タグのセット
	void SetTag(const std::string& Tag) { tag = Tag; }
	//タグが一致するか
	const std::string& GetTag() { return tag; }

	//isAliveセッタ＆ゲッタ
	void SetIsActive(const bool& Active) { isActive = Active; };
	const bool& GetIsActive() { return isActive; }

	//isRemoveセッタ＆ゲッタ
	void SetIsRemove(const bool& Remove) { isRemove = Remove; };
	const bool& GetIsRemove() { return isRemove; }
		
	//isReferセッタ＆ゲッタ
	void SetIsRefer(const bool& isRefer) { this->isRefer = isRefer; };
	const bool& GetIsRefer() { return isRefer; }

	//canMoveセッタ＆ゲッタ
	void SetCanMove(const bool& canMove) { this->canMove = canMove; };
	const bool& GetCanMove() { return canMove; }

	//カメラ角度のセッタ＆ゲッタ
	void SetAngle(const float& angle) { this->angle = angle; }
	const float& GetAngle() { return angle; }
	//ワイルドイント引数
	void SetStock(const int& Stock) { this->stock = Stock; };
	const int& GetStock() { return stock; }
	//ワイルドブール引数
	void SetPause(const bool& pause) { this->pause = pause; }
	const bool& GetPause() { return pause; }
	//衝突判定
	void SetCollide(const bool& collide) { this->collide = collide; }
	const bool& GetCollide() { return collide; }

	//各処理
	void Initialize(Model* model, const std::string& tag = "None", ActorComponent* compornent = nullptr);	//初期化処理
	void Update();		//更新処理
	/// <summary>
	/// 導入アップデート
	/// </summary>
	/// <param name="Timer"></param>
	void IntroUpdate(const float& Timer);
	/// <summary>
	/// 終了アップデート
	/// </summary>
	/// <param name="Timer"></param>
	void ResultUpdate(const float& Timer);

	void Demo();		//更新処理

	void FirstDraw(DirectXCommon* dxCommon);	//先に描画処理
	void Draw(DirectXCommon* dxCommon);	//描画処理
	void LastDraw(DirectXCommon* dxCommon);	//後で描画処理

	void DemoDraw(DirectXCommon* dxCommon);	//描画処理
	void Finalize();	//終了処理
	//仮想関数
	virtual void OnInit() {};
	virtual void OnUpda() {};
	virtual void OnFirstDraw(DirectXCommon* dxCommon) {};
	virtual void OnDraw(DirectXCommon* dxCommon) {};
	virtual void OnLastDraw(DirectXCommon* dxCommon) {};
	virtual void OnFinal() {};
	virtual void DebugUpdate() {};
	virtual void IntroOnUpdate(const float& Timer) {};
	virtual void ResultOnUpdate(const float& Timer) {};
	virtual void OnCollision(const std::string& Tag) {};
protected:
	int command = 0;
	std::unique_ptr<Object3d> obj;
	float collide_size = 1.0f;
	// 残像データの数
	float PlayerX[AFTIMAGENUM], RotY[AFTIMAGENUM],PlayerZ[AFTIMAGENUM];
	HitBound hitBound;

	float max_hp = 0;
	float hp = 0;
	
	float angle = 0;
	bool pause = false;
	bool first = false;
	bool DeadFlag = false;
	
	bool collide = false;

	XMFLOAT3 old_pos = {};
	int stock = 0;
	//コンポーネント
	ActorComponent* compornent = nullptr;
};