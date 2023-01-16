#pragma once
#include "Easing.h"
#include "Sprite.h"
#include <WinApp.h>

#include<vector>
#include <memory>
#include<string>
#include <list>

class SceneChanger
{
public:
	SceneChanger();
	~SceneChanger();

	/// <summary>
	/// 初期k処理
	/// </summary>
	void Init();
	/// <summary>
	/// 描画
	/// </summary>
	void Draw();
	/// <summary>
	/// シーン切り替え
	/// </summary>
	/// <param name="sceneName"></param>
	void ChangeScene(const std::string& sceneName);
	/// <summary>
	/// イージングスタート
	/// </summary>
	void  ChangeStart() { easing_start = true; }
	const bool& GetEasingStart() { return easing_start; }
private:
	std::list<std::unique_ptr<Sprite>> sprites;

	float width = WinApp::window_width;
	float height = WinApp::window_height;

	const int base_size = 80;
	int width_num  = (int)width / base_size;
	int height_num = (int)height / base_size;

	bool easing_start = false;
	float ease_frame = 0.0f;
	//何フレームで終わるか
	float end_frame = 100.0f;
};