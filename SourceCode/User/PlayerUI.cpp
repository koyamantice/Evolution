#include"PlayerUI.h"
#include"ActorManager.h"
#include <SourceCode/User/ImageManager.h>
PlayerUI::PlayerUI() {
	attachActor = ActorManager::GetInstance()->SearchActor("Player");
	Sprite* _Hp;
	_Hp = Sprite::Create(ImageManager::Test, { 150,678 });
	_Hp->SetSize({ 32,32 });
	Hp.reset(_Hp);

	Sprite* _Chara;
	_Chara = Sprite::Create(ImageManager::CharaCover, { 10,582 });
	Chara.reset(_Chara);

	Sprite* _Bullet;
	_Bullet = Sprite::Create(ImageManager::CharaCover, { 680,592 });
	_Bullet->SetScale(0.8f);
	Bullet.reset(_Bullet);

}

PlayerUI::~PlayerUI() {
}

void PlayerUI::OnInitialize() {
}

void PlayerUI::OnUpdate() {
//	Hp->SetSize({ 32,attachActor->GetHp() });
}

void PlayerUI::OnFinalize() {
}

void PlayerUI::OnDraw() {
	Sprite::PreDraw();
	Chara->Draw();
	Bullet->Draw();
	Hp->Draw();
}

void PlayerUI::OnAttached() {
}
