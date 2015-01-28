#ifndef HOUGHTRANSFORMCIRCLE_H
#define HOUGHTRANSFORMCIRCLE_H

#include "common.h"

class HoughTransformCircle
{
public:
    HoughTransformCircle();
    virtual ~HoughTransformCircle();

    /**
     * @brief   Se computa o espaco de Hough (a,b) da imagem passada como parametro
     *          para se detectar circunferencias na mesma
     *
     * @param edgeImage     Imagem pre-processada com algum metodo de deteccao de
     *                      contornos, e binarizada.
     *
     * @param r     Raio das circunferencias que serao procuradas.
     */
     void compute( const cv::Mat& edgeImage, float r );

     /**
      * @brief   Usado para se obter o espaco de Hough (Acumulador).
      *
      * @return  Imagem em escala de cinza representado o espaco de Hough
      *          (linhas -> a, colunas -> b).
      */
     cv::Mat getHoughSpaceImage();

     /**
      * @brief   Imprime os valores do Espaço de Hough na saída padrão.
      */
     void printHoughSpaceValues();

     /**
      * @brief  Retorna os pares (a,b) encontrados onde o valor referente a posicao
      *         seja igual ou maior ao limiar passado como parametro.
      *
      * @param treshold Limiar utilizado para escolha dos pares (a,b).
      *
      * @param nPairsOfParams  Numero de pares de parametros maximo a se retornar.
      *
      * @return     Vector com os pares encontrados.
      */
     std::vector< int3 > getBestParamsAB( const int treshold, uint nPairsOfParams );

private:
     /**
      * @brief   Usado para se obter os melhores pontos (a,b) que caracterizam
      *          as circunferencias da imagem.
      *
      * @param treshold  Define o valor minimo do acumulador no ponto (a,b) para se
      *                  aceitar essas coordenadas como uma "boa" circunferencia.
      *
      * @return  Numero de pares ordenados de (a,b) aceitos.
      */
     int findBestParamsAB( const int treshold );

     /**
      * @brief  Matriz que representa o espaco de Hough (acumulador), onde
      *         [linhas][colunas] -> [a+ceil(r)][b+ceil(r)].
      */
     matrix2D HoughSpace;

     /**
      * @brief   Vector contento os pares ordenados de (a,b) que caracterizam
      *          as circunferencias da imagem.
      */
     std::vector< int3 > bestParams;

     /**
      * @brief  Especifica o raio das circunferencias que estao sendo procuradas.
      */
     float radius;
};

#endif // HOUGHTRANSFORMCIRCLE_H
