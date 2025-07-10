#ifndef KALMAN_H
#define KALMAN_H

/**
 * @file kalman.h
 * @brief Funções para a implementação de um Filtro de Kalman para fusão de
 * dados de acelerômetro e giroscópio.
 */

// Estrutura que armazena o estado de um filtro de Kalman para um eixo.
typedef struct {
    /* Variáveis do Filtro de Kalman */
    float Q_angle;   // Incerteza do processo para o ângulo
    float Q_bias;    // Incerteza do processo para o desvio (bias) do giroscópio
    float R_measure; // Incerteza da medição

    float angle;     // O ângulo estimado (saída do filtro)
    float bias;      // O desvio (bias) do giroscópio estimado
    float rate;      // A taxa de rotação estimada
    float P[2][2];   // Matriz de covariância do erro
} KalmanState;

/**
 * @brief Inicializa o estado de um Filtro de Kalman.
 * @param state Ponteiro para a estrutura de estado do filtro a ser inicializada.
 * @details Define os valores iniciais para as matrizes e as incertezas do filtro.
 * Deve ser chamada uma vez para cada eixo a ser filtrado.
 */
void kalman_init(KalmanState *state);

/**
 * @brief Executa um passo do Filtro de Kalman para estimar o ângulo.
 * @param state Ponteiro para a estrutura de estado do filtro.
 * @param newAngle Novo ângulo medido (geralmente pelo acelerômetro).
 * @param newRate Nova taxa de rotação medida (geralmente pelo giroscópio).
 * @param dt Intervalo de tempo (delta time) desde a última chamada, em segundos.
 * @return O ângulo filtrado.
 */
float kalman_get_angle(KalmanState *state, float newAngle, float newRate, float dt);

#endif // KALMAN_H
