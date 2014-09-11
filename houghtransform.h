#ifndef HOUGHTRANSFORM_H
#define HOUGHTRANSFORM_H

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <iostream>
#include <vector>
#include <cmath>
#include <cstdint>

#define _USE_MATH_DEFINES

// Porque somente ate 90? Talvez seja por causa do quadrante que a imagem esta
#define _THETA_AXIS 360
#define _CONVERSION_RAD M_PI/180.0

class HoughTransform
{
public:
    HoughTransform( const cv::Size &imgSize );
    virtual ~HoughTransform();

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
     * @brief   Retorna os melhores pares (r,theta) encontrados, usando
     *          um limiar passado como parametro para determina-los.
     *
     * @param treshold  Limiar usado para definir quais pares (r,theta) sao considerados
     *                  bons. Caso o valor do acumulador na posicao [r][theta] > treshold,
     *                  estes parametros sao aceitos.
     *
     * @return  Vector com os parametros encontrados.
     */
    std::vector< int > getBestParamsRT( const uint treshold );

private:
    /**
     * @brief   Usado para se obter os melhores pontos (r,theta) que caracterizam
     *          as retas da imagem.
     *
     * @param treshold  Define o valor minimo do acumulador no ponto (r,theta) para se
     *                  Aceitar essas coordenadas como uma "boa" reta.
     *
     * @return  Numero de pares ordenados de (r,theta) aceitos.
     */
    int findBestParamsRT( const uint treshold );

    /**
     * @brief Preenche o Espaco de Hough com zeros.
     */
    void clearHoughSpace();

    /**
     * @brief   Angulo de inclinacao assumido. Igual ao valor de theta do ponto
     *          do acumulador com maior valor ("melhor reta").
     */
    int bestThetaPos;

    /**
     * @brief   Valor de R da melhor reta encontrada.
     */
    int bestRPos;

    /**
     * @brief   Maior valor do acumulador
     */
    uint maxHSValue;

    /**
     * @brief   Especifica se os melhores parametros (r,theta) ja foram encontrados.
     */
    bool bestRTFinded;

    /**
     * @brief   Especifica qual o ultimo limiar utilizado para achar os melhores parametros
     *          (r,theta). Caso ainda nao tenha achado esses parametros, o valor padrao
     *          eh 0.
     */
    uint lastTresholdUsed;

    /**
     * @brief   Matriz que representa o espaco de Hough (acumulador),
     *          onde [x][y] -> [r][theta].
     */
    uint **houghSpace;

    /**
     * @brief   Vetor de de 2 posições que representa o tamango da matriz houghSpace, onde
     *          [linhas][colunas] -> [r][theta].
     */
    uint houghSpaceSize [2];

    /**
     * @brief   Vector contento os pares ordenados de (r,theta) que caracterizam
     *          as retas da imagem.
     */
    std::vector< int > bestParams;
};

#endif // HOUGHTRANSFORM_H
