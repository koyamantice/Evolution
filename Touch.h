#pragma once
#include "Object2d.h"



class Touch {
public:
	Touch();
	~Touch();


private:
	std::unique_ptr<Object2d> fire;


};
