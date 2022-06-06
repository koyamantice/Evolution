#pragma once
#include "BaseScene.h"
#include "AbstractSceneFactory.h"
#include <memory> 

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
	void Finalize();

	bool IsEndRequst() { return endResquest_; }

	void SetEnd(bool endResquest_) { this->endResquest_ = endResquest_; }

public:
	//次シーン予約
	void ChangeScene(const std::string& sceneName);

	void SetSceneFactory(AbstractSceneFactory* sceneFactory) { sceneFactory_ = sceneFactory; }
private:
	bool endResquest_ = false;
	//今のシーン
	BaseScene* scene_ = nullptr;
	//次のシーン
	BaseScene* nextScene_ = nullptr;

	AbstractSceneFactory* sceneFactory_ = nullptr;

	~SceneManager()=default;
	SceneManager()=default;
	SceneManager(const SceneManager&)=delete;
	void operator =(const SceneManager&)=delete;
};

