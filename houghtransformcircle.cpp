#include "houghtransformcircle.h"

HoughTransformCircle::HoughTransformCircle( ) : radius( -1 )
{

}

HoughTransformCircle::~HoughTransformCircle()
{
    freeMatrix( this->HoughSpace );
}

cv::Mat HoughTransformCircle::getHoughSpaceImage()
{
    return getImageOfMatrix( this->HoughSpace );
}

void HoughTransformCircle::printHoughSpaceValues()
{
    printMatrix( this->HoughSpace );
}

int HoughTransformCircle::findBestParamsAB( const int treshold )
{
    for( uint i = 0; i < this->HoughSpace.rows; i++ )
        for( uint j = 0; j < this->HoughSpace.cols; j++ )
            if( this->HoughSpace.mat[ i ][ j ] >= treshold )
            {
                int3 newParam( this->HoughSpace.mat[ i ][ j ],
                               ( i - ceil( this->radius ) ),
                               ( j - ceil( this->radius ) ) );
                this->bestParams.push_back( newParam );
            }

    std::sort( this->bestParams.begin(), this->bestParams.end() );
    return (int) ( this->bestParams.size() );
}

std::vector< int3 > HoughTransformCircle::getBestParamsAB( const int treshold,
                                                           uint nPairsOfParams )
{
    if( this->bestParams.empty() )
        this->findBestParamsAB( treshold );

    if( nPairsOfParams >= this->bestParams.size() )
        return this->bestParams;
    else
        return std::vector< int3 >( this->bestParams.end() - nPairsOfParams,
                                    this->bestParams.end());
}

void HoughTransformCircle::compute( const cv::Mat &edgeImage, float r )
{
    uchar pixel;
    // Se aumenta r no tamnaho do espaco de hough para comportar
    // pontos a,b que excedem a imagem original, i.e semi-circunferencia
    // numa extremidade da imagem.
    int houghSize_x = edgeImage.cols + 2 * ceil( r );
    int houghSize_y = edgeImage.rows + 2 * ceil( r );
    int a = 0;
    int b = 0;

    // Limpando residuos de processamentos anteriores.
    this->bestParams.clear();
    initMatrix( this->HoughSpace, houghSize_y, houghSize_x );
    fillMatrixWith( this->HoughSpace, 0 );
    this->radius = r;

    for( int y = 0; y < edgeImage.rows; y++ )
    {
        for( int x = 0; x < edgeImage.cols; x++ )
        {
            // Em uma imagem, o ponto final fica na linha mais abaixo,
            // ao contrario de um plano cartesiano.
            pixel = edgeImage.at< uchar >( edgeImage.rows - y - 1, x );

            if( pixel > 250 )
            {
                for( float theta = 0.0; theta < 360.0; theta++ )
                {
                    a = round( x - r * cos( ( theta * _CONVERSION_RAD ) ) );
                    b = round( y - r * sin( ( theta * _CONVERSION_RAD ) ) );
                    // Ajuste de valor para nao ter a,b negativos caso o centro
                    // de massa da circunferencia seja abaixo ou a esquerda do
                    // limite da imagem.
                    a = a + ceil( r );
                    b = b + ceil( r );

                    this->HoughSpace.mat[ a ][ b ]++;
                }
            }
        }
    }
}
