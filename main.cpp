#include <iostream>
#include <opencv2/highgui/highgui.hpp>

#include "houghtransform.h"
#include "houghtransformcircle.h"
#include "common.h"

#define _COLS 500
#define _ROWS 500

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
void drawCircle( cv::Mat& img, int pixelXCenter = -1, int pixelYCenter = -1, int r = -1 )
{
    if( pixelXCenter == -1 )
        pixelXCenter = round( (float) _COLS / 2.0 );
    if( pixelYCenter == -1 )
        pixelYCenter = round( (float) _ROWS / 2.0 );
    if( r == -1 )
    {
        int colMin = round( (float) _COLS / 10.0 ), rowMin = round( (float) _ROWS / 10.0 );
        if( colMin < rowMin )
            r = round( (float) ( _COLS - colMin - colMin ) / 2.0 );
        else
            r = round( (float) ( _ROWS - rowMin - rowMin ) / 2.0 );
    }

    for( float theta = 0.0; theta < 360.0; theta = theta + 0.1 )
    {
        int x = round( pixelXCenter + r * cos( ( theta * _CONVERSION_RAD ) ) );
        int y = round( pixelYCenter + r * sin( ( theta * _CONVERSION_RAD ) ) );

        if( x >= 0 && x < img.cols &&
            y >= 0 && y < img.rows)
            img.at< uchar>( y, x ) = 255;
    }
}

inline int calcPixel( const cv::Mat src, const short kernel[ 3 ][ 3 ], int row, int col )
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
    HoughTransformCircle HTC;
    //HoughTransform HT( imgTest.size() );

    //imgTest.at< uchar >( _ROWS / 2, _COLS / 2 ) = 255;
    //drawMainDiagonal( imgTest );
    //drawSquare( imgTest );
    drawCircle( imgTest, 30, 48, 130 );
    drawCircle( imgTest, imgTest.cols - 120, imgTest.rows - 150, 130);
    drawCircle( imgTest, imgTest.cols/2, imgTest.rows/2, 130 );

    //cv::Mat imgTestBlurred = medianBlur( imgTest );

    //HT.compute( imgTestBlurred );
    //HT.getBestParamsRT( 10 );
    //cv::Mat HSimg = HT.getHoughSpaceImage();
    //cv::Mat Gx = HT.getGradientImageX();
    //cv::Mat Gy = HT.getGradientImageY();

    HTC.compute( imgTest, 130 );
    std::vector< int3 > bestParams = HTC.getBestParamsAB( 1, 15 );
    cv::Mat HSimg = HTC.getHoughSpaceImage();

    for( int3& centroide : bestParams )
    {
        std::cout << "---------------------" << std::endl;
        std::cout << "a,b: " << centroide.n1 << "," << centroide.n2 << std::endl;
        std::cout << "pixelXCenter,pixelYCenter: " << centroide.n1 << "," << imgTest.rows - centroide.n2 << std::endl;
        std::cout << "#Points in this circle: " << centroide.key << std::endl;
        std::cout << "---------------------" << std::endl;
    }

    cv::namedWindow( _MAIN_WINDOW_TITLE );
    cv::imshow( _MAIN_WINDOW_TITLE, imgTest );

    //cv::imshow( "X Gradient ", Gx );
    //cv::imshow( "Y Gradient ", Gy );
    cv::imshow( "Hough Space", HSimg );

    cv::waitKey( 0 );

    return 0;
}

