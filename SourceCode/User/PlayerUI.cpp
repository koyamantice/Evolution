#include"PlayerUI.h"
#include"ActorManager.h"
#include <SourceCode/User/ImageManager.h>
PlayerUI::PlayerUI() {
}

PlayerUI::~PlayerUI() {
}

void PlayerUI::OnInitialize() {
	Sprite* _Hp;
	_Hp = Sprite::Create(ImageManager::Test, { 150,658 });
	_Hp->SetColor({0,1.0f,0,1 });
	_Hp->SetSize({ 32,32 });
	Hp.reset(_Hp);

	Sprite* _Chara;
	_Chara = Sprite::Create(ImageManager::CharaCover, { 10,582 });
	Chara.reset(_Chara);

	Sprite* _Bullet;
	_Bullet = Sprite::Create(ImageManager::CharaCover, { 680,592 });
	_Bullet->SetScale(0.8f);
	Bullet.reset(_Bullet);
	

	Sprite* _slash;
	_slash = Sprite::Create(ImageManager::slash, { 1010,600 });
	_slash->SetScale(0.8f);
	Slash.reset(_slash);

	Sprite* _numBack[2];
	const DirectX::XMFLOAT2 numPos[2] = { {1050,590},{810,590} };
	for (int i = 0; i < 2;i++) {
		_numBack[i] = Sprite::Create(ImageManager::numBack, numPos[i]);
	//	_numBack[i]->SetScale(0.8f);
		numBack[i].reset(_numBack[i]);
	}

	const int w = 54;
	const int h = 60;
	const int l = 10;
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 10; j++) {
			num[i][j] = Sprite::Create(ImageManager::Num, { 0,0 });
			int number_index_y = j / l;
			int number_index_x = j % l;
			num[i][j]->SetTextureRect(
				{ static_cast<float>(number_index_x) * w, static_cast<float>(number_index_y) * h },
				{ static_cast<float>(w), static_cast<float>(h) });
			num[i][j]->SetSize({ 84,84 });
			num[i][j]->SetScale(1.0f);
			num[i][j]->SetAnchorPoint({ 0,0 });
		}
	}
	for (int j = 0; j < 10; j++) {
		num[0][j]->SetPosition({ 1150 ,600 });
		num[1][j]->SetPosition({ 1060, 600 });
	}
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 10; j++) {
			numBullet[i][j] = Sprite::Create(ImageManager::Num, { 0,0 });
			int number_index_y = j / l;
			int number_index_x = j % l;
			numBullet[i][j]->SetTextureRect(
				{ static_cast<float>(number_index_x) * w, static_cast<float>(number_index_y) * h },
				{ static_cast<float>(w), static_cast<float>(h) });
			numBullet[i][j]->SetSize({ 84,84 });
			numBullet[i][j]->SetScale(1.0f);
			numBullet[i][j]->SetAnchorPoint({ 0,0 });
		}
	}
	for (int j = 0; j < 10; j++) {
		numBullet[0][j]->SetPosition({ 910 ,600 });
		numBullet[1][j]->SetPosition({ 820, 600 });
	}
}

void PlayerUI::OnUpdate() {
	Actor* AttachActor = ActorManager::GetInstance()->SearchActor("Player");
	
	Hp->SetSize({AttachActor->GetHp() * 50 ,32});
	if (AttachActor->GetHp()<0) {
		Hp->SetSize({ 0 ,32 });
	}
	
	//count++;
	//Timer = count / 60.0f;

	OnLive = AttachActor->GetStock();
	onLive.clear();
	for (int tmp = OnLive; tmp > 0;) {
		onLive.push_back(tmp % 10);
		tmp /= 10;
	}


	Stock = ActorManager::GetInstance()->SearchNum("Bullet");
	if (Stock>100) {
		Stock=99;
	}
	stock.clear();
	for (int tmp = Stock; tmp > 0;) {
		stock.push_back(tmp % 10);
		tmp /= 10;
	}

}

void PlayerUI::OnFinalize() {
}

void PlayerUI::OnDraw() {
	Sprite::PreDraw();
	Chara->Draw();
	Bullet->Draw();
	Hp->Draw();
	Slash->Draw();
	for (int i = 0; i < 2; i++) {
		numBack[i]->Draw();
	}
	for (int i = 0; i < stock.size() && i < 2; i++) {
		num[i][stock[i]]->Draw();
	}
	if (stock.size()==0) {
		num[0][0]->Draw();
	}
	for (int i = 0; i < onLive.size() && i < 2; i++) {
		numBullet[i][onLive[i]]->Draw();
	}
	if (onLive.size() == 0) {
		numBullet[0][0]->Draw();
	}

}

void PlayerUI::OnAttached() {
}
