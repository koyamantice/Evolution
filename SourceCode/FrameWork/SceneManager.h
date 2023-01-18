#pragma once

#include <memory> 

#include "BaseScene.h"
#include "BattleScene.h"

#include "AbstractSceneFactory.h"

/// <summary>
/// シーンマネージャー
/// </summary>
class SceneManager {
public:
	static SceneManager* GetInstance();
	/// <summary>
	/// 更新
	/// </summary>
	void Update(DirectXCommon* dxCommon);
	/// <summary>
	/// 描画
	/// </summary>
	void Draw(DirectXCommon* dxCommon);
	/// <summary>
	/// 終了
	/// </summary>
	void Finalize();

	//シーン終了リクエスト
	const bool& IsEndRequst() { return endResquest_; }
	void SetEnd(const bool& endResquest_) { this->endResquest_ = endResquest_; }

public:
	//次シーンに変更します。
	void ChangeScene(const std::string& sceneName);
	//シーンファクトリーをセットする
	void SetSceneFactory(AbstractSceneFactory* sceneFactory) { sceneFactory_ = sceneFactory; }

private:

	//シーン終了
	bool endResquest_ = false;
	//今のシーン
	BaseScene* scene_ = nullptr;
	//次のシーン
	BaseScene* nextScene_ = nullptr;
	//シーンファクトリー
	AbstractSceneFactory* sceneFactory_ = nullptr;

private:
	~SceneManager() = default;
	SceneManager() = default;
	SceneManager(const SceneManager&) = delete;
	void operator =(const SceneManager&) = delete;
};

