#include "kalman.h"
#include <math.h>

/**
 * @file kalman.c
 * @brief Implementação do Filtro de Kalman.
 */

void kalman_init(KalmanState *state) {
    // Valores de incerteza do filtro. Estes podem ser ajustados (tuning)
    // para otimizar o desempenho do filtro para uma aplicação específica.
    state->Q_angle = 0.001f;
    state->Q_bias = 0.003f;
    state->R_measure = 0.03f;

    state->angle = 0.0f; // Inicia o ângulo em 0
    state->bias = 0.0f;  // Inicia o bias em 0

    // Inicia a matriz de covariância do erro com valores de incerteza elevados
    state->P[0][0] = 0.0f;
    state->P[0][1] = 0.0f;
    state->P[1][0] = 0.0f;
    state->P[1][1] = 0.0f;
}

float kalman_get_angle(KalmanState *state, float newAngle, float newRate, float dt) {
    // Etapa de Predição:
    // Estima o ângulo atual com base na leitura do giroscópio.
    state->rate = newRate - state->bias;
    state->angle += dt * state->rate;

    // Atualiza a matriz de covariância do erro da predição.
    state->P[0][0] += dt * (dt * state->P[1][1] - state->P[0][1] - state->P[1][0] + state->Q_angle);
    state->P[0][1] -= dt * state->P[1][1];
    state->P[1][0] -= dt * state->P[1][1];
    state->P[1][1] += state->Q_bias * dt;

    // Etapa de Correção:
    // Calcula o erro (inovação) entre o ângulo medido e o ângulo predito.
    float y = newAngle - state->angle;

    // Calcula a covariância da inovação.
    float S = state->P[0][0] + state->R_measure;

    // Calcula o Ganho de Kalman.
    float K[2];
    K[0] = state->P[0][0] / S;
    K[1] = state->P[1][0] / S;

    // Atualiza a estimativa do ângulo com a medição.
    state->angle += K[0] * y;

    // Atualiza a estimativa do desvio (bias) do giroscópio.
    state->bias += K[1] * y;

    // Atualiza a matriz de covariância do erro.
    float P00_temp = state->P[0][0];
    float P01_temp = state->P[0][1];

    state->P[0][0] -= K[0] * P00_temp;
    state->P[0][1] -= K[0] * P01_temp;
    state->P[1][0] -= K[1] * P00_temp;
    state->P[1][1] -= K[1] * P01_temp;

    return state->angle;
}
