#pragma once
#include<vector>
#include <memory>
#include<string>
#include"Object3d.h"
#include<DirectXMath.h>

class ActorComponent;

class Actor 
{
private:
	static int ID;
protected: // エイリアス
// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
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
	virtual ~Actor() = default;
	//座標の取得
	void SetPosition(const DirectX::XMFLOAT3& pos) { obj->SetPosition(pos); }
	const DirectX::XMFLOAT3& GetPosition() { return obj->GetPosition(); }
	//角度の取得
	void SetRotation(const DirectX::XMFLOAT3& rot) { obj->SetRotation(rot); }
	const DirectX::XMFLOAT3& GetRotation() { return obj->GetRotation(); }

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
	void Initialize(Model* model,ActorComponent* compornent=nullptr);	//初期化処理
	void Update();		//更新処理
	void Draw();	//描画処理
	void Finalize();	//終了処理

	virtual void OnInit() {};
	virtual void OnUpda() {};
	virtual void OnDraw() {};
	virtual void OnFinal() {};

protected:
	std::unique_ptr<Object3d> obj;
	//コンポーネント
	ActorComponent* compornent = nullptr;
};