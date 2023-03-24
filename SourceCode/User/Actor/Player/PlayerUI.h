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
	std::unique_ptr<Sprite> Slash;
	std::unique_ptr<Sprite> Damage;
	std::unique_ptr <Sprite> num[2][10]{};
	std::unique_ptr<Sprite> numBack[2];
	std::unique_ptr <Sprite> numBullet[2][10]{};

	int Stock = 0;
	int OldStock = Stock;
	std::vector<int> stock;
	bool stockChange = false;
	float stockFrame = 0.0f;

	float Alpha = 0.0f;
	XMFLOAT2 Dmapos{};
	float DamageFrame = 0.0f;

	int OnLive = 0;
	int OldLive = OnLive;
	bool scaleChange = false;
	bool decrement = false;
	bool increment = false;

	bool stock_decre = false;
	bool stock_incre = false;


	float frame = 0.0f;

	XMFLOAT2 scapos{};
	XMFLOAT2 scapos2{};
	std::vector<int> onLive;
	std::unique_ptr<Sprite> Chara;
	std::unique_ptr<Sprite> Bullet;
};
