#include "TimedInvoke.h"

TimedInvoke::TimedInvoke(std::function<bool()> _func, uint16_t _time) {
	//メンバ変数にしまう
	callback_ = _func;
	time_ = _time;
}

void TimedInvoke::Update() {

	if (isFinish) {
		return;
	}
	time_--;
	if (time_<=0) {
		isFinish = true;
		//コールバック関数呼び出し
		callback_();
	}

}
