#include <iostream>
#include <stdio.h>
#include <opencv2/opencv.hpp>

using namespace cv;

using namespace std;

bool compareContourAreas( vector<Point>, vector<Point>);

int main(int argc, char** argv )
{
    if ( argc != 2 )
    {
        printf("usage: DisplayImage.out <Image_Path>\n");
        return -1;
    }
    Mat image;
    image = imread( argv[1]);
    if ( !image.data )
    {
        printf("No image data \n");
        return -1;
    }

    int scale = 1;
    int ddepth = CV_16S;
    int delta = 0;
    Size ksize;
    ksize = Size(9,9);
    Mat gray;
    Mat gradientX;
    Mat absGradientX;
    Mat gradientY;
    Mat absGradientY;
    Mat gradient;
    Mat blurred;
    Mat thresholded;
    Mat kernel;
    Mat closed;
    Mat eroded;
    Mat dilated;
    vector<vector<Point> > contours;

    cvtColor(image, gray, CV_BGR2GRAY);

    Sobel( gray, gradientX, ddepth, 1, 0, 3, scale, delta, BORDER_DEFAULT );
    convertScaleAbs( gradientX, absGradientX );
    Sobel( gray, gradientY, ddepth, 0, 1, 3, scale, delta, BORDER_DEFAULT );
    convertScaleAbs( gradientY, absGradientY );

    subtract(absGradientX, absGradientY, gradient);
    
    blur(gradient, blurred, ksize);

    threshold(blurred, thresholded, 120, 255, THRESH_BINARY);

    kernel = getStructuringElement(MORPH_RECT, Size(21,7));

    morphologyEx(thresholded, closed, MORPH_CLOSE, kernel);

    erode(closed, eroded, Mat(), Point(-1, -1), 3);

    dilate(eroded, dilated, Mat(), Point(-1, -1), 7);

    findContours(dilated, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

    sort(contours.begin(), contours.end(), compareContourAreas);

    vector<vector<Point> > contour;

    RotatedRect contourRect;
    contourRect = minAreaRect(contours[contours.size()-1]);

    /*
     * Contour is the biggest contour 
     * of all the contours and we assume that
     * it surrounds the bar code
     */
    contour.push_back(contours[contours.size()-1]);

    //drawContours(image, contours, -1, Scalar(0, 255, 0), 3);
    Point2f vertices[4];
    contourRect.points(vertices);

    for (int i = 0; i < 4; i++) {
        line(image, vertices[i], vertices[(i+1)%4], Scalar(0,255,0));
    }

    namedWindow("Display Image", WINDOW_AUTOSIZE );
    imshow("Display Image", image);

    waitKey(0);
    return 0;
}

bool compareContourAreas ( vector<Point> contour1, vector<Point> contour2 ) {
        double i = fabs( contourArea(Mat(contour1)) );
            double j = fabs( contourArea(Mat(contour2)) );
                return ( i < j );
}
