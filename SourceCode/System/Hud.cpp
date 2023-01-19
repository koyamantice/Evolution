#include"Hud.h"
#include <ImageManager.h>
#include <Easing.h>

Hud::Hud() {
	Initialize();
}

Hud::~Hud() {
	Finalize();
}

void Hud::Initialize() {
	//カメラの操作説明のテクスチャを一枚の中から切り取ります。
	//画像の一枚あたり元サイズ(ごめん未来の俺)
	const XMFLOAT2 kBasicSize = { 256,256 };
	const XMFLOAT2 kBasicAnchor = { 0.5f,0.5f } ;
	for (int i = 0; i < CAMERACONTROLMAX; i++) {
		Sprite* camera_hud_[CAMERACONTROLMAX]{};
		camera_hud_[i] = Sprite::Create(ImageManager::CameCon, {0,0});
		camera_hud[i].reset(camera_hud_[i]);
		int number_index_y = i / CAMERACONTROLMAX;
		int number_index_x = i % CAMERACONTROLMAX;
		camera_hud[i]->SetTextureRect(
			{ static_cast<float>(number_index_x) * kBasicSize.x, static_cast<float>(number_index_y) * kBasicSize.y },
			{ static_cast<float>(kBasicSize.x), static_cast<float>(kBasicSize.y) });
		//でかすぎるので表示用サイズにします。
		camera_hud[i]->SetSize(kVisibleBasicSize);
		//中心座標にします。
		camera_hud[i]->SetAnchorPoint(kBasicAnchor);
	}
	//座標を真ん中を軸に移動させます.
	for (int i = 0; i < CAMERACONTROLMAX; i++) {
		XMFLOAT2 pos = kCameraHudBase;
		if (i < COMMENT) {
			pos.x -= camera_hud[i]->GetSize().x;
		} else if (i > COMMENT) {
			pos.x += camera_hud[i]->GetSize().x;
		}
		camera_hud[i]->SetPosition(pos);
	}

	//ボタン説明のテクスチャを一枚の中から切り取ります。
	//画像の一枚あたり元サイズ(ごめん未来の俺)
	const XMFLOAT2 kBasicButtonSize = { 512,256 };
	const XMFLOAT2 kVisibleButtonSize = { 128,64 };
	for (int i = 0; i < BUTTONCONTROLMAX; i++) {
		Sprite* con_vis_[BUTTONCONTROLMAX]{};
		con_vis_[i] = Sprite::Create(ImageManager::Con_vis, {0,0});
		button_hud[i].reset(con_vis_[i]);
		int num_index_y = i / BUTTONCONTROLMAX;
		int num_index_x = i % BUTTONCONTROLMAX;
		button_hud[i]->SetTextureRect(
			{ static_cast<float>(num_index_x) * kBasicButtonSize.x
			, static_cast<float>(num_index_y) * kBasicButtonSize.y },
			{ static_cast<float>(kBasicButtonSize.x), static_cast<float>(kBasicButtonSize.y) });
		button_hud[i]->SetSize(kVisibleButtonSize);
		button_hud[i]->SetAnchorPoint(kBasicAnchor);
	}
	//構造体の初期化
	button_system[GNOMESHOT].isvisible = false;
	button_system[GNOMESHOT].ease_frame = 0.0f;
	button_system[GNOMESHOT].ease_frame_max = 60.0f;
	button_system[GNOMESHOT].s_pos = { -64,520 };
	button_system[GNOMESHOT].e_pos = { 70,520 };

	button_system[GNOMERECOVERY].isvisible = false;
	button_system[GNOMERECOVERY].ease_frame = 0.0f;
	button_system[GNOMERECOVERY].ease_frame_max = 60.0f;
	button_system[GNOMERECOVERY].s_pos = { -64,600 };
	button_system[GNOMERECOVERY].e_pos = {  70,600 };
}

void Hud::Update() {
	//入力関連
	if (input->TiltPushStick(Input::R_RIGHT) || 
		input->TiltPushStick(Input::R_LEFT)  ||
		input->TriggerButton(Input::LT)) {
		explation_finish = true;

	}
	if (input->TriggerButton(Input::B)) {
		button_system[GNOMESHOT].isvisible = true;
	}

	if (input->TriggerButton(Input::A)) {
		button_system[GNOMERECOVERY].isvisible = true;
	}

	//コントローラー説明の更新
	CameraHudUpdate();
	//ボタンの操作説明の更新
	BottonHudUpdate();
}

void Hud::CameraHudUpdate() {
	switch_frame++;
	//アニメーション変更
	if (switch_frame == kSwitchFrameMax) {
		//左Triggerの表示アニメーション
		switch(visible_trigger){
		case TRIGGERNORMAL:	
			visible_trigger = TRIGGERPUSH;
			break;
		case TRIGGERPUSH:
			visible_trigger = TRIGGERNORMAL;
			break;
		}

		//右スティックの表示アニメーション
		switch (visible_stick) {
			//defaultで確認
		case STICKLEFT:
			old_visible_stick = visible_stick;
			visible_stick = STICK;
			break;
		case STICK:
			if (old_visible_stick== STICKLEFT) {
				visible_stick = STICKRIGHT;
			} else {
				visible_stick = STICKLEFT;
			}
			break;
		case STICKRIGHT:
			old_visible_stick = visible_stick;
			visible_stick = STICK;
			break;
		}
		//アニメーション変更処理終了
		switch_frame = 0;
	}
	//説明表示を端まで補間処理
	if (explation_finish) {
		if (edge_frame < 1.0f) {
			edge_frame += 1.0f/ kEdgeFrameMax;
		} else {
			edge_frame = 1.0f;
		}
		//端に行くまでに小さくします
		XMFLOAT2 siz{};
		siz.x = Ease(In, Quad, edge_frame, kVisibleBasicSize.x, kCameraHudSize2Edge.x);
		siz.y = Ease(In, Quad, edge_frame, kVisibleBasicSize.y, kCameraHudSize2Edge.y);
		for (int i = 0; i < CAMERACONTROLMAX; i++) {
			camera_hud[i]->SetSize(siz);
		}
		//端に向かいます。
		XMFLOAT2 pos = kCameraHudBase;
		for (int i = 0; i < CAMERACONTROLMAX; i++) {
			pos.x = Ease(In, Quad, edge_frame, kCameraHudBase.x, kCameraHudPos2Edge.x);
			pos.y = Ease(In, Quad, edge_frame, kCameraHudBase.y, kCameraHudPos2Edge.y);
			if (i < COMMENT) {
				pos.x -= camera_hud[i]->GetSize().x;
			} else if (i > COMMENT) {
				pos.x += camera_hud[i]->GetSize().x;
			}
			camera_hud[i]->SetPosition(pos);
		}
	}
}

void Hud::BottonHudUpdate() {
	for (int i = 0; i < BUTTONCONTROLMAX; i++) {
		if (button_system[i].isvisible) {
			XMFLOAT2 pos{};

			if (button_system[i].ease_frame < 1.0f) {
				button_system[i].ease_frame += 1.0f / button_system[i].ease_frame_max;
			} else {
				button_system[i].ease_frame = 1.0f;
			}

			pos.x = Ease(In, Quad,
				button_system[i].ease_frame,
				button_system[i].s_pos.x,
				button_system[i].e_pos.x);

			pos.y = Ease(In, Quad,
				button_system[i].ease_frame,
				button_system[i].s_pos.y,
				button_system[i].e_pos.y);

			button_hud[i]->SetPosition(pos);
		}
	}
}

void Hud::Draw() {
	Sprite::PreDraw();
	//カメラ回転用説明
	camera_hud[visible_trigger]->Draw();
	camera_hud[COMMENT]->Draw();
	camera_hud[visible_stick]->Draw();
	//各ボタンの説明
	for (int i = 0; i < BUTTONCONTROLMAX; i++) {
		if (button_system[i].isvisible) {
			button_hud[i]->Draw();
		}
	}

}

void Hud::Finalize() {
}
