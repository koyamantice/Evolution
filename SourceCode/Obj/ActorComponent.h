#pragma once
#include<memory>
#include<string>
#include<memory>
#include "Actor.h"

class ActorComponent {
protected:
	//アタッチしたアクター（所有権はもたない）
	Actor* attachActor;

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
	//
	bool isDamage = false;

	bool magic = false;
public:
	virtual ~ActorComponent() {}

	void Initialize();	//初期化処理(アクター版)
	void Update();		//更新処理(アクター版)
	void Draw();	//描画処理(アクター版)
	void Finalize();	//終了処理(アクター版)
		//ゲームオブジェクトにアタッチ
	void SetAttachActor(Actor* Actor);

		//isAliveセッタ＆ゲッタ
	void SetIsActive(bool Active) { isActive = Active; }
	bool GetIsActive() { return isActive; }

	//isRemoveセッタ＆ゲッタ
	void SetIsRemove(bool Remove) { isRemove = Remove; }
	bool GetIsRemove() { return isRemove; }

	//isDamageセッタ＆ゲッタ
	void SetIsDamage(bool Damage) { isDamage = Damage; }
	bool GetIsDamage() { return isDamage; }

	//isDamageセッタ＆ゲッタ
	void SetMagic(bool magic) { this->magic = magic; }
	bool GetMagic() { return magic; }


};