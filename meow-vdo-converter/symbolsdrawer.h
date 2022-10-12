#pragma once;

#include <iostream>
#include <string>
#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;

class symbolsdrawer
{
	string ascii;  
	int fontFace;  
	double fontScale;  
	int thickness;   
	int type;      
	vector<Mat> symbolsImages;  

	void SetMaxSymbolWidth();
	void SetMaxSymbolHeight();
	vector<Mat> SymbolsImages();
	Mat GetSymbolImage(string str, Scalar color);
public:
	int maxSymbolWidth;
	int maxSymbolHeight;
	symbolsdrawer(string ascii = "");
	void SetFontScale(double scale);
	void DrawAsciiLine(Mat image, int i, string line, vector<Scalar> lineColors);
};