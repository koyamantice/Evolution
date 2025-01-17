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
	void Initialize();
	/// <summary>
	/// 初期k処理
	/// </summary>
	void InitializeOver();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();
	enum ReverseType{
		NonReverse=1,
		Reverse=-1
	};
	/// <summary>
	/// シーン切り替え
	/// </summary>
	/// <param name="sceneName"></param>
	void ChangeScene(const std::string& sceneName,const ReverseType& _reverse= NonReverse);

	/// <summary>
	/// シーン切り替え
	/// </summary>
	/// <param name="sceneName"></param>
	void ChangeGameOver();




	/// <summary>
	/// イージングスタート
	/// </summary>
	void  ChangeStart() { easing_start = true; }
	const bool GetEasingStart() { return easing_start; }
private:
	std::list<std::unique_ptr<Sprite>> sprites;

	std::list<std::unique_ptr<Sprite>> over_sprites;

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