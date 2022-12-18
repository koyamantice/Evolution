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
	_Hp = Sprite::Create(ImageManager::HpCover, { 730,20 });
	//_Hp->SetColor({ 0,1.0f,0,1 });
	//_Hp->SetSize({692/3 ,308/8 });
	Hp.reset(_Hp);
	Sprite* _Hp2;
	_Hp2 = Sprite::Create(ImageManager::HpRed, { 730+13,20+4});
	//_Hp2->SetColor({ 1.0f,0.3f,0.1f,1 });
	//_Hp2->SetSize({ 32,32 });
	Hp2.reset(_Hp2);

	Sprite* _Hp3;
	_Hp3 = Sprite::Create(ImageManager::HpGreen, { 730 + 13,20 + 4 });
	//_Hp2->SetColor({ 1.0f,0.3f,0.1f,1 });
	//_Hp3->SetSize({ 32,32 });
	Hp3.reset(_Hp3);
}

void EnemyUI::OnUpdate() {
	Actor* AttachActor = ActorManager::GetInstance()->SearchActor("Enemy");
	static const float MaxHp = AttachActor->GetHp();
	Hp2->SetSize({ 495 *(AttachActor->GetHp()/ MaxHp) ,22});
	Hp2->SetAnchorPoint({ 0,0 });
	if (AttachActor->GetHp() < 0) {
		Hp2->SetSize({ 0 ,32 });
	}
	XMFLOAT2 AfterPos = Hp2->GetSize();
	XMFLOAT2 pos = Hp3->GetSize();
	pos.x = Ease(In,Quad,0.3f,pos.x,AfterPos.x);
	pos.y = Ease(In, Quad, 0.3f, pos.y, AfterPos.y);
	Hp3->SetSize(pos);



}

void EnemyUI::OnFinalize() {
}

void EnemyUI::OnDraw() {
	Sprite::PreDraw();
	ImGui::Begin("test");
	float F = Hp2->GetSize().x;
	float P = Hp2->GetPosition().x;
	ImGui::SliderFloat("size", &F, 800, 0);
	ImGui::SliderFloat("position", &P, 2000, 0);
	ImGui::Unindent();
	ImGui::End();


	Hp->Draw();
	Hp3->Draw();
	Hp2->Draw();
}

void EnemyUI::OnAttached() {
}
