#ifndef DEFINES_H
#define DEFINES_H

/*
 * Arquivo de definições e constantes de configuração para o Dispositivo Sensorial.
 * Autor: Diogo Correia da Silva
 */

// Definição da frequência do cristal para as funções de delay
#define FCY 4000000UL
#include <libpic30.h>

// --- Códigos de Erro (conforme Figura 83) ---
#define ERRO_1_NRF_SETUP    1   // Falha na inicialização do NRF24L01
#define ERRO_2_MPU_SETUP    2   // Falha na inicialização do MPU9250
#define ERRO_3_COMUNICACAO  3   // Falha na comunicação (envio/recebimento)

// --- Comandos Recebidos do Gateway (conforme Figura 81) ---
#define CMD_ENVIAR_DADOS        0x01 // Comando para forçar envio de dados
#define CMD_RECONFIG_TIMMER     0x02 // Comando para reconfigurar o timer de leitura

// --- Configurações Gerais da Aplicação ---
#define BUFFER_SIZE         19      // Tamanho do buffer de transmissão em int16_t (total 38 bytes)
#define KALMAN_DT           0.015f  // Intervalo de tempo (dt) para o filtro, baseado no período do Timer1 (15 ms)

#ifndef M_PI
#define M_PI                3.14159265358979323846 // Definição de PI para cálculos matemáticos
#endif

// --- Configurações da Comunicação NRF24L01+ ---
#define NRF_CHANNEL         76      // Canal de RF (0-125)
#define NRF_ADDRESS         {0x52, 0x00, 0xE1, 0xAB, 0x12} // Endereço de 5 bytes para a comunicação

// --- Configurações de Tensão e ADC ---
#define VIN_MCU             2.5f    // Tensão de operação do circuito (V)
#define VREF_ADC            2.5f    // Tensão de referência para o conversor A/D (V)
#define FATOR_DIVISOR_BAT   2.0f    // Fator do divisor de tensão para medição da bateria

// --- Parâmetros do Sensor de Temperatura (NTC) ---
#define R_DIVISOR_NTC       10000.0f// Resistência do divisor de tensão para o NTC (Ohms)
#define BETA_NTC            3950.0f // Coeficiente Beta do termistor
#define T0_NTC              298.15f // Temperatura de referência em Kelvin (25 °C)
#define R0_NTC              10000.0f// Resistência nominal do termistor em T0 (Ohms)

// --- Parâmetros dos Sensores de Deformação (Strain Gauge) ---
#define VREF_AMP            1.25f   // Tensão de referência do amplificador de instrumentação (V)
#define GANHO_AMP           25.0f   // Ganho configurado no amplificador de instrumentação
#define VEXC_PONTE          2.5f    // Tensão de excitação da ponte (V)
#define GF_STRAIN           2.0f    // Fator de medição (Gauge Factor) do strain gauge
#define E_MODULUS           200e9f  // Módulo de Young do material em teste (ex: Aço = 200 GPa)
#define AREA_SECAO          1e-4f   // Área da seção transversal do material (ex: 1cm² = 0.0001 m²)

// --- Parâmetros de Sensibilidade do MPU9250 (Exemplo para uma configuração) ---
#define MPU_ACCEL_SENSITIVITY_4G    8192.0f  // LSB/g para fundo de escala de +/-4g
#define MPU_GYRO_SENSITIVITY_500DPS 65.5f    // LSB/(°/s) para fundo de escala de +/-500°/s

#endif	/* DEFINES_H */
