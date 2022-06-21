#pragma once
#include<memory>
#include<string>

class Actor;

class ActorComponent {
protected:
	//アタッチしたアクター（所有権はもたない）
	std::weak_ptr<Actor> attachActor;

	//仮想関数
	virtual void OnInitialize() {}	//初期化処理(継承版)
	virtual void OnUpdate() {}	//更新処理(継承版)
	virtual void OnFinalize() {}	//終了処理(継承版)
	virtual void OnDraw() {}		//描画処理(継承版)
	virtual void OnAttached() {}	//ゲームオブジェクトにアタッチされたときに呼ばれる関数

	//機能しているか
	bool isActive = true;
	//削除するか
	bool isRemove = false;


public:
	virtual ~ActorComponent() {}

	void Initialize();	//初期化処理(アクター版)
	void Update();		//更新処理(アクター版)
	void Draw();	//描画処理(アクター版)
	void Finalize();	//終了処理(アクター版)
		//ゲームオブジェクトにアタッチ
	void SetAttachActor(std::weak_ptr<Actor> Actor);

		//isAliveセッタ＆ゲッタ
	void SetIsActive(bool Active) { isActive = Active; }
	bool GetIsActive() { return isActive; }

	//isRemoveセッタ＆ゲッタ
	void SetIsRemove(bool Remove) { isRemove = Remove; }
	bool GetIsRemove() { return isRemove; }

};