#include <chrono>
#include <cmath>
#include <windows.h>
#include <iostream>

#pragma once
namespace CommonUtilities
{
	class Timer
	{
	public:
		Timer();
		Timer(const Timer& aTimer) = delete;
		Timer& operator=(const Timer& aTimer) = default;
		void Update();
		//int TimeToWaitForFixedFrameRate(const int aFrameRate);
		float GetDeltaTime() const;
		void SetDeltaTime(const float aDeltaTime);
		double GetTotalTime() const;
		void SetTotalTime(const double aTotalTime);

	private:
		float myDeltaTime;
		double myTotalTime;
		std::chrono::high_resolution_clock::time_point myTotalTimePoint;
		int myCurrentFrame;
	};
}