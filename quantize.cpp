#include <opencv2/core.hpp>
#include "opencv2/imgcodecs.hpp"
#include <opencv2/highgui.hpp>
#include <iostream>
#include <string>

using namespace std;
using namespace cv;

// Takes the reference of the input and output image and lookup table
void quantize(Mat &input, Mat &output, const Mat &table){
	CV_Assert(input.depth() == CV_8U);
	CV_Assert(output.depth() == CV_8U);

	int rows = input.rows;
	int cols = input.cols;
	int channels = input.channels();

	cols = cols * channels;

	// Go through each row in the image
	for (int x = 0; x < rows; x++){
		uchar *ptrIn = input.ptr<uchar>(x);
		uchar *ptrOut = output.ptr<uchar>(x);

		// For every channel in each pixel in the row, take its intensity and
		// quantize using the lookup table, take the result and place it in the
		// appropriate spot in the output image

		for (int y = 0; y < cols; y++){
			ptrOut[y] = table.at<uchar>(0, ptrIn[y]);
		}
	}
}

int main(int argc, char *argv[]){
	Mat frame, output;
	int divisor;

	string use_case = "Use case is ./a.out I/V (for image or video) path/to/source"
            " (or 0 if webcam) divisor \noutput_name (if using video file input or "
            "if you want image output)"
			"\nExample 1: ./a.out I pizza.png 40"
			"\nExample 2: ./a.out V teddy.mp4 100 teddy_out";

	// Error checking
	if (argc != 4 && argc != 5){
		cerr << use_case << endl;
		return -1;
	}

	// Test for wrong order of arguments
    try {
        divisor = stoi(argv[3]);
        if (divisor < 1) {
			cerr << "Error: Bad divisor!" << endl;
            cerr << use_case << endl;
            return -1;
        }
    } catch (const invalid_argument& e) {
        cerr << "Error: Bad divisor!" << endl;
		cerr << use_case << endl;
        return -1;
    }

	if (divisor < 1){
		cerr << "Error: Bad divisor!" << endl;
		return -1;
	}

	// Makes a table used to quantize color intensity by the given divisor
	Mat lookUpTable(1, 256, CV_8U);
	for (int i = 0; i < 256; i++){
		lookUpTable.at<uchar>(0, i) = (uchar)(divisor * (i / divisor));
	}


	if (strcmp(argv[1], "I") == 0){
		frame = imread(argv[2]);

		// Check for bad image
		if (frame.empty()){
			cerr << "Error: Bad File!" << endl;
			return -1;
		}

		// Make blank image for quantized pixels to be placed in
		output = Mat::zeros(frame.size(), frame.type());
		quantize(frame, output, lookUpTable);

		if (argc == 5){
			string output_name = string(argv[4]) + ".jpg";
			imwrite(output_name, output);
		}

		imshow("output", output);
		waitKey(0);

	} else if(strcmp(argv[1], "V") == 0 && strcmp(argv[2], "0") == 0){
		VideoCapture cap(0);

		if (!cap.isOpened()){
				cerr << "Error: Error opening camera!" << endl;
				return -1;
		}
		while (true){
			cap >> frame;
			if (frame.empty()){
				break;
			}

			output = Mat::zeros(frame.size(), frame.type());
			quantize(frame, output, lookUpTable);
			imshow("Output", output);
			if (waitKey(10) == 'q') {
				break;
			}
		}
		cap.release();
	} else if(strcmp(argv[1], "V") == 0){
		VideoCapture cap(argv[2]);
		//Check if the video file was opened successfully
		if (!cap.isOpened()){
						cerr << "Error: Error opening video file!" << endl;
						return -1;
		}
		// Set up video writer
		int width = static_cast<int>(cap.get(CAP_PROP_FRAME_WIDTH));
		int height = static_cast<int>(cap.get(CAP_PROP_FRAME_HEIGHT));
		double fps = cap.get(CAP_PROP_FPS);
		string output_name = (argc > 4) ? string(argv[4]) + ".avi" : "output.avi";
		VideoWriter video(output_name, VideoWriter::fourcc('M', 'J', 'P', 'G'), fps,
						Size(width, height));
		cout << "Processing video..." << endl;

		while (true){
			cap >> frame;
			if (frame.empty()){
				break;
			}
			output = Mat::zeros(frame.size(), frame.type());
			quantize(frame, output, lookUpTable);
			video.write(output);
		}

		cout << "Done!" << endl;
		cap.release();
	} else {
		cerr << use_case << endl;
		return -1;
	}
	destroyAllWindows();
}
