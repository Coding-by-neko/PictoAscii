#pragma once

#include <iostream>
#include <string>
#include "opencv2/opencv.hpp"
#include "symbolsdrawer.h"

using namespace std;
using namespace cv;

class converterascii
{	
	string ascii;
	int type;    
	symbolsdrawer drawer;  
	vector<Scalar> colors;  

	char ConvertMatrixToChar(Mat matrix);
	Mat AsciiStringToAsciiImage(string  str, int height, int width);
	string ConvertImageToString(Mat image);
	void DrawAsciiLine(Mat image, int j, string line);
	Mat ResizeImage(Mat image);

public:
	converterascii();
	Mat ConvertImageToAsciiImage(Mat image);
	string ConvertImageToAsciiString(Mat image);
	int GetMaxSymbolHeight();
	int GetMaxSymbolWidth();
	void SetFontScale(double scale);
};