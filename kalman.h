#ifndef KALMAN_H
#define KALMAN_H

/**
 * @file kalman.h
 * @brief Fun��es para a implementa��o de um Filtro de Kalman para fus�o de
 * dados de aceler�metro e girosc�pio.
 */

// Estrutura que armazena o estado de um filtro de Kalman para um eixo.
typedef struct {
    /* Vari�veis do Filtro de Kalman */
    float Q_angle;   // Incerteza do processo para o �ngulo
    float Q_bias;    // Incerteza do processo para o desvio (bias) do girosc�pio
    float R_measure; // Incerteza da medi��o

    float angle;     // O �ngulo estimado (sa�da do filtro)
    float bias;      // O desvio (bias) do girosc�pio estimado
    float rate;      // A taxa de rota��o estimada
    float P[2][2];   // Matriz de covari�ncia do erro
} KalmanState;

/**
 * @brief Inicializa o estado de um Filtro de Kalman.
 * @param state Ponteiro para a estrutura de estado do filtro a ser inicializada.
 * @details Define os valores iniciais para as matrizes e as incertezas do filtro.
 * Deve ser chamada uma vez para cada eixo a ser filtrado.
 */
void kalman_init(KalmanState *state);

/**
 * @brief Executa um passo do Filtro de Kalman para estimar o �ngulo.
 * @param state Ponteiro para a estrutura de estado do filtro.
 * @param newAngle Novo �ngulo medido (geralmente pelo aceler�metro).
 * @param newRate Nova taxa de rota��o medida (geralmente pelo girosc�pio).
 * @param dt Intervalo de tempo (delta time) desde a �ltima chamada, em segundos.
 * @return O �ngulo filtrado.
 */
float kalman_get_angle(KalmanState *state, float newAngle, float newRate, float dt);

#endif // KALMAN_H
