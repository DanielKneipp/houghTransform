/*
 * Referencias:
 * 1 - http://dovgalecs.com/blog/opencv-matrix-types/  ---> CV_16U = ushort, CV_8U = uchar
 */

#include "houghtransform.h"

HoughTransform::HoughTransform( const cv::Size& imgSize ) : bestThetaPos( 0 ), bestRPos( 0 ),
                                                            maxHSValue( 0 ), lastTresholdUsed( 0 )
{
    this->houghSpaceSize[ 0 ] = (int) round( sqrt( pow( (double) imgSize.width, 2 ) +
                                                   pow( (double) imgSize.height, 2 ) ) );
    houghSpaceSize[ 1 ] = _THETA_AXIS ;

    this->houghSpace = new uint*[ houghSpaceSize[ 0 ] ];
    for( uint i = 0; i < houghSpaceSize[ 0 ]; i++ )
        this->houghSpace[ i ] = new uint[ _THETA_AXIS ];

    this->clearHoughSpace();
}

HoughTransform::~HoughTransform()
{
    for( uint i = 0; i < this->houghSpaceSize[ 0 ]; i++ )
        delete[] this->houghSpace[ i ];
    delete[] this->houghSpace;
}

void HoughTransform::compute( cv::Mat& edgeImage )
{
    uchar pixel;
    int dist;

    this->clearHoughSpace();

    for( int i = 0; i < edgeImage.rows; i++ )
    {
        for( int j = 0; j < edgeImage.cols; j++ )
        {
            // Em uma imagem, o ponto final fica na linha mais abaixo, ao contrario de um plano cartesiano
            pixel = edgeImage.at< uchar >( edgeImage.rows - i - 1, j );

            //std::cout << "pixel value: " << (int) pixel << std::endl;
            if( pixel > 250 )
            {
                //std::cout << "-----------------------" << std::endl << std::endl;
                for( float angle = 0.f; angle < _THETA_AXIS / 2; angle++ )
                {
                    double rad_angle = angle * _CONVERSION_RAD;
                    dist = round( ( (double) j * cos( rad_angle ) ) +
                                  ( (double) i * sin( rad_angle ) ) );

                    // ( (-1)^k * r, theta + pi*k )
                    if( dist < 0 )
                    {
                        dist = abs( dist );
                        this->houghSpace[ dist ][ (int) angle + 180 ]++;
                    }
                    else
                    {
                        this->houghSpace[ dist ][ (int) angle ]++;
                    }

                    //std::cout << "space value: " << this->houghSpace[ dist ][ (int) angle ] << std::endl;
                }
                //std::cout << "-----------------------" << std::endl << std::endl;
            }
        }
    }
    this->bestRTFinded = false;
}

int HoughTransform::findBestParamsRT( const uint treshold )
{
    bestParams.clear();
    this->lastTresholdUsed = treshold;

    this->maxHSValue = this->houghSpace[ 0 ][ 0 ];

    for( uint i = 0; i < this->houghSpaceSize[ 0 ]; i++ )
    {
        for( uint j = 0; j < this->houghSpaceSize[ 1 ]; j++ )
        {
            if( this->houghSpace[ i ][ j ] > this->maxHSValue)
            {
                this->maxHSValue = this->houghSpace[ i ][ j ];
                this->bestThetaPos = j;
                this->bestRPos = i;
            }
            if( this->houghSpace[ i ][ j ] >= treshold )
            {
                bestParams.push_back( i );
                bestParams.push_back( j );
            }
        }
    }
    this->bestRTFinded = true;

    return (int) ( this->bestParams.size() / 2 );
}

void HoughTransform::clearHoughSpace()
{
    for( uint i = 0; i < houghSpaceSize[ 0 ]; i++ )
        for( uint j = 0; j < houghSpaceSize[ 1 ]; j++ )
            this->houghSpace[ i ][ j ] = 0;
}

cv::Mat HoughTransform::getHoughSpaceImage()
{
    cv::Mat HSimg( this->houghSpaceSize[ 0 ], this->houghSpaceSize[ 1 ], CV_8U );

    float ratio = 255.f / (float) this->maxHSValue;

    for( int i = 0; i < HSimg.rows; i++ )
        for( int j = 0; j < HSimg.cols; j++ )
            HSimg.at< uchar >( i, j ) = (int) this->houghSpace[ i ][ j ] * ratio;

    return HSimg;
}

void HoughTransform::printHoughSpaceValues()
{
    if( !this->bestRTFinded )
        this->findBestParamsRT( 3 );

    std::cout << std::endl << std::endl << " ------ Hough Space: ------ " << std::endl;

    for( uint i = 0; i < houghSpaceSize[ 0 ]; i++ )
    {
        std::cout << "|";
        for( uint j = 0; j < houghSpaceSize[ 1 ]; j++ )
            std::cout << " " << this->houghSpace[ i ][ j ] << " ";

        std::cout << "|" << std::endl;
    }
    std::cout << std::endl << std::endl << " ------ Hough Space: ------ " << std::endl;
}

std::vector< int > HoughTransform::getBestParamsRT( const uint treshold )
{
    if( this->bestRTFinded && this->lastTresholdUsed == treshold)
        return this->bestParams;
    else
    {
        this->findBestParamsRT( treshold );
        return this->bestParams;
    }
}
