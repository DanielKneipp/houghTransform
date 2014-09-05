#ifndef HOUGHTRANSFORM_H
#define HOUGHTRANSFORM_H

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <iostream>
#include <vector>
#include <cmath>
#include <cstdint>

#define _USE_MATH_DEFINES

#define _THETA 180.0
#define _CONVERSION_RAD 180.0/M_PI

class HoughTransform
{
public:
    HoughTransform(const cv::Size &imgSize );

    /**
     * @brief   Calcula o parametro r (distancia da origem) da equacao da reta
     *          no formato de coordenadas polares (r = x*cos(theta) + y*sen(theta)),
     *          variando o theta (angulo) de 0 a 180. Somente eh feito esse calculo
     *          para pontos da imagem edgeImage que sejam brancos (255).
     *
     * @param edgeImage     Imagem pre-processada com algum metodo de deteccao de
     *                      contornos, e binarizada.
     */
    void compute( cv::Mat& edgeImage );

    /**
     * @brief   Usado para se obter os melhores pontos (r,theta) que caracterizam
     *          as retas da imagem.
     *
     * @param treshold  Define o valor minimo do acumulador no ponto (r,theta) para se
     *                  Aceitar essas coordenadas como uma "boa" reta.
     *
     * @return  Vector contento os pares ordenados de (r,theta) aceitos.
     */
    std::vector< int > getBestParamsRT( const ushort treshold );

    /**
     * @brief   Usado para se obter o espaco de Hough (Acumulador).
     *
     * @return  Imagem em escala de cinza representado o espaco de Hough
     *          (linhas -> r, colunas -> theta).
     */
    cv::Mat getHoughSpaceImage();

    void printHoughSpaceValues();

private:
    /**
     * @brief Imagem em CV_16U que representa o espaco de Hough (acumulador).
     */
    cv::Mat houghSpace;

    /**
     * @brief   Angulo de inclinacao assumido. Igual ao valor de theta do ponto
     *          do acumulador com maior valor ("melhor reta").
     */
    float thetaMax;

    /**
     * @brief Especifica se os melhores parametros (r,theta) ja foram encontrados.
     */
    bool bestRTFinded;
};

#endif // HOUGHTRANSFORM_H
