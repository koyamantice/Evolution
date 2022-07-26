#include"PlayerUI.h"
#include"ActorManager.h"
#include <SourceCode/User/ImageManager.h>
PlayerUI::PlayerUI() {
	attachActor = ActorManager::GetInstance()->SearchActor("Player");
	
	Sprite* _Hp;
	_Hp=Sprite::Create(ImageManager::Test,{0,688});
	Hp.reset(_Hp);
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
	Hp->Draw();
}

void PlayerUI::OnAttached() {
}
