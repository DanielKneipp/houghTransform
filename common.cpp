#include <common.h>

int3::int3(){}

int3::int3( int key, int n1, int n2 ) : key( key ), n1( n1 ), n2( n2 ) {}

bool int3::operator < ( const int3& other ) const
{
    return ( key < other.key );
}

void findBestValuePos( matrix2D& Matrix )
{
    for( uint i = 0; i < Matrix.rows; i++ )
        for( uint j = 0; j < Matrix.cols; j++ )
            if( Matrix.mat[ i ][ j ] > Matrix.maxValuesPos[ 0 ] )
            {
                Matrix.maxValuesPos[ 0 ] = Matrix.mat[ i ][ j ];
                Matrix.maxValuesPos[ 1 ] = i;
                Matrix.maxValuesPos[ 2 ] = j;
            }
}

void initMatrix( matrix2D& Matrix, int rows, int cols )
{
    if( Matrix.initialized )
        freeMatrix(Matrix);

    Matrix.cols = cols;
    Matrix.rows = rows;
    Matrix.initialized = true;
    for( ushort i = 0; i < 2; i++)
        Matrix.maxValuesPos[ i ] = -1;

    Matrix.mat = new int*[ rows ];
    for( int i = 0; i < rows; i++ )
        Matrix.mat[ i ] = new int[ cols ];
}

void freeMatrix( matrix2D& Matrix )
{
    if( Matrix.initialized )
    {
        for( uint i = 0; i < Matrix.rows; i++)
            delete[] Matrix.mat[ i ];
        delete[] Matrix.mat;
    }
}

void fillMatrixWith( matrix2D& Matrix, int n )
{
    for( uint i = 0; i < Matrix.rows; i++ )
        for( uint j = 0; j < Matrix.cols; j++ )
            Matrix.mat[ i ][ j ] = n;
}

void printMatrix( matrix2D& Matrix )
{
    if( !Matrix.initialized )
    {
        std::cout << "Matriz nao inicializada" << std::endl;
        return;
    }
    std::cout << std::endl << std::endl << " ------ MATRIZ ------ " << std::endl;
    for( uint i = 0; i < Matrix.rows; i++ )
    {
        std::cout << "|";
        for( uint j = 0; j < Matrix.cols; j++ )
            std::cout << " " << Matrix.mat[ i ][ j ] << " ";

        std::cout << "|" << std::endl;
    }
    std::cout << std::endl << std::endl << " ------ MATRIZ ------ " << std::endl;
}

cv::Mat getImageOfMatrix( matrix2D& Matrix )
{
    if( !Matrix.initialized )
        return cv::Mat();

    if( Matrix.maxValuesPos[ 0 ] == -1)
        findBestValuePos( Matrix );

    cv::Mat mImg( Matrix.rows, Matrix.cols, CV_8U );

    float ratio = 255.f / (float) Matrix.maxValuesPos[0];

    for( int i = 0; i < mImg.rows; i++ )
        for( int j = 0; j < mImg.cols; j++ )
            mImg.at< uchar >( i, j ) = (int) Matrix.mat[ i ][ j ] * ratio;

    return mImg;
}
