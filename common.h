#ifndef COMMON_H
#define COMMON_H

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <cmath>
#include <vector>

#define _USE_MATH_DEFINES

#define _THETA_AXIS 360
#define _CONVERSION_RAD M_PI/180.0
#define _CONVERSION_DEG 180.0/M_PI

struct matrix2D
{
    /** Matriz contendo os valores inteiros. */
    int **mat = NULL;

    /** Numero de linhas da matriz. */
    uint rows = 0;

    /** Numero de linhas da matriz. */
    uint cols = 0;

    /** Especifica se a matriz ja foi inicializada  */
    bool initialized = false;

    /** Vetor que guarda o valor maximo e a posicao que contem este valor
     *  na {@link #**mat matriz}. Se armazena na ordem [valor, x, y] */
    int maxValuesPos[ 3 ] = { -1, -1, -1 };
};

typedef struct matrix2D matrix2D;

/**
 * @brief   Acha a posicao que contem o maior valor da matriz.
 *
 * @param Matrix    Onde sera procurado o maior valor.
 */
void findBestValuePos( matrix2D& Matrix );


#endif // COMMON_H
