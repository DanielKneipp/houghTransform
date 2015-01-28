#ifndef COMMON_H
#define COMMON_H

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <cmath>
#include <vector>
#include <iostream>

#define _USE_MATH_DEFINES

#define _THETA_AXIS 360
#define _CONVERSION_RAD M_PI/180.0
#define _CONVERSION_DEG 180.0/M_PI

#define DIST( x, y ) sqrt( pow( x, 2 ) + pow( y, 2 ) )

struct int3
{
    int3( int key, int n1, int n2 );
    int3();

    /** Chave da estrutura, usada para comparacao de magnitude */
    int key;

    /** Primeiro valor */
    int n1;

    /** Segundo valor */
    int n2;

    /** Seleciona o atributo key para ser valor a ser comparado
     *  quando duas estruturas destipo sao compradas com relacao a suas
     *  magnitudes */
    bool operator < ( const int3& other ) const;
};

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
     *  na {@link #**mat matriz}. Se armazena na ordem [valor, linha, coluna] */
    int maxValuesPos[ 3 ] = { -1, -1, -1 };
};

//typedef struct matrix2D matrix2D;

/**
 * @brief   Acha a posicao que contem o maior valor da matriz.
 *
 * @param Matrix    Onde sera procurado o maior valor.
 */
void findBestValuePos( matrix2D& Matrix );


/**
 * @brief   Inicializa a matrix2D passada como parametro, alocando a matriz
 *          e inicializando os valores internos.
 *
 * @param Matrix    Matriz que sera inicializada.
 *
 * @param rows  Quantidade de linhas da matriz.
 *
 * @param cols  Quantidade de colunas da matriz.
 */
void initMatrix( matrix2D& Matrix , int rows, int cols );

/**
 * @brief   Desaloca a matriz da memoria.
 * .
 * @param Matrix    Matriz a ser desalocada.
 */
void freeMatrix( matrix2D& Matrix );

/**
 * @brief   Preenche toda a matriz com o valor passado como parametro.
 *
 * @param Matrix    Matriz a ser preenchida.
 *
 * @param n     Valor a ser usado para preencher a matriz.
 */
void fillMatrixWith( matrix2D& Matrix, int n );

/**
 * @brief   Imrime a matriz na saida padrao.
 *
 * @param Matrix    Matriz que sera impressa.
 */
void printMatrix( matrix2D& Matrix );

/**
 * @brief   Retorna uma imagem que representa a matriz passada como parametro.
 *
 * @param Matrix    Matriz que sera representada na imagem.
 *
 * @return  Imagem que representa a matriz passada como parametro.
 */
cv::Mat getImageOfMatrix( matrix2D& Matrix );


#endif // COMMON_H
