#pragma once
#include "SceneManager.h"
#include "Actor.h"
#include"TitleText.h"

/// <summary>
/// �^�C�g���V�[��
/// </summary>

class TitleScene : public BaseScene {
public:

	/// <summary>
	/// ������
	/// </summary>
	void Initialize(DirectXCommon* dxCommon) override;
	/// <summary>
	/// �I��
	/// </summary>
	void Finalize() override;

	/// <summary>
	/// ���t���[���X�V
	/// </summary>
	void Update(DirectXCommon* dxCommon) override;
	/// <summary>
	/// �`��
	/// </summary>
	void Draw(DirectXCommon* dxCommon) override;

private:

	void Feed(const std::string& sceneName);

	//�����o�ϐ�
	enum {
		BackScene,
		Button,
		moveTitle,
		moveDebug,
		SpriteMax,
	};
	std::unique_ptr<Sprite> UI[SpriteMax];
	std::unique_ptr<Sprite> Effect;
	std::unique_ptr<TitleText> text;
	bool Change = false;
	float alpha = 0.0f;
	float frame = 0;

	enum Portal {
		Title=0,
		Debug,

	};

	int nextScene = Portal::Title;

	XMFLOAT2 SetPos[3]={ { 640.0f,300.0f } ,{ 640.0f,360.0f } ,{ 640.0f,460.0f } };
	
	float moveText[2]{};
	int moveTitleNow = 1;
	int moveDebugNow = 2;

	std::string SceneName = "null";
};

