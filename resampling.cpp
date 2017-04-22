#include "resampling.h"

void resample(const Mat &origin, Mat &target, int trows, int tcols, int choice)
{
	if(choice == 0)
		nnresample(origin, target, trows, tcols);
}

void nnresample(const Mat &origin, Mat &target, int trows, int tcols)
{
	int rows = origin.rows;
	int cols = origin.cols;

	//scale coefficient
	double sr,sc;

	if(rows > trows)
		sr = double(rows) / trows;
	else
		sr = double(rows - 1) / trows;

	if(cols > tcols)
		sc = double(cols) / tcols;
	else
		sc = double(cols - 1) / tcols;

	target = Mat(trows, tcols, CV_8UC3);

	for(int row = 0; row < trows; row++)
		for(int col = 0; col < tcols; col++)
		{
			double rf = sr * row;
			double cf = sc * col;

			target.at<Vec3b>(row, col) = origin.at<Vec3b>(round(rf), round(cf));
		}
}