#include "TimedInvoke.h"

TimedInvoke::TimedInvoke(std::function<bool()> _func, uint16_t _time) {
	//�����o�ϐ��ɂ��܂�
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
		//�R�[���o�b�N�֐��Ăяo��
		callback_();
	}

}
