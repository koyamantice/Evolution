#pragma once
#include"ActorComponent.h"
#include <Input.h>

class Player: public ActorComponent {
public :
	void OnInitialize();	//初期化処理(継承版)
	void OnUpdate();	//更新処理(継承版)
	void OnFinalize();	//終了処理(継承版)
	void OnDraw();	//描画処理(継承版)
	void OnAttached();	//ゲームオブジェクトにアタッチされたときに呼ばれる関数
	Input* input = Input::GetInstance();
	


};