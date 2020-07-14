#pragma once

#include <opencv2/opencv.hpp>
#include <fstream>
#include <iostream>
#include <cmath>
#include <thread>

class m_tym {
private:
	double st_tym = 0;
	double en_tym = 0;
	const char* name;
public:
	m_tym(const char* nm) : name(nm) {
		start();
	}

	~m_tym() {
		stop();
		printTime();
	}

	void start() {
		st_tym = cv::getTickCount();
	}

	void stop() {
		en_tym = cv::getTickCount();
	}

	void printTime() {
		std::cout << "\n[Block "<< name << "] Time elapsed: "<< (en_tym - st_tym) / cv::getTickFrequency() <<" seconds\n";
	}
};

