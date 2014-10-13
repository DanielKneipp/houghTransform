/*
 * Referencias:
 * 1 - http://dovgalecs.com/blog/opencv-matrix-types/  ---> CV_16U = ushort, CV_8U = uchar
 */

#include "houghtransform.h"

inline int calcPixelGrad( cv::Mat src, const short kernel[ 3 ][ 3 ], int row, int col)
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

inline int calcPixelGrad_Left( cv::Mat src, const short kernel[ 3 ][ 3 ], int row, int col)
{
    return ( ( 0 )
           + ( src.at< uchar >( row-1, col ) * kernel[ 0 ][ 1 ] )
           + ( src.at< uchar >( row-1, col+1 ) * kernel[ 0 ][ 2 ] )

           + ( 0 )
           + ( src.at< uchar >( row, col ) * kernel[ 1 ][ 1 ] )
           + ( src.at< uchar >( row, col+1 ) * kernel[ 1 ][ 2 ] )

           + ( 0 )
           + ( src.at< uchar >( row+1, col ) * kernel[ 2 ][ 1 ] )
           + ( src.at< uchar >( row+1, col+1 ) * kernel[ 2 ][ 2 ] ) );
}

inline int calcPixelGrad_Right( cv::Mat src, const short kernel[ 3 ][ 3 ], int row, int col)
{
    return ( ( src.at< uchar >( row-1, col-1 ) * kernel[ 0 ][ 0 ] )
           + ( src.at< uchar >( row-1, col ) * kernel[ 0 ][ 1 ] )
           + ( 0 )

           + ( src.at< uchar >( row, col-1 ) * kernel[ 1 ][ 0 ] )
           + ( src.at< uchar >( row, col ) * kernel[ 1 ][ 1 ] )
           + ( 0 )

           + ( src.at< uchar >( row+1, col-1 ) * kernel[ 2 ][ 0 ] )
           + ( src.at< uchar >( row+1, col ) * kernel[ 2 ][ 1 ] )
           + ( 0 ) );
}

inline int calcPixelGrad_Top( cv::Mat src, const short kernel[ 3 ][ 3 ], int row, int col)
{
    return ( ( 0 )
           + ( 0 )
           + ( 0 )

           + ( src.at< uchar >( row, col-1 ) * kernel[ 1 ][ 0 ] )
           + ( src.at< uchar >( row, col ) * kernel[ 1 ][ 1 ] )
           + ( src.at< uchar >( row, col+1 ) * kernel[ 1 ][ 2 ] )

           + ( src.at< uchar >( row+1, col-1 ) * kernel[ 2 ][ 0 ] )
           + ( src.at< uchar >( row+1, col ) * kernel[ 2 ][ 1 ] )
           + ( src.at< uchar >( row+1, col+1 ) * kernel[ 2 ][ 2 ] ) );
}

inline int calcPixelGrad_Bottom( cv::Mat src, const short kernel[ 3 ][ 3 ], int row, int col)
{
    return ( ( src.at< uchar >( row-1, col-1 ) * kernel[ 0 ][ 0 ] )
           + ( src.at< uchar >( row-1, col ) * kernel[ 0 ][ 1 ] )
           + ( src.at< uchar >( row-1, col+1 ) * kernel[ 0 ][ 2 ] )

           + ( src.at< uchar >( row, col-1 ) * kernel[ 1 ][ 0 ] )
           + ( src.at< uchar >( row, col ) * kernel[ 1 ][ 1 ] )
           + ( src.at< uchar >( row, col+1 ) * kernel[ 1 ][ 2 ] )

           + ( 0 )
           + ( 0 )
           + ( 0 ) );
}

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

    if( this->gradientX.initialized )
    {
        for( uint i = 0; i < this->gradientX.rows; i++ )
            delete[] this->gradientX.mat[ i ];
        delete[] this->gradientX.mat;
    }

    if( this->gradientY.initialized )
    {
        for( uint i = 0; i < this->gradientY.rows; i++ )
            delete[] this->gradientY.mat[ i ];
        delete[] this->gradientY.mat;
    }
}

void HoughTransform::initGrads( uint rows, uint cols )
{
    // Desalocacao de memoria, caso necessario
    if( this->gradientX.initialized )
    {
        for( uint i = 0; i < this->gradientX.rows; i++)
            delete[] this->gradientX.mat[ i ];
        delete[] this->gradientX.mat;
    }
    if( this->gradientY.initialized )
    {
        for( uint i = 0; i < this->gradientY.rows; i++)
            delete[] this->gradientY.mat[ i ];
        delete[] this->gradientY.mat;
    }

    // Inicializacao ocm novos valores.
    this->gradientX.mat = new int*[ rows ];
    for( uint i = 0; i < rows; i++)
        this->gradientX.mat[ i ] = new int[ cols ];
    this->gradientX.rows = rows;
    this->gradientX.cols = cols;
    this->gradientX.initialized = true;
    for( ushort i = 0; i < 2; i++ )
        this->gradientX.maxValuesPos[ i ] = -1;

    this->gradientY.mat = new int*[ rows ];
    for( uint i = 0; i < rows; i++)
        this->gradientY.mat[ i ] = new int[ cols ];
    this->gradientY.rows = rows;
    this->gradientY.cols = cols;
    this->gradientY.initialized = true;
    for( ushort i = 0; i < 2; i++ )
        this->gradientY.maxValuesPos[ i ] = -1;
}

void HoughTransform::compute( cv::Mat& edgeImage )
{
    uchar pixel;
    int valueGx, valueGy;
    int dist;
    double angle, rad_angle;
    double phi, deg_phi;

    this->clearHoughSpace();
    this->initGrads( edgeImage.rows, edgeImage.cols );

    this->gradientCalc( edgeImage );

    for( int i = 0; i < edgeImage.rows; i++ )
    {
        for( int j = 0; j < edgeImage.cols; j++ )
        {
            // Em uma imagem, o ponto final fica na linha mais abaixo,
            // ao contrario de um plano cartesiano.
            pixel = edgeImage.at< uchar >( edgeImage.rows - i - 1, j );
            valueGx = this->gradientX.mat[ this->gradientX.rows - i - 1 ][ j ];
            valueGy = this->gradientY.mat[ this->gradientY.rows - i - 1 ][ j ];

            //std::cout << "pixel value: " << (int) pixel << std::endl;
            if( pixel > 20 )
            {
                //std::cout << "-----------------------" << std::endl << std::endl;

                phi = atan2( valueGy, valueGx );
                deg_phi = phi * _CONVERSION_DEG;
                if( deg_phi < 0 )
                    deg_phi = deg_phi + 180;

                angle = deg_phi;
                //angle = deg_phi + 90; // angle (theta) nao eh com relacao a reta em si!
                                        // Mas sim com relacao a semi-reta e o eixo x,
                                        // semi-reta que traca a menor distancia entre a reta de interesse
                                        // e a origem.

                rad_angle = angle * _CONVERSION_RAD;
                dist = round( ( (double) j * cos( rad_angle ) ) +
                              ( (double) i * sin( rad_angle ) ) );

                // ( (-1)^k * r, theta + pi*k )
                if( dist < 0 )
                {
                    dist = abs( dist );
                    this->houghSpace[ dist ][ (int) round( angle ) + 180 ]++;
                }
                else
                {
                    this->houghSpace[ dist ][ (int) round( angle ) ]++;
                }

                //std::cout << "space value: " << this->houghSpace[ dist ][ (int) angle ] << std::endl;

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

cv::Mat HoughTransform::getGradientImageX()
{
    this->imgGradientX = cv::Mat( this->gradientX.rows, this->gradientX.cols, CV_8U );

    float ratio = 255.f / (float) this->gradientX.maxValuesPos[ 0 ];

    for( uint i = 0; i < this->gradientX.rows; i++ )
        for( uint j = 0; j < this->gradientX.cols; j++ )
            this->imgGradientX.at< uchar >( i, j ) = (int) round( this->gradientX.mat[ i ][ j ] * ratio );

    return this->imgGradientX;
}

cv::Mat HoughTransform::getGradientImageY()
{
    this->imgGradientY = cv::Mat( this->gradientY.rows, this->gradientY.cols, CV_8U );

    float ratio = 255.f / (float) this->gradientY.maxValuesPos[ 0 ];

    for( uint i = 0; i < this->gradientY.rows; i++ )
        for( uint j = 0; j < this->gradientY.cols; j++ )
            this->imgGradientY.at< uchar >( i, j ) = (int) round( this->gradientY.mat[ i ][ j ] * ratio );

    return this->imgGradientY;
}

void HoughTransform::gradientCalc( const cv::Mat src )
{
    const short kernelX [ 3 ][ 3 ] = { -1, 0, 1,
                                       -1, 0, 1,
                                       -1, 0, 1 };
    const short kernelY [ 3 ][ 3 ] = { -1, -1, -1,
                                        0, 0, 0,
                                        1, 1, 1 };

    // Passada interna a imagem
    for( int i = 1; i < src.rows - 1; i++ )
    {
        for( int j = 1; j < src.rows - 1; j++ )
        {
            this->gradientX.mat[ i ][ j ] = calcPixelGrad( src, kernelX, i, j );
            this->gradientY.mat[ i ][ j ] = calcPixelGrad( src, kernelY, i, j );
        }
    }

    // Passada nas bordas da imagem
    // 1 - Extremos esquerdo e direito
    for( int i = 1; i < src.rows - 1; i++ )
    {
        this->gradientX.mat[ i ][ 0 ] = calcPixelGrad_Left( src, kernelX, i, 0 );
        this->gradientY.mat[ i ][ 0 ] = calcPixelGrad_Left( src, kernelY, i, 0 );

        this->gradientX.mat[ i ][ src.cols - 1 ] = calcPixelGrad_Right( src, kernelX, i, src.cols - 1 );
        this->gradientY.mat[ i ][ src.cols - 1 ] = calcPixelGrad_Right( src, kernelY, i, src.cols - 1 );
    }

    // 2 - Extremo superior e inferior
    for( int j = 1; j < src.cols - 1; j++ )
    {
        this->gradientX.mat[ 0 ][ j ] = calcPixelGrad_Top( src, kernelX, 0, j );
        this->gradientY.mat[ 0 ][ j ] = calcPixelGrad_Top( src, kernelY, 0, j );

        this->gradientX.mat[ src.rows - 1 ][ j ] = calcPixelGrad_Bottom( src, kernelX, src.rows - 1, j );
        this->gradientY.mat[ src.rows - 1 ][ j ] = calcPixelGrad_Bottom( src, kernelY, src.rows - 1, j );
    }

    // TODO: 3 - Quinas

    HoughTransform::findBestValuePos( this->gradientX );
    HoughTransform::findBestValuePos( this->gradientY );
}

void HoughTransform::findBestValuePos( matrix& Matrix )
{
    for( uint i = 0; i < Matrix.rows; i++ )
        for( uint j = 0; j < Matrix.rows; j++ )
            if( Matrix.mat[ i ][ j ] > Matrix.maxValuesPos[ 0 ] )
            {
                Matrix.maxValuesPos[ 0 ] = Matrix.mat[ i ][ j ];
                Matrix.maxValuesPos[ 1 ] = i;
                Matrix.maxValuesPos[ 2 ] = j;
            }
}

