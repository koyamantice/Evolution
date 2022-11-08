#pragma once
#include"ActorComponent.h"
#include <Player.h>
#include <SourceCode/Obj/2d/Sprite.h>

class EnemyUI : public  ActorComponent {
public:
	EnemyUI();
	~EnemyUI();
	void OnInitialize() override;	//初期化処理(継承版)
	void OnUpdate() override;	//更新処理(継承版)
	void OnFinalize() override;	//終了処理(継承版)
	void OnDraw() override;		//描画処理(継承版)
	void OnAttached() override;	//ゲームオブジェクトにアタッチされたときに呼ばれる関数

private:
	std::unique_ptr<Sprite> Hp;
	std::unique_ptr<Sprite> Slash;
	Sprite* num[2][10]{};
	std::unique_ptr<Sprite> numBack[2];
	Sprite* numBullet[2][10]{};

	int Stock = 0;
	std::vector<int> stock;

	int OnLive = 0;
	std::vector<int> onLive;
	float frame = 0;
	float c = 0;
	float a = 0.01f;
	std::unique_ptr<Sprite> Chara;
	std::unique_ptr<Sprite> Bullet;
};