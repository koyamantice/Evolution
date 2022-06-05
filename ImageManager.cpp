#include "ImageManager.h"

ImageManager* ImageManager::GetIns() {
	static ImageManager instans;
	return &instans;
}

void ImageManager::Load2D() {
	// テクスチャ読み込み
	Sprite::LoadTexture(playerHp, L"Resources/2d/Ui/HPBar.png");
	Sprite::LoadTexture(arrow, L"Resources/2d/Ui/Arrow.png");
	Sprite::LoadTexture(arrow2, L"Resources/2d/Ui/Arrow2.png");
	Sprite::LoadTexture(TITLE, L"Resources/2d/TitleParts/layoutImage.png");
	Sprite::LoadTexture(SELECT1, L"Resources/2d/Select/noSelect.png");
	Sprite::LoadTexture(hpGauge, L"Resources/2d/Ui/HPGauge.png");
	Sprite::LoadTexture(energy, L"Resources/2d/Ui/HPmark_energy.png");
	Sprite::LoadTexture(pinch, L"Resources/2d/Ui/HPmark_pinch.png");
	Sprite::LoadTexture(weak, L"Resources/2d/Ui/HPmark_weak.png");
	Sprite::LoadTexture(select1, L"Resources/2d/Select/selected.png");
	Sprite::LoadTexture(select2, L"Resources/2d/Select/kine_select.png");
	Sprite::LoadTexture(select3, L"Resources/2d/Select/shoes_Select.png");
	Sprite::LoadTexture(select4, L"Resources/2d/Select/washitu2.png");
	Sprite::LoadTexture(select5, L"Resources/2d/Select/hand_Select.png");
	Sprite::LoadTexture(kitchen, L"Resources/2d/Select/kitchen.png");
	Sprite::LoadTexture(genkan, L"Resources/2d/Select/shoes_Sname.png");
	Sprite::LoadTexture(niwa, L"Resources/2d/Select/kine_Sname.png");
	Sprite::LoadTexture(washitu, L"Resources/2d/Select/washitu.png");
	Sprite::LoadTexture(shinshitu, L"Resources/2d/Select/hand_Sname.png");
	Sprite::LoadTexture(button, L"Resources/2d/TitleParts/cursor2.png");
	Sprite::LoadTexture(num, L"Resources/2d/Ui/Num.png");
	Sprite::LoadTexture(bairitu, L"Resources/2d/Ui/Bairitu.png");
	Sprite::LoadTexture(NumX, L"Resources/2d/Ui/NumX.png");
	Sprite::LoadTexture(NumF, L"Resources/2d/Ui/NumF.png");
	Sprite::LoadTexture(Change, L"Resources/2d/Effect/Change.png");
	Sprite::LoadTexture(Change2, L"Resources/2d/Effect/Change2.png");
	Sprite::LoadTexture(BlackFilter, L"Resources/2d/Effect/Blackfilter.png");
	Sprite::LoadTexture(WhiteFilter, L"Resources/2d/Effect/Whitefilter.png");
	Sprite::LoadTexture(Tselect, L"Resources/2d/TitleParts/menu.png");
	Sprite::LoadTexture(ok, L"Resources/2d/text/Ok.png");
	Sprite::LoadTexture(text1, L"Resources/2d/text/text1.png");
	Sprite::LoadTexture(text2, L"Resources/2d/text/text2.png");
	Sprite::LoadTexture(text3, L"Resources/2d/text/text3.png");
	Sprite::LoadTexture(text4, L"Resources/2d/text/text4.png");
	Sprite::LoadTexture(text5, L"Resources/2d/text/text5.png");
	Sprite::LoadTexture(text6, L"Resources/2d/text/text6.png");
	Sprite::LoadTexture(text7, L"Resources/2d/text/text7.png");
	Sprite::LoadTexture(text8, L"Resources/2d/text/text8.png");
	Sprite::LoadTexture(text9, L"Resources/2d/text/text9.png");
	Sprite::LoadTexture(text10,L"Resources/2d/text/text10.png");
	Sprite::LoadTexture(text11,L"Resources/2d/text/text11.png");
	Sprite::LoadTexture(textSkip, L"Resources/2d/text/textSkip.png");
	Sprite::LoadTexture(Enemyhp,  L"Resources/2d/Ui/EnemyHPBar.png");
	Sprite::LoadTexture(Enemyhp2, L"Resources/2d/Ui/EnemyHPDownBar.png");
	Sprite::LoadTexture(Enemyhp3, L"Resources/2d/Ui/EnemyHPGauge.png");
	Sprite::LoadTexture(SpinGauge, L"Resources/2d/Ui/spinGauge.png");
	Sprite::LoadTexture(SpinBar, L"Resources/2d/Ui/spinGaugeBar.png");
	Sprite::LoadTexture(GameOver, L"Resources/2d/Ui/GameOverLogo.png");
	Sprite::LoadTexture(noBoss_Select, L"Resources/2d/Select/noBoss_Select.png");
	Sprite::LoadTexture(noBoss_Sname, L"Resources/2d/Select/noBoss_Sname.png");
	Sprite::LoadTexture(SPD, L"Resources/2d/Ui/SPD.png");
	Sprite::LoadTexture(StageClear, L"Resources/2d/Ui/ClearMark.png");
	Sprite::LoadTexture(endtext1, L"Resources/2d/endText/endText1.png");
	Sprite::LoadTexture(endtext2, L"Resources/2d/endText/endText2.png");
	Sprite::LoadTexture(endtext3, L"Resources/2d/endText/endText3.png");
	Sprite::LoadTexture(endtext4, L"Resources/2d/endText/endText4.png");
	Sprite::LoadTexture(endtext5, L"Resources/2d/endText/endText5.png");
	Sprite::LoadTexture(endtext6, L"Resources/2d/endText/endText6.png");
	Sprite::LoadTexture(endtext7, L"Resources/2d/endText/endText7.png");
	Sprite::LoadTexture(endtext8, L"Resources/2d/endText/endText8.png");
	Sprite::LoadTexture(endtext9, L"Resources/2d/endText/endText9.png");
	Sprite::LoadTexture(endtext10, L"Resources/2d/endText/endText10.png");
	Sprite::LoadTexture(endroll1, L"Resources/2d/endrollText/endrollText1.png");
	Sprite::LoadTexture(endroll2, L"Resources/2d/endrollText/endrollText2.png");
	Sprite::LoadTexture(endroll3, L"Resources/2d/endrollText/endrollText3.png");
	Sprite::LoadTexture(endroll4, L"Resources/2d/endrollText/endrollText4.png");
	Sprite::LoadTexture(endroll5, L"Resources/2d/endrollText/endrollText5.png");
	Sprite::LoadTexture(endroll6, L"Resources/2d/endrollText/endrollText6.png");
	Sprite::LoadTexture(endroll7, L"Resources/2d/endrollText/endrollText7.png");
	Sprite::LoadTexture(endroll8, L"Resources/2d/endrollText/endrollText8.png");
	Sprite::LoadTexture(endroll9, L"Resources/2d/endrollText/endrollText9.png");
}

void ImageManager::LoadTex2D() {
	Texture::LoadTexture(enemy, L"Resources/2d/Enemy/enemy.png");
	Texture::LoadTexture(shadow, L"Resources/2d/Enemy/shadow.png");
	Texture::LoadTexture(effect, L"Resources/2d/Effect/effect.png");
	Texture::LoadTexture(smoke, L"Resources/2d/Effect/smoke.png");
	Texture::LoadTexture(net, L"Resources/2d/Enemy/motiNet.png");
	Texture::LoadTexture(water, L"Resources/2d/Enemy/puddle.png");
	Texture::LoadTexture(Charge, L"Resources/2d/Effect/Charge.png");
	Texture::LoadTexture(ChargeEffect, L"Resources/2d/Effect/ChargeEffect.png");
	Texture::LoadTexture(spawnhp, L"Resources/2d/Ui/EHubHP.png");
	Texture::LoadTexture(SELECTTex2, L"Resources/2d/Select/noSelect.png");
	Texture::LoadTexture(kitchenSelect, L"Resources/2d/Select/kitchenSelect.png");
	Texture::LoadTexture(JstyleSelect, L"Resources/2d/Select/JstyleSelect.png");
	Texture::LoadTexture(EntranceSelect, L"Resources/2d/Select/EntranceSelect.png");
	Texture::LoadTexture(GardenSelect, L"Resources/2d/Select/GardenSelect.png");
	Texture::LoadTexture(BedroomSelect, L"Resources/2d/Select/BedroomSelect.png");
	Texture::LoadTexture(Stun, L"Resources/2d/Enemy/stun.png");
	Texture::LoadTexture(Hit, L"Resources/2d/Enemy/Attack.png");
	Texture::LoadTexture(Attension, L"Resources/2d/Enemy/Attension.png");
	Texture::LoadTexture(clearStage1, L"Resources/2d/stageMark/clearStage1.png");
	Texture::LoadTexture(clearStage2, L"Resources/2d/stageMark/clearStage2.png");
	Texture::LoadTexture(clearStage3, L"Resources/2d/stageMark/clearStage3.png");
	Texture::LoadTexture(clearStage4, L"Resources/2d/stageMark/clearStage4.png");
	Texture::LoadTexture(clearStage5, L"Resources/2d/stageMark/clearStage5.png");
	Texture::LoadTexture(offStage1, L"Resources/2d/stageMark/offStage1.png");
	Texture::LoadTexture(offStage2, L"Resources/2d/stageMark/offStage2.png");
	Texture::LoadTexture(offStage3, L"Resources/2d/stageMark/offStage3.png");
	Texture::LoadTexture(offStage4, L"Resources/2d/stageMark/offStage4.png");
	Texture::LoadTexture(offStage5, L"Resources/2d/stageMark/offStage5.png");
	Texture::LoadTexture(onStage1, L"Resources/2d/stageMark/onStage1.png");
	Texture::LoadTexture(onStage2, L"Resources/2d/stageMark/onStage2.png");
	Texture::LoadTexture(onStage3, L"Resources/2d/stageMark/onStage3.png");
	Texture::LoadTexture(onStage4, L"Resources/2d/stageMark/onStage4.png");
	Texture::LoadTexture(onStage5, L"Resources/2d/stageMark/onStage5.png");
}
