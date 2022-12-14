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
	_Hp = Sprite::Create(ImageManager::Test, { 950,40 });
	_Hp->SetColor({ 0,1.0f,0,1 });
	_Hp->SetSize({ 32,32 });
	Hp.reset(_Hp);
	Sprite* _Hp2;
	_Hp2 = Sprite::Create(ImageManager::Test, { 950,40 });
	_Hp2->SetColor({ 1.0f,0.3f,0.1f,1 });
	_Hp2->SetSize({ 32,32 });
	Hp2.reset(_Hp2);
}

void EnemyUI::OnUpdate() {
	Actor* AttachActor = ActorManager::GetInstance()->SearchActor("Enemy");
	Hp->SetSize({ AttachActor->GetHp()*10 ,32 });
	if (AttachActor->GetHp() < 0) {
		Hp->SetSize({ 0 ,32 });
	}
	XMFLOAT2 AfterPos = Hp->GetSize();
	XMFLOAT2 pos = Hp2->GetSize();
	pos.x = Ease(In,Quad,0.3f,pos.x,AfterPos.x);
	pos.y = Ease(In, Quad, 0.3f, pos.y, AfterPos.y);
	Hp2->SetSize(pos);



}

void EnemyUI::OnFinalize() {
}

void EnemyUI::OnDraw() {
	Sprite::PreDraw();
	Hp2->Draw();
	Hp->Draw();
}

void EnemyUI::OnAttached() {
}
