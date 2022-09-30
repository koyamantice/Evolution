#include"EnemyUI.h"
#include"ActorManager.h"
#include <SourceCode/User/ImageManager.h>
EnemyUI::EnemyUI() {
}

EnemyUI::~EnemyUI() {
}

void EnemyUI::OnInitialize() {
	Sprite* _Hp;
	_Hp = Sprite::Create(ImageManager::Test, { 400 + 150,40 });
	_Hp->SetColor({ 0,1.0f,0,1 });
	_Hp->SetSize({ 32,32 });
	Hp.reset(_Hp);
	
}

void EnemyUI::OnUpdate() {
	Actor* AttachActor = ActorManager::GetInstance()->SearchActor("Enemy");

	Hp->SetSize({ (AttachActor->GetHp() * 15) ,32 });
	if (AttachActor->GetHp() < 0) {
		Hp->SetSize({ 0 ,32 });
	}

}

void EnemyUI::OnFinalize() {
}

void EnemyUI::OnDraw() {
	Sprite::PreDraw();
	Hp->Draw();
}

void EnemyUI::OnAttached() {
}
