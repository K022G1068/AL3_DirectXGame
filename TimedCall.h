#pragma once
#include <functional>
class TimedCall {
public:
	
	TimedCall(std::function<void()> callback, uint32_t time);
	//Update
	void Update();
	//Check if the count is over
	bool IsFinished() { return finished_; };

private:
	//Callback function
	std::function<void()> callback_;
	uint32_t time_;
	bool finished_ = false;
};

