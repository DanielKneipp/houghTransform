#include <iostream>
#include <opencv2/highgui/highgui.hpp>

#include "houghtransform.h"

#define _COLS 100
#define _ROWS 100
#define _SIZE 30

#define _MAIN_WINDOW_TITLE "Image Test"

using namespace std;

void drawSquare( cv::Mat& img )
{
    for( int i = round( _SIZE / 10 ); i < img.rows - round( _SIZE / 10 ); i++ )
    {
        img.at< uchar >( i, i ) = 255;
    }
}

int main()
{
    cv::Mat imgTest( _SIZE, _SIZE, CV_8U, cv::Scalar::all( 0 ) );
    HoughTransform HT( imgTest.size() );

    drawSquare( imgTest );
    HT.compute( imgTest );
    HT.getBestParamsRT( 4 );
    cv::Mat HSimg = HT.getHoughSpaceImage();

    cv::namedWindow( _MAIN_WINDOW_TITLE );
    cv::imshow( _MAIN_WINDOW_TITLE, imgTest );

    cv::imshow( "Hough Space", HSimg );

    cv::waitKey( 0 );

    return 0;
}

