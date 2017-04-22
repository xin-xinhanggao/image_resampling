#include <opencv2/opencv.hpp>
using namespace cv;

void resample(const Mat &origin, Mat &target, int trows, int tcols, int choice);

void nnresample(const Mat &origin, Mat &target, int trows, int tcols);

void biliresample(const Mat &origin, Mat &target, int trows, int tcols);

void bicubicresample(const Mat &origin, Mat &target, int trows, int tcols);