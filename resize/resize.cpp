#include <opencv2/core.hpp>
#include "opencv2/imgcodecs.hpp"
#include <opencv2/highgui.hpp>
#include <iostream>
#include <string>

using namespace std;
using namespace cv;

Mat resizeLarger(Mat &input, int widthFactor, int heightFactor){
	int height = input.rows;
	int width = input.cols;

	int rows = height * heightFactor;
	int cols = width * widthFactor;

	Mat output = Mat::zeros(Size(cols, rows), input.type());

	for (int i = 0; i < rows; i++){
		for (int j = 0; j < cols; j++){
			output.ptr<uchar>(i)[j] = input.ptr<uchar>(i/heightFactor)[j/widthFactor];
		}
	}
	return output;
}