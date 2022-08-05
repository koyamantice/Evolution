#pragma once
#include"ActorComponent.h"
#include <Player.h>
#include <SourceCode/Obj/2d/Sprite.h>




class PlayerUI : public  ActorComponent {
public:
	PlayerUI();
	~PlayerUI();
	void OnInitialize() override;	//初期化処理(継承版)
	void OnUpdate() override;	//更新処理(継承版)
	void OnFinalize() override;	//終了処理(継承版)
	void OnDraw() override;		//描画処理(継承版)
	void OnAttached() override;	//ゲームオブジェクトにアタッチされたときに呼ばれる関数

private:
	std::unique_ptr<Sprite> Hp;
	std::unique_ptr<Sprite> Chara;
	std::unique_ptr<Sprite> Bullet;
};
