#include <common.h>

void findBestValuePos( matrix2D& Matrix )
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
