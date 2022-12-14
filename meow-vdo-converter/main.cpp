#include "opencv2\opencv.hpp"
#include <fstream>
#include "converterascii.h"
using namespace cv;
using namespace std;

void PrintHelp()
{
    cout << "Usage: VideoConverter <input_file_name> <font_scale = 0.2>" << endl;
}

int main(int argc, char**argv)
{
   
    if (argc < 2 || argc > 3)
    {
        PrintHelp();
        return 0;
    }


    

    double scale;
    char *ptr;
    if (argc == 3)
    {
        try
        {
            scale = strtod(argv[2], &ptr);
        }
        catch (...)
        {
            cout << "Error: scale is not correct" << endl;
            PrintHelp();
            return 1;
        }
        if (*ptr != 0 || scale < 0)
        {
            cout << "Error: scale is not correct" << endl;
            PrintHelp();
            return 1;
        }

        if (scale > 2)
        {
            cout << "Error: scale is too big. Scale must be less than 2" << endl;
            PrintHelp();
            return 1;

        }
    }
    else
    {
        scale = 0.2;
    }
   
    cout << "Font scale = " << scale << endl;

    converterascii converter;
    converter.SetFontScale(scale);

    string input = argv[1];
    cout << input << endl;
    string extension = input.substr(input.find_first_of("."));
    VideoCapture inputVideo(input); 
    if (!inputVideo.isOpened() || extension == ".jpg")
    {
        Mat image;
        image = imread(input, CV_LOAD_IMAGE_COLOR);
        if (!image.data)
        {
            cout << "Error: Can not open input video or image" << endl;
            return -1;
        }

        Mat ascii;
        cout << "Rendering..." << endl;
        ascii = converter.ConvertImageToAsciiImage(image);
        string outImageName = "out" + extension;
        imwrite(outImageName, ascii);
        cout << "File save as " << outImageName << endl;
        return 0;
    }

    


    string name = "out" + extension;
    int ex = static_cast<int>(inputVideo.get(CV_CAP_PROP_FOURCC));     

    
    char EXT[] = { (char)(ex & 0XFF), (char)((ex & 0XFF00) >> 8), (char)((ex & 0XFF0000) >> 16), (char)((ex & 0XFF000000) >> 24), 0 };


    int h = inputVideo.get(CV_CAP_PROP_FRAME_HEIGHT);
    int w = inputVideo.get(CV_CAP_PROP_FRAME_WIDTH);
    h = h + (converter.GetMaxSymbolHeight() - h % converter.GetMaxSymbolHeight());  
    w = w + (converter.GetMaxSymbolWidth() - w % converter.GetMaxSymbolWidth());

    VideoWriter outputVideo;    
    outputVideo.open(name, ex, inputVideo.get(CV_CAP_PROP_FPS), Size(w, h), true);

    if (!outputVideo.isOpened())
    {
        cout << "Unknown codec" << endl;
        outputVideo.open(name, ex = -1, inputVideo.get(CV_CAP_PROP_FPS), Size(w, h), true);
        if (!outputVideo.isOpened())
        {
            cout << "Could not open the output video for write: " << endl;
            return -1;
        }

    }
    
    cout << "Input frame resolution: Width=" << w << "  Height=" << h
        << " number of frames: " << inputVideo.get(CV_CAP_PROP_FRAME_COUNT) << endl;
    cout << "Input codec type: " << EXT << endl;



    Mat asciiFrame;
    long long frames = 1;
    int oldPercent = 0;
    
    cout << "Start rendering" << endl;
    for (;;)
    {
        Mat frame;
        inputVideo >> frame; 
        if (frame.empty()) break;  
        asciiFrame = converter.ConvertImageToAsciiImage(frame);
        outputVideo << asciiFrame;
        double percent = frames / (double)inputVideo.get(CV_CAP_PROP_FRAME_COUNT)  * 100;

        if (percent - oldPercent > 1)
        {
            oldPercent = std::floor(percent);
            cout << oldPercent << " % rendered..." << endl;
        }
        frames++;
    }
    cout << "All 100% rendered" << endl;
    cout << "End rendering" << endl;
    cout << "File save as " << name << endl;
    return 0;
}