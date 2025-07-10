#include "kalman.h"
#include <math.h>

/**
 * @file kalman.c
 * @brief Implementa��o do Filtro de Kalman.
 */

void kalman_init(KalmanState *state) {
    // Valores de incerteza do filtro. Estes podem ser ajustados (tuning)
    // para otimizar o desempenho do filtro para uma aplica��o espec�fica.
    state->Q_angle = 0.001f;
    state->Q_bias = 0.003f;
    state->R_measure = 0.03f;

    state->angle = 0.0f; // Inicia o �ngulo em 0
    state->bias = 0.0f;  // Inicia o bias em 0

    // Inicia a matriz de covari�ncia do erro com valores de incerteza elevados
    state->P[0][0] = 0.0f;
    state->P[0][1] = 0.0f;
    state->P[1][0] = 0.0f;
    state->P[1][1] = 0.0f;
}

float kalman_get_angle(KalmanState *state, float newAngle, float newRate, float dt) {
    // Etapa de Predi��o:
    // Estima o �ngulo atual com base na leitura do girosc�pio.
    state->rate = newRate - state->bias;
    state->angle += dt * state->rate;

    // Atualiza a matriz de covari�ncia do erro da predi��o.
    state->P[0][0] += dt * (dt * state->P[1][1] - state->P[0][1] - state->P[1][0] + state->Q_angle);
    state->P[0][1] -= dt * state->P[1][1];
    state->P[1][0] -= dt * state->P[1][1];
    state->P[1][1] += state->Q_bias * dt;

    // Etapa de Corre��o:
    // Calcula o erro (inova��o) entre o �ngulo medido e o �ngulo predito.
    float y = newAngle - state->angle;

    // Calcula a covari�ncia da inova��o.
    float S = state->P[0][0] + state->R_measure;

    // Calcula o Ganho de Kalman.
    float K[2];
    K[0] = state->P[0][0] / S;
    K[1] = state->P[1][0] / S;

    // Atualiza a estimativa do �ngulo com a medi��o.
    state->angle += K[0] * y;

    // Atualiza a estimativa do desvio (bias) do girosc�pio.
    state->bias += K[1] * y;

    // Atualiza a matriz de covari�ncia do erro.
    float P00_temp = state->P[0][0];
    float P01_temp = state->P[0][1];

    state->P[0][0] -= K[0] * P00_temp;
    state->P[0][1] -= K[0] * P01_temp;
    state->P[1][0] -= K[1] * P00_temp;
    state->P[1][1] -= K[1] * P01_temp;

    return state->angle;
}
