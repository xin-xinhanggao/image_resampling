#include <opencv2/opencv.hpp>
#include "resampling.h"
#include <cmath>
#include <iostream>

using namespace cv;

double PSNR(Mat &origin, Mat &target, int choice)
{
	Mat recover;
	int rows = origin.rows;
	int cols = origin.cols;

	resample(target, recover, rows, cols, choice);

	double psnr = 0.0;

	for(int row = 0; row < rows; row++)
		for(int col = 0; col < cols; col++)
		{

			double delr = int(origin.at<Vec3b>(row, col)[0]) - int(recover.at<Vec3b>(row, col)[0]);
			double delg = int(origin.at<Vec3b>(row, col)[1]) - int(recover.at<Vec3b>(row, col)[1]);
			double delb = int(origin.at<Vec3b>(row, col)[2]) - int(recover.at<Vec3b>(row, col)[2]);

			psnr += delr * delr + delg * delg + delb * delb;
		}

	psnr = psnr / rows / cols / 3;
	psnr = 20 * log10(255 / sqrt(psnr));

	return psnr;
}