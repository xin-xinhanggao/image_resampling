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

	resample(origin, recover, rows, cols, choice);

	double psnr = 0.0;

	for(int row = 0; row < rows; row++)
		for(int col = 0; col < cols; col++)
			psnr += origin.at<Vec3b>(row, col).dot(recover.at<Vec3b>(row, col));

	psnr = psnr / rows / cols;
	psnr = 20 * log10(255 / sqrt(psnr));

	return psnr;
}