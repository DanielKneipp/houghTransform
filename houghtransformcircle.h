#ifndef HOUGHTRANSFORMCIRCLE_H
#define HOUGHTRANSFORMCIRCLE_H

#include "common.h"

class HoughTransformCircle
{
public:
    HoughTransformCircle( const cv::Size &imgSize );
    virtual ~HoughTransform();

    /**
     * @brief   Se computa o espaco de Hough (a,b) da imagem passada como parametro
     *          para se detectar circulos na mesma
     *
     * @param edgeImage     Imagem pre-processada com algum metodo de deteccao de
     *                      contornos, e binarizada.
     */
     void compute( cv::Mat& edgeImage );

     /**
      * @brief   Usado para se obter o espaco de Hough (Acumulador).
      *
      * @return  Imagem em escala de cinza representado o espaco de Hough
      *          (linhas -> r, colunas -> theta).
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
      * @return     Vector com os pares encontrados.
      */
     std::vector< int > getBestParamsAB( const uint treshold );

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
     int findBestParamsRT( const uint treshold );
};

#endif // HOUGHTRANSFORMCIRCLE_H
