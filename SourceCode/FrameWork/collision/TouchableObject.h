#pragma once
#include "Object3d.h"
class TouchableObject :
	public Object3d
{
public:
	/// <summary>
	/// オブジェクト生成
	/// </summary>
	/// <returns></returns>
	static TouchableObject* Create(Model* model = nullptr);

	/// <summary>
	/// 初期化
	/// </summary>
	/// <returns>成否</returns>
	bool Initialize(Model * model);
	/// <summary>
	/// スケールの設定
	/// </summary>
	/// <param name="position">スケール</param>
	inline void SetScale(XMFLOAT3 scale) { this->scale = scale; }
	inline void SetRotation(XMFLOAT3 rotation) { this->rotation = rotation; }
	inline void SetPosition(XMFLOAT3 position) { this->position = position; }
};

