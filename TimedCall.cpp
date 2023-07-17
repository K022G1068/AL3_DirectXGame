#include "TimedCall.h"
TimedCall::TimedCall(std::function<void()> callback, uint32_t time) { 
	callback_ = callback;
	time_ = time;

}

void TimedCall::Update() 
{ 
	if (finished_)
	{
		return;
	}
	time_--;
	if (time_ <= 0)
	{
		finished_ = true;
		callback_();
	}
}
