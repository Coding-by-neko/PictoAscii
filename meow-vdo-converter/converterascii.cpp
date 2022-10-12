#include "converterascii.h"
#include "Common.h"


converterascii::converterascii()
{
	ascii = " .,-:;i1tfLCG08@";
	type = 16;
	drawer = symbolsdrawer(ascii);
}


Mat converterascii::ResizeImage(Mat image)
{
	int newImageHeight = image.rows + (drawer.maxSymbolHeight - image.rows % drawer.maxSymbolHeight); 
	int newImageWidth = image.cols + (drawer.maxSymbolWidth - image.cols % drawer.maxSymbolWidth);
	resize(image, image, Size(newImageWidth, newImageHeight));
	return image;
}

Scalar CharColor(Mat matrix)
{
	int red = 0;
	int green = 0;
	int blue = 0;
	for (int i = 0; i < matrix.rows; i++)
	{
		for (int j = 0; j < matrix.cols; j++)
		{
			Vec3b pixel = GetBGRPixel(matrix, i, j);
			blue += pixel[0];
			green += pixel[1];
			red += pixel[2];
			
		}
	}

	red /= matrix.rows * matrix.cols;
	green /= matrix.rows * matrix.cols;
	blue /= matrix.rows * matrix.cols;



	return Scalar(blue, green, red);


}

string converterascii::ConvertImageToString(Mat image)
{
	int verticalStep = drawer.maxSymbolHeight;
	int horizontalStep = drawer.maxSymbolWidth;
	string symbol;
	string resultString = "";
	Scalar color;

	for (int i = 0; i < image.rows; i += verticalStep)
	{
		string line = "";
		for (int j = 0; j < image.cols; j += horizontalStep)
		{
			Mat tmp = image(Rect(j, i, horizontalStep, verticalStep));
			Mat subm(tmp);
			symbol = ConvertMatrixToChar(subm);
			line += symbol;
			color = CharColor(subm);
			colors.push_back(color);
			
		}
		line += "\n";
		resultString += line;
	}
	return resultString;
}

Mat converterascii::AsciiStringToAsciiImage(string  str, int height, int width)
{
	int start = 0;
	int index = 0;
	int currentY = 0;
	int colorIndex = 0;
	Point org;
	Scalar color;
	Mat res(height, width, 16);
	while ((index = str.find('\n', start)) != -1)
	{
		string line = str.substr(start, index - start);
		start = index + 1;
		drawer.DrawAsciiLine(res, currentY, line, vector<Scalar>(colors.begin() + colorIndex, colors.begin() + colorIndex + line.length()));
		currentY += drawer.maxSymbolHeight;
		colorIndex+=line.length();
	}
	return res;
}

Mat converterascii::ConvertImageToAsciiImage(Mat image)
{
	colors.clear();
	image = ResizeImage(image);
	return AsciiStringToAsciiImage(ConvertImageToString(image), image.rows, image.cols);  
}

string converterascii::ConvertImageToAsciiString(Mat image)
{
	colors.clear();
	image = ResizeImage(image);
	return ConvertImageToString(image);
}

char converterascii::ConvertMatrixToChar(Mat matrix)
{
	double bright = 0;
	for (int i = 0; i < matrix.rows; i++)
	{
		for (int j = 0; j < matrix.cols; j++)
		{
			Vec3b pixel = GetBGRPixel(matrix, i, j);
			bright += GetBrightness(pixel);
		}
	}


	bright /= (matrix.rows * matrix.cols);

	int index = bright / ascii.length();
	return ascii[index];
}

int converterascii::GetMaxSymbolHeight()
{
	return drawer.maxSymbolHeight;
}

int converterascii::GetMaxSymbolWidth()
{
	return drawer.maxSymbolWidth;
}

void converterascii::SetFontScale(double scale)
{
	drawer.SetFontScale(scale);
}