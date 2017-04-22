#include "resampling.h"
#include "psnr.h"
#include <opencv2/opencv.hpp>
#include <iostream>

int main(int argc, char **argv)
{
	cv::Mat origin = cv::imread(argv[1]);

	int trows = atoi(argv[2]);

	int tcols = atoi(argv[3]);

	int choice = atoi(argv[4]);

	cv::Mat target;

	resample(origin, target, trows, tcols, choice);

	cv::imwrite("target.png", target);

	std::cout<<PSNR(origin, target, choice)<<std::endl;

	return 0;
}