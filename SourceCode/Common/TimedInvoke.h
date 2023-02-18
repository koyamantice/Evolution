#pragma once

#include <functional>
/// <summary>
/// ��������
/// </summary>
class TimedInvoke {
public: 
	//�R���X�g���N�^
	TimedInvoke(std::function<bool()> _func,uint16_t _time);
	/// <summary>
	/// �X�V
	/// </summary>
	void Update();
	//�����t���O
	const bool& GetIsFinish() { return isFinish; }

private:
	//�R�[���o�b�N
	std::function<bool()> callback_;
	//�c�莞��
	uint16_t time_;
	//�����t���O
	bool isFinish = false;


};

