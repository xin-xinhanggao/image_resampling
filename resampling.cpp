#include "resampling.h"

void resample(const Mat &origin, Mat &target, int trows, int tcols, int choice)
{
	if(choice == 0)
		nnresample(origin, target, trows, tcols);
	else if(choice == 1)
		biliresample(origin, target, trows, tcols);
	else if(choice == 2)
		bicubicresample(origin, target, trows, tcols);
	else if(choice == 3)
		lanczosresample(origin, target, trows, tcols);
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

void checkrgb(int &value)
{
	if(value < 0)
		value = 0;
	if(value > 255)
		value = 255;
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

	for(int row = 0; row < trows; row++)
		for(int col = 0; col < tcols; col++)
		{
			double rf = sr * row;
			double cf = sc * col;

			int r = floor(rf);
			int c = floor(cf);

			double dr = rf - r;
			double dc = cf - c;

			int rpixel = r;
			int cpixel = c;
			checkrange(rows, cols, rpixel, cpixel);

			double R = 0;
			double G = 0;
			double B = 0;

			R = double(origin.at<Vec3b>(rpixel, cpixel)[0]) * (1 - dr) * (1 - dc);
			G = double(origin.at<Vec3b>(rpixel, cpixel)[1]) * (1 - dr) * (1 - dc);
			B = double(origin.at<Vec3b>(rpixel, cpixel)[2]) * (1 - dr) * (1 - dc);

			rpixel = r + 1;
			cpixel = c;
			checkrange(rows, cols, rpixel, cpixel);

			R += double(origin.at<Vec3b>(rpixel, cpixel)[0]) * dr * (1 - dc);
			G += double(origin.at<Vec3b>(rpixel, cpixel)[1]) * dr * (1 - dc);
			B += double(origin.at<Vec3b>(rpixel, cpixel)[2]) * dr * (1 - dc);

			rpixel = r;
			cpixel = c + 1;
			checkrange(rows, cols, rpixel, cpixel);

			R += double(origin.at<Vec3b>(rpixel, cpixel)[0]) * (1 - dr) * dc;
			G += double(origin.at<Vec3b>(rpixel, cpixel)[1]) * (1 - dr) * dc;
			B += double(origin.at<Vec3b>(rpixel, cpixel)[2]) * (1 - dr) * dc;


			rpixel = r + 1;
			cpixel = c + 1;
			checkrange(rows, cols, rpixel, cpixel);

			R += double(origin.at<Vec3b>(rpixel, cpixel)[0]) * dr * dc;
			G += double(origin.at<Vec3b>(rpixel, cpixel)[1]) * dr * dc;
			B += double(origin.at<Vec3b>(rpixel, cpixel)[2]) * dr * dc;

			int rvalue = round(R);
			int gvalue = round(G);
			int bvalue = round(B);
			checkrgb(rvalue);
			checkrgb(gvalue);
			checkrgb(bvalue);

			target.at<Vec3b>(row, col) = Vec3b(rvalue, gvalue, bvalue);
		}
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

	for(int row = 0; row < trows; row++)
		for(int col = 0; col < tcols; col++)
		{
			double rf = sr * row;
			double cf = sc * col;

			int r = floor(rf);
			int c = floor(cf);

			double dr = rf - r; //b
			double dc = cf - c; //a

			double mcoe[4];
			mcoe[0] = -1 * dr * (1 - dr) * (1 - dr);
			mcoe[1] = 1 - 2 * dr * dr + dr * dr * dr;
			mcoe[2] = dr * (1 + dr - dr * dr);
			mcoe[3] = -1 * dr * dr * (1 - dr);

			double ncoe[4];
			ncoe[0] = -1 * dc * (1 - dc) * (1 - dc);
			ncoe[1] = 1 - 2 * dc * dc + dc * dc * dc;
			ncoe[2] = dc * (1 + dc - dc * dc);
			ncoe[3] = -1 * dc * dc * (1 - dc);

			double R = 0,G = 0,B = 0;
			for(int n = -1; n <= 2; n++)
				for(int m = -1; m <= 2; m++)
				{
					int rpixel = r + m;
					int cpixel = c + n;

					checkrange(rows, cols, rpixel, cpixel);

					double coe = mcoe[m + 1] * ncoe[n + 1];
					R += double(origin.at<Vec3b>(rpixel, cpixel)[0]) * coe;
					G += double(origin.at<Vec3b>(rpixel, cpixel)[1]) * coe;
					B += double(origin.at<Vec3b>(rpixel, cpixel)[2]) * coe;

				}
			int rvalue = floor(R);
			int gvalue = floor(G);
			int bvalue = floor(B);
			checkrgb(rvalue);
			checkrgb(gvalue);
			checkrgb(bvalue);

			target.at<Vec3b>(row, col) = Vec3b(rvalue, gvalue, bvalue);
		}
}

double sinc(double x)
{
	double Pi = 3.1415926;
	return sin(Pi * x) / (Pi * x);
}

double L(double x, int band)
{

	if(x >= band || -x > band)
		return 0;

	if(x < 0.0001 && x > -0.0001)
		return 1.0;

	
	return sinc(x) * sinc(x / band);
}

void lanczosresample(const Mat &origin, Mat &target, int trows, int tcols)
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

	int band = 8;
	for(int row = 0; row < trows; row++)
		for(int col = 0; col < tcols; col++)
		{
			double xs = sr * row;
			double ys = sc * col;

			int *x = new int[band * 2];
			int *y = new int[band * 2];
			Vec3f *I = new Vec3f[band * 2];

			I[0] = Vec3f(0,0,0);
			x[0] = floor(xs) - (band - 1);
			y[0] = floor(ys) - (band - 1);
			for(int i = 1; i < band * 2; i++)
			{
				I[i] = Vec3f(0, 0, 0);
				x[i] = x[0] + i;
				y[i] = y[0] + i;
			}

			for(int i = 0; i < band * 2; i++)
				checkrange(rows, cols, x[i], y[i]);

			double R = 0,G = 0,B = 0;

			double amout = 0;
			for(int k = 0; k < band * 2; k++)
				for(int i = 0; i < band * 2; i++)
				{
					double coe = L(xs - x[i], band) * L(ys - y[k], band);
					R += origin.at<Vec3b>(x[i], y[k])[0] * coe;
					G += origin.at<Vec3b>(x[i], y[k])[1] * coe;
					B += origin.at<Vec3b>(x[i], y[k])[2] * coe;
					amout += coe;
				}


			int rvalue = floor(R / amout);
			int gvalue = floor(G / amout);
			int bvalue = floor(B / amout);
			checkrgb(rvalue);
			checkrgb(gvalue);
			checkrgb(bvalue);

			target.at<Vec3b>(row, col) = Vec3b(rvalue, gvalue, bvalue);
		}
}