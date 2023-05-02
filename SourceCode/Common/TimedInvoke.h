#pragma once

#include <functional>
/// <summary>
/// 時限発動
/// </summary>
class TimedInvoke {
public: 
	//コンストラクタ
	TimedInvoke(std::function<void(void)> _func,uint16_t _time);
	~TimedInvoke() {};
	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	//完了フラグ
	const bool GetIsFinish() { return isFinish; }

private:
	//コールバック
	std::function<void()> callback_;
	//残り時間
	uint16_t time_;
	//完了フラグ
	bool isFinish = false;
};

