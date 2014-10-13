#include <iostream>
#include <opencv2/highgui/highgui.hpp>

#include "houghtransform.h"

#define _COLS 100
#define _ROWS 100
#define _SIZE 50

#define _MAIN_WINDOW_TITLE "Image Test"

using namespace std;

void drawMainDiagonal( cv::Mat& img )
{
    float ratio =  (float) _COLS / (float) _ROWS;
    for( int i = round( (float) _ROWS / 10.0 ) - 1; i < img.rows - round( (float) _ROWS / 10.0 ); i++ )
    {
        img.at< uchar >( i, round( (float) i * ratio ) ) = 255;
    }
}

void drawSquare( cv::Mat& img )
{
    int colMin = round( (float) _COLS / 10.0 ), rowMin = round( (float) _ROWS / 10.0 );

    // Linhas de cima e de baixo
    for( int i = colMin - 1; i < img.cols - colMin; i++ )
    {
        img.at< uchar >( rowMin, i ) = 255;
        img.at< uchar >( img.rows - rowMin, i ) = 255;
    }

    // Colunas da esquerda e direita
    for( int i = rowMin - 1; i < img.rows - rowMin; i++ )
    {
        img.at< uchar >( i, colMin ) = 255;
        img.at< uchar >( i, img.cols - colMin ) = 255;
    }
}

// Errors nos extremos inferior e direito.
void drawCircle( cv::Mat& img )
{
    int colMin = round( (float) _COLS / 10.0 ), rowMin = round( (float) _ROWS / 10.0 );
    int a = round( (float) _ROWS / 2.0 );
    int b = round( (float) _COLS / 2.0 );
    int r = round( (float) ( ( _COLS - colMin ) - colMin) / 2.0 );

    for( int i = rowMin ; i < img.rows - rowMin; i++ )
    {
        for( int j = colMin ; j < img.cols - colMin; j++ )
        {
            if(  pow( i - a, 2 )  + pow( j - b, 2 ) >=  pow( r, 2 ) - 70 &&
                 pow( i - a, 2 )  + pow( j - b, 2 ) <=  pow( r, 2 ) + 70 )
            {
                img.at< uchar >( i, j ) = 255;
            }
        }
    }
}

inline int calcPixel( const cv::Mat src, const short kernel[ 3 ][ 3 ], int row, int col)
{
    return ( ( src.at< uchar >( row-1, col-1 ) * kernel[ 0 ][ 0 ] )
           + ( src.at< uchar >( row-1, col ) * kernel[ 0 ][ 1 ] )
           + ( src.at< uchar >( row-1, col+1 ) * kernel[ 0 ][ 2 ] )

           + ( src.at< uchar >( row, col-1 ) * kernel[ 1 ][ 0 ] )
           + ( src.at< uchar >( row, col ) * kernel[ 1 ][ 1 ] )
           + ( src.at< uchar >( row, col+1 ) * kernel[ 1 ][ 2 ] )

           + ( src.at< uchar >( row+1, col-1 ) * kernel[ 2 ][ 0 ] )
           + ( src.at< uchar >( row+1, col ) * kernel[ 2 ][ 1 ] )
           + ( src.at< uchar >( row+1, col+1 ) * kernel[ 2 ][ 2 ] ) );
}

cv::Mat medianBlur( cv::Mat& imgSrc )
{
    const short kernel[ 3 ][ 3 ] = { 1, 1, 1,
                                     1, 1, 1,
                                     1, 1, 1, };
    cv::Mat resul = cv::Mat( imgSrc.rows, imgSrc.cols, CV_8U, cv::Scalar::all(0) );
    int resulCalc = 0;

    for( int i = 1; i < imgSrc.rows - 1; i++ )
        for ( int j = 1; j < imgSrc.cols - 1; j++ )
        {
            resulCalc = round( ( (float) calcPixel( imgSrc, kernel, i, j ) ) / 9.f );
            resul.at< uchar >( i, j ) = resulCalc;
        }
    return resul;
}

int main( int argv, char** argc )
{
    cv::Mat imgTest( _ROWS, _COLS, CV_8U, cv::Scalar::all( 0 ) );
    HoughTransform HT( imgTest.size() );

    drawMainDiagonal( imgTest );
    //drawSquare( imgTest );
    //drawCircle( imgTest );
    //cv::circle( imgTest, cv::Point( 50,50 ), 25, cv::Scalar::all( 255 ), 1, 1 );

    cv::Mat imgTestBlurred = medianBlur( imgTest );

    HT.compute( imgTestBlurred );
    HT.getBestParamsRT( 10 );
    cv::Mat HSimg = HT.getHoughSpaceImage();
    cv::Mat Gx = HT.getGradientImageX();
    cv::Mat Gy = HT.getGradientImageY();

    cv::namedWindow( _MAIN_WINDOW_TITLE );
    cv::imshow( _MAIN_WINDOW_TITLE, imgTestBlurred );

    cv::imshow( "X Gradient ", Gx );
    cv::imshow( "Y Gradient ", Gy );
    cv::imshow( "Hough Space", HSimg );

    cv::waitKey( 0 );

    return 0;
}

