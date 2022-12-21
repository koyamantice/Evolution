#include"EnemyUI.h"
#include"ActorManager.h"
#include <SourceCode/User/ImageManager.h>
#include <SourceCode/Common/Easing.h>
EnemyUI::EnemyUI() {
}

EnemyUI::~EnemyUI() {
}

void EnemyUI::OnInitialize() {
	Sprite* _HpCover;
	_HpCover = Sprite::Create(ImageManager::HpCover, { 730,20 });
	HpCover.reset(_HpCover);

	Sprite* _HpGauge;
	_HpGauge = Sprite::Create(ImageManager::HpRed, { 730 + 13,20 + 4 });
	HpGauge.reset(_HpGauge);

	Sprite* _UnderGauge;
	_UnderGauge = Sprite::Create(ImageManager::HpGreen, { 730 + 13,20 + 4 });
	UnderGauge.reset(_UnderGauge);
}

void EnemyUI::OnUpdate() {
	Actor* AttachActor = ActorManager::GetInstance()->SearchActor("Enemy");
	

	static const float MaxHp = AttachActor->GetHp();
	HpGauge->SetSize({ 495 * (AttachActor->GetHp() / MaxHp) ,22 });
	HpGauge->SetAnchorPoint({ 0,0 });
	if (AttachActor->GetHp() < 0) {
		HpGauge->SetSize({ 0 ,32 });
	}

	XMFLOAT2 AfterPos = HpGauge->GetSize();
	XMFLOAT2 pos = UnderGauge->GetSize();
	pos.x = Ease(In, Quad, 0.3f, pos.x, AfterPos.x);
	pos.y = Ease(In, Quad, 0.3f, pos.y, AfterPos.y);
	UnderGauge->SetSize(pos);
}

void EnemyUI::OnFinalize() {
}

void EnemyUI::OnDraw() {
	Sprite::PreDraw();
	HpCover->Draw();
	UnderGauge->Draw();
	HpGauge->Draw();
}

void EnemyUI::OnAttached() {
}
