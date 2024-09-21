#include <opencv2/core.hpp>
#include "opencv2/imgcodecs.hpp"
#include <opencv2/highgui.hpp>
#include <iostream>

using namespace std;
using namespace cv;

// Takes the reference of the input and output image and lookup table
void quantize(Mat& input, Mat& output, const Mat& table){
    CV_Assert(input.depth() == CV_8U);
    CV_Assert(output.depth() == CV_8U);

	int rows = input.rows;
	int cols = input.cols;
	int channels = input.channels();

	cols = cols * channels;

    // Go through each row in the image
	for (int x = 0; x < rows; x++){
		uchar* ptrIn  = input.ptr<uchar>(x);
		uchar* ptrOut = output.ptr<uchar>(x);

        // For every channel in each pixel in the row, take its intensity and
        // quantize using the lookup table, take the result and place it in the
        // appropriate spot in the output image
        
		for (int y = 0; y < cols; y++){
			ptrOut[y] = table.at<uchar>(0, ptrIn[y]);
		}
	}
}

int main(int argc, char* argv[]){
    Mat input_image, output_image;
    int divisor;

    // Error checking
	if (argc < 3){
		cout << "Use case is ./a.out image_file divisor" << endl;
		return -1;
	}

	divisor = stoi(argv[2]);
    if (divisor < 1){
        cout << "Bad divisor" << endl;
		return -1;
    }

	input_image = imread(argv[1]);
    if (input_image.empty()) {
        cout << "Error: Bad Image!" << endl;
        return -1;
    }
    
    // Set empty output to the same size and type as input
	output_image = Mat::zeros(input_image.size(), input_image.type());


    // Makes a table used to quantize color intensity by the given divisor
	Mat lookUpTable(1, 256, CV_8U);
	for (int i = 0; i < 256; i++){
		lookUpTable.at<uchar>(0, i) = (uchar)(divisor * (i / divisor));
	}

	quantize(input_image, output_image, lookUpTable);
    imshow("cool window", output_image);
    waitKey(0);
}
