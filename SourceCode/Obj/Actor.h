#pragma once
#include<vector>
#include <memory>
#include<string>
#include"Object3d.h"
#include<DirectXMath.h>

class Actor 
{
private:
	static int ID;

protected:
	//名前
	std::string name;
	//付与されているタグへのポインタ
	std::string tag = "None";
private:
	//オブジェクト識別番号(絶対に被ることはない数字)
	int id;
	//機能中か
	bool isActive = true;
	//削除
	bool isRemove = false;
public:
	Actor(const std::string& Name) { SetName(Name); SetTag(Name); id = ID++; }

	virtual ~Actor() {}

	void SetPosition(const DirectX::XMFLOAT3& pos) { obj->SetPosition(pos); }
	const DirectX::XMFLOAT3& GetPosition() { return obj->GetPosition(); }

	//ID取得
	const int& GetID()const { return id; }

	//名前セッタ＆ゲッタ
	void SetName(const std::string& Name) { name = Name; }
	const std::string& GetName() { return name; }

	//タグのセット
	void SetTag(const std::string& Tag){tag = Tag;};
	//タグが一致するか
	const std::string& GetTag() { return tag; }

	//isAliveセッタ＆ゲッタ
	void SetIsActive(bool Active) {isActive = Active;};
	bool GetIsActive() { return isActive; }

	//isRemoveセッタ＆ゲッタ
	void SetIsRemove(bool Remove) {isRemove = Remove;};
	bool GetIsRemove() { return isRemove; }
	
	bool Collide(Actor* actor);

	//virtualにしようか悩み中
	void Initialize(Model* model);	//初期化処理
	void Update();		//更新処理
	void Draw();	//描画処理
	void Finalize();	//終了処理
	//
protected:
	std::unique_ptr<Object3d> obj;
};