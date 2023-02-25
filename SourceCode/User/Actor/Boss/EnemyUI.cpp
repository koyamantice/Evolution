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
	_HpGauge = Sprite::Create(ImageManager::HpRed, { 730,20 });
	HpGauge.reset(_HpGauge);
	HpGauge->SetSize({});
	Sprite* _UnderGauge;
	_UnderGauge = Sprite::Create(ImageManager::HpGreen, { 730,20 });
	UnderGauge.reset(_UnderGauge);
}

void EnemyUI::OnUpdate() {
	Actor* AttachActor = ActorManager::GetInstance()->SearchActor("Enemy");
	
	HpGauge->SetSize({ HpCover->GetSize().x * (AttachActor->GetHp() / AttachActor->GetMaxHp()) ,HpCover->GetSize().y});
	HpGauge->SetAnchorPoint({ 0,0 });

	if (AttachActor->GetHp() < 0) {
		HpGauge->SetSize({ 0,HpCover->GetSize().y });
	}

	XMFLOAT2 after_pos = HpGauge->GetSize();
	XMFLOAT2 pos = UnderGauge->GetSize();
	pos.x = Ease(In, Quad, 0.3f, pos.x, after_pos.x);
	pos.y = Ease(In, Quad, 0.3f, pos.y, after_pos.y);
	UnderGauge->SetSize(pos);
}

void EnemyUI::OnFinalize() {
}

void EnemyUI::OnDraw() {
	Sprite::PreDraw();
	UnderGauge->Draw();
	HpGauge->Draw();
	HpCover->Draw();
}

void EnemyUI::OnAttached() {
}
