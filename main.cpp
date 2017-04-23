#include "resampling.h"
#include "psnr.h"
#include <opencv2/opencv.hpp>
#include <iostream>

int main(int argc, char **argv)
{
	if(argc != 5)
	{
		std::cout<<"*-------------------*"<<std::endl;
		std::cout<<"usage: ./resample origin_img target_height target_width algorithm_choice"<<std::endl;
		std::cout<<"algorithm_choice: 0<-->nn  1<-->bili  2<-->bicubic 3<-->Lanczos"<<std::endl;
		std::cout<<"*-------------------*"<<std::endl;
		return 0;
	}
	cv::Mat origin = cv::imread(argv[1]);

	int trows = atoi(argv[2]);

	int tcols = atoi(argv[3]);

	int choice = atoi(argv[4]);

	cv::Mat target;

	resample(origin, target, trows, tcols, choice);

	cv::imwrite("target.png", target);

	std::cout<<"PSNR: "<<PSNR(origin, target, choice)<<std::endl;

	return 0;
}