#include "Timer.h"

namespace CommonUtilities
{
	Timer::Timer()
	{
		myDeltaTime = 0;
		myTotalTime = 0;
		myCurrentFrame = 0;
		myTotalTimePoint = std::chrono::high_resolution_clock::now();
	}

	void Timer::Update()
	{
			std::chrono::high_resolution_clock::time_point time = std::chrono::high_resolution_clock::now();
			std::chrono::duration<double> difference = time - myTotalTimePoint;
			double deltaTime = difference.count();
			myTotalTime += deltaTime;
			myDeltaTime = static_cast<float>(deltaTime);
			myTotalTimePoint = time;	
			//std::cout << "deltaTime: " << myDeltaTime << "\ttotalTime: " << myTotalTime << std::endl;
	}

	//int Timer::TimeToWaitForFixedFrameRate(const int aFrameRate)
	//{
	//	double timeUntilNextFrame;
	//	bool countTimeNormally = true;
	//	if (myCurrentFrame >= aFrameRate)
	//	{
	//		myCurrentFrame = 0;
	//		if (fmod(myTotalTime, 1.0) > 0.2)
	//		{
	//			timeUntilNextFrame = 1.0 - fmod(myTotalTime, 1.0);
	//			countTimeNormally = false;
	//		}
	//	}
	//	if (countTimeNormally)
	//	{
	//		timeUntilNextFrame = (1.0 - fmod(myTotalTime, 1.0)) / (aFrameRate - myCurrentFrame);
	//	}
	//	myCurrentFrame++;
	//	timeUntilNextFrame *= 1000;
	//	return static_cast<int>(timeUntilNextFrame);
	//}

	float Timer::GetDeltaTime() const
	{
		return myDeltaTime;
	}

	void Timer::SetDeltaTime(const float aDeltaTime)
	{
		myDeltaTime = aDeltaTime;
	}

	double Timer::GetTotalTime() const
	{
		return myTotalTime;
	}

	void Timer::SetTotalTime(const double aTotalTime)
	{
		myTotalTime = aTotalTime;
	}
}