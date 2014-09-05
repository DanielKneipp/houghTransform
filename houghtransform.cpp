/*
 * Referencias:
 * 1 - http://dovgalecs.com/blog/opencv-matrix-types/  ---> CV_16U = ushort, CV_8U = uchar
 */

#include "houghtransform.h"

HoughTransform::HoughTransform( const cv::Size& imgSize ) :
    houghSpace( cv::Mat( (int) round( sqrt( pow( (double) imgSize.width, 2 ) +
                                            pow( (double) imgSize.height, 2 ) ) ),
                         _THETA, CV_16U, cv::Scalar::all( 0 ) ) ),
    thetaMax( 0.0 )
{
}

void HoughTransform::compute( cv::Mat& edgeImage )
{
    uchar pixel;
    int dist;

    for( int i = 0; i < edgeImage.rows; i++ )
    {
        for( int j = 0; j < edgeImage.cols; j++ )
        {
            pixel = edgeImage.at< uchar >( i, j );
            std::cout << "pixel value: " << (int) pixel << std::endl;
            if( pixel == 255 )
            {
                for( float angle = 0.f; angle < _THETA; angle++ ) // Porque somente ate 180?
                {
                    dist = round( ( (double) j * cos( angle * _CONVERSION_RAD ) ) +
                                  ( (double) i * sin( angle * _CONVERSION_RAD ) ) );
                    this->houghSpace.at< ushort >( dist, angle )++;
                    std::cout << "space value: " << this->houghSpace.at< uchar >( dist, angle ) << std::endl << std::endl;
                }
            }
        }
    }

    this->printHoughSpaceValues();
}

std::vector< int > HoughTransform::getBestParamsRT( const ushort treshold )
{
    std::vector< int > bestParams;

    ushort pointMax = this->houghSpace.at< ushort >( 0, 0 );

    for( int i = 0; i < this->houghSpace.rows; i++ )
    {
        for( int j = 0; j < this->houghSpace.cols; j++ )
        {
            if( this->houghSpace.at< ushort >( i, j ) > pointMax)
            {
                pointMax = this->houghSpace.at< ushort >( i, j );
                this->thetaMax = j;
            }
            if( this->houghSpace.at< ushort >( i, j ) >= treshold )
            {
                bestParams.push_back( i );
                bestParams.push_back( j );
            }
        }
    }
    this->bestRTFinded = true;

    return bestParams;
}

cv::Mat HoughTransform::getHoughSpaceImage()
{
    cv::Mat HSimg;
    this->houghSpace.convertTo( HSimg, CV_8U );
    return HSimg;
}

void HoughTransform::printHoughSpaceValues()
{
    std::cout << " = "<< std::endl << " "  << this->houghSpace << std::endl << std::endl;
}
