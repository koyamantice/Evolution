#include"EnemyUI.h"
#include"ActorManager.h"
#include <SourceCode/User/ImageManager.h>
#include <SourceCode/Common/Easing.h>
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
	frame+=a;

	if (frame>1.0f|| frame < 0.0f) {
		a *= -1;
	}
	c=Ease(In,Quad,frame,0,500);



	Hp->SetSize({ c ,32 });
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
