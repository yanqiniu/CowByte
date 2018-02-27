//
//  Timer.h
//  password-mac
//
//  Simple wrapper for std::chrono timer
//
//  Created by Sanjay Madhav on 12/26/14.
//  Copyright (c) 2014 Sanjay Madhav. All rights reserved.
//

#pragma once

#include <chrono>

class Timer
{
public:
	// Construct the timer
	Timer();
	
	// Start the timer
	void start();
	
	// Get the amount of elapsed time, in seconds since start was called
	double getElapsed();
	
private:
	std::chrono::high_resolution_clock::time_point mStart;
};

