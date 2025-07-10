#ifndef DEFINES_H
#define DEFINES_H

/*
 * Arquivo de defini��es e constantes de configura��o para o Dispositivo Sensorial.
 * Autor: Diogo Correia da Silva
 */

// Defini��o da frequ�ncia do cristal para as fun��es de delay
#define FCY 4000000UL
#include <libpic30.h>

// --- C�digos de Erro (conforme Figura 83) ---
#define ERRO_1_NRF_SETUP    1   // Falha na inicializa��o do NRF24L01
#define ERRO_2_MPU_SETUP    2   // Falha na inicializa��o do MPU9250
#define ERRO_3_COMUNICACAO  3   // Falha na comunica��o (envio/recebimento)

// --- Comandos Recebidos do Gateway (conforme Figura 81) ---
#define CMD_ENVIAR_DADOS        0x01 // Comando para for�ar envio de dados
#define CMD_RECONFIG_TIMMER     0x02 // Comando para reconfigurar o timer de leitura

// --- Configura��es Gerais da Aplica��o ---
#define BUFFER_SIZE         12      // Tamanho do buffer de transmiss�o em int16_t (total 24 bytes)
#define KALMAN_DT           0.015f  // Intervalo de tempo (dt) para o filtro, baseado no per�odo do Timer1 (15 ms)

#ifndef M_PI
#define M_PI                3.14159265358979323846 // Defini��o de PI para c�lculos matem�ticos
#endif

// --- Configura��es da Comunica��o NRF24L01+ ---
#define NRF_CHANNEL         76      // Canal de RF (0-125)
#define NRF_ADDRESS         {0x52, 0x00, 0xE1, 0xAB, 0x12} // Endere�o de 5 bytes para a comunica��o

// --- Configura��es de Tens�o e ADC ---
#define VIN_MCU             2.5f    // Tens�o de opera��o do circuito (V)
#define VREF_ADC            2.5f    // Tens�o de refer�ncia para o conversor A/D (V)
#define FATOR_DIVISOR_BAT   2.0f    // Fator do divisor de tens�o para medi��o da bateria

// --- Par�metros do Sensor de Temperatura (NTC) ---
#define R_DIVISOR_NTC       10000.0f// Resist�ncia do divisor de tens�o para o NTC (Ohms)
#define BETA_NTC            3950.0f // Coeficiente Beta do termistor
#define T0_NTC              298.15f // Temperatura de refer�ncia em Kelvin (25 �C)
#define R0_NTC              10000.0f// Resist�ncia nominal do termistor em T0 (Ohms)

// --- Par�metros dos Sensores de Deforma��o (Strain Gauge) ---// --- Par�metros dos Sensores de Deforma��o (Strain Gauge) ---
#define VREF_AMP            1.25f   // Tens�o de refer�ncia do amplificador de instrumenta��o (V)
#define GANHO_AMP           25.0f   // Ganho configurado no amplificador de instrumenta��o
#define VEXC_PONTE          2.5f    // Tens�o de excita��o da ponte (V)
#define GF_STRAIN           2.0f    // Fator de medi��o (Gauge Factor) do strain gauge
#define E_MODULUS           200e9f  // M�dulo de Young do material em teste (ex: A�o = 200 GPa)
#define AREA_SECAO          1e-4f   // �rea da se��o transversal do material (ex: 1cm� = 0.0001 m�)


// --- Par�metros de Sensibilidade do MPU9250 (Exemplo para uma configura��o) ---
// Estes valores devem corresponder � configura��o real no driver do MPU9250
#define MPU_ACCEL_SENSITIVITY_4G    8192.0f  // LSB/g para fundo de escala de +/-4g
#define MPU_GYRO_SENSITIVITY_500DPS 65.5f    // LSB/(�/s) para fundo de escala de +/-500�/s

#endif	/* DEFINES_H */
