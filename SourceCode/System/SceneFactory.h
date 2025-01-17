#pragma once
#include"AbstractSceneFactory.h"

//このゲーム用のシーン工場
class SceneFactory :
	public AbstractSceneFactory
{
public:
	///<summary>
	///シーン生成
	///</summary>
	/// <param name="SceneName">シーン名</param>
    ///<returns>生成したシーン</returns>
	std::unique_ptr<BaseScene> CreateScene(const std::string& sceneName) override;
};

