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
			output.at<Vec3b>(i, j) = input.at<Vec3b>(i/heightFactor, j/widthFactor);
		}
	}
	return output;
}

Vec3b average(Vec3b* pixels, int area){
	int blue = 0;
	int green = 0;
	int red = 0;

    for (int i = 0; i < area; ++i) {
        blue += pixels[i][0];
        green += pixels[i][1];
        red += pixels[i][2];
    }

	return Vec3b(blue/area, green/area, red/area);
}


Mat resizeSmaller(Mat &input, int widthFactor, int heightFactor){
	int height = input.rows;
	int width = input.cols;

	int rows = height / heightFactor;
	int cols = width / widthFactor;

	int area = widthFactor * heightFactor;
	Vec3b pixels [area];
	int window[area][2];

	Mat output = Mat::zeros(Size(cols, rows), input.type());

	char x = 0;
	char y = 0;

	for (int i = 0; i < rows; i++){
		for (int j = 0; j < cols; j++){
			for(int i = 0; i < area; i++){
				x = window[area][0];
				y = window[area][1];
				pixels[i] = input.at<Vec3b>(i + x, j + y);
				//cout << "ij " << i << " " << j << endl;
				//cout << "xy " << x << " " << y << endl;
			}
			output.at<Vec3b>(i, j) = average(pixels, area);
		}
	}

	return output;
}

int main (){
	Mat in = imread("mongol.jpg");
	imshow("in", in);
	waitKey(0);
	try
	{
		imshow("out", resizeSmaller(in, 2, 2));
		waitKey(0);
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
	
}