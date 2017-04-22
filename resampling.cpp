#include "resampling.h"

void resample(const Mat &origin, Mat &target, int trows, int tcols, int choice)
{
	if(choice == 0)
		nnresample(origin, target, trows, tcols);
	else if(choice == 1)
		biliresample(origin, target, trows, tcols);
	else if(choice == 2)
		bicubicresample(origin, target, trows, tcols);
}

void checkrange(int rows, int cols, int &r, int &c)
{
	if(r < 0)
	{
		r = 0;
	}
	if(c < 0)
	{
		c = 0;
	}
	if(r >= rows)
	{
		r = rows - 1;
	}
	if(c >= cols)
	{
		c = cols - 1;
	}
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

	for(int row = 1; row <= trows; row++)
		for(int col = 1; col <= tcols; col++)
		{
			double rf = sr * row;
			double cf = sc * col;

			int rpixel = round(rf) - 1;
			int cpixel = round(cf) - 1;
			checkrange(rows, cols, rpixel, cpixel);

			target.at<Vec3b>(row - 1, col - 1) = origin.at<Vec3b>(rpixel, cpixel);
		}
}

void biliresample(const Mat &origin, Mat &target, int trows, int tcols)
{
	int rows = origin.rows;
	int cols = origin.cols;

	//scale coefficient
	double sr,sc;
	sr = double(rows) / trows;
	sc = double(cols) / tcols;

	target = Mat(trows, tcols, CV_8UC3);

	for(int row = 1; row <= trows; row++)
		for(int col = 1; col <= tcols; col++)
		{
			double rf = sr * row;
			double cf = sc * col;

			int r = floor(rf);
			int c = floor(cf);

			double dr = rf - r;
			double dc = cf - c;

			int rpixel = r - 1;
			int cpixel = c - 1;
			checkrange(rows, cols, rpixel, cpixel);

			target.at<Vec3b>(row - 1, col - 1) = origin.at<Vec3b>(rpixel, cpixel) * (1 - dr) * (1 - dc);

			rpixel = r;
			cpixel = c - 1;
			checkrange(rows, cols, rpixel, cpixel);

			target.at<Vec3b>(row - 1, col - 1) += origin.at<Vec3b>(rpixel, cpixel) * dr * (1 - dc);

			rpixel = r - 1;
			cpixel = c;
			checkrange(rows, cols, rpixel, cpixel);

			target.at<Vec3b>(row - 1, col - 1) += origin.at<Vec3b>(rpixel, cpixel) * (1 - dr) * dc;

			rpixel = r;
			cpixel = c;
			checkrange(rows, cols, rpixel, cpixel);

			target.at<Vec3b>(row - 1, col - 1) += origin.at<Vec3b>(rpixel, cpixel) * dr * dc; 
		}
}

double Q(double x)
{
	if(x > 0)
		return x;
	return 0;
}

double P(double x)
{
	return (pow(Q(x + 2), 3) - 4 * pow(Q(x + 1), 3) - 6 * pow(Q(x), 3) - 4 * pow(Q(x - 1), 3)) / 6.0;
}

void bicubicresample(const Mat &origin, Mat &target, int trows, int tcols)
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

	for(int row = 1; row <= trows; row++)
		for(int col = 1; col <= tcols; col++)
		{
			double rf = sr * row;
			double cf = sc * col;

			int r = floor(rf);
			int c = floor(cf);

			double dr = rf - r;
			double dc = cf - c;

			target.at<Vec3b>(row - 1, col - 1) = Vec3b(0.0,0.0,0.0);
			
			double amount = 0.0;

			for(int m = -1; m <= 2; m++)
				for(int n = -1; n <= 2; n++)
				{
					//int rpixel = r + m - 1;
					//int cpixel = c + n - 1;
					//checkrange(rows, cols, rpixel, cpixel, count);

					int rpixel = r;
					int cpixel = c;
					checkrange(rows, cols, rpixel, cpixel);

					amount += P(dr - m) * P(n - dc);
					target.at<Vec3b>(row - 1, col - 1) += origin.at<Vec3b>(rpixel, cpixel) * P(dr - m) * P(n - dc);
					//target.at<Vec3b>(row - 1, col - 1) += origin.at<Vec3b>(rpixel, cpixel) * P(dr - m) * P(n - dc);
				}
			std::cout<<amount<<std::endl;
		}
}