
/**
  EXT_INT Generated Driver File 

  @Company:
    Microchip Technology Inc.

  @File Name:
    ext_int.c

  @Summary
    This is the generated driver implementation file for the EXT_INT 
    driver using PIC24 / dsPIC33 / PIC32MM MCUs

  @Description:
    This source file provides implementations for driver APIs for EXT_INT. 
    Generation Information : 
        Product Revision  :  PIC24 / dsPIC33 / PIC32MM MCUs - 1.171.4
        Device            :  PIC24FJ64GA002
    The generated drivers are tested against the following:
        Compiler          :  XC16 v2.10
        MPLAB             :  MPLAB X v6.05
*/

/*
    (c) 2020 Microchip Technology Inc. and its subsidiaries. You may use this
    software and any derivatives exclusively with Microchip products.

    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
    WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
    PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION
    WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.

    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
    BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
    FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
    ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
    THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.

    MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE
    TERMS.
*/

/**
   Section: Includes
 */

#include "ext_int.h"
//#include "../main.h"
#include "../defines.h"
#include "../mpu9250.h"
#include "../nrf24l01.h"

//***User Area Begin->code: Add External Interrupt handler specific headers 
extern bool flag_timer;          // Flag interrupção timer
extern bool flag_mpu;            // Flag interrupção mpu
extern bool flag_nrf;            // Flag interrupção nrf
extern bool enviar_dados;        // Flag para enviar dados
extern bool dados_recebidos;     // Flag para dados recebidos   
extern int16_t receivedCommand;  // Comando recebido pelo NRF24L01+
extern int16_t accel[3], gyro[3], mag[3], temp;
//***User Area End->code: Add External Interrupt handler specific headers

/**
   Section: External Interrupt Handlers
*/
 /**/
 void __attribute__ ((weak)) EX_INT0_CallBack(void)
{
    // Aqui você pode adicionar o código para tratar a interrupção
    flag_nrf = true;
    tipo_interrupt(2);   // Chama a função para tratar a interrupção do NRF24L01+

    uint8_t status = NRF24L01_CheckStatus();
    if (status & 0x40) {
        ;// TX FIFO vazio
    }
    if (status & 0x20) {
        // Payload recebido
        dados_recebidos = true;
        NRF24L01_ReadPayload(&receivedCommand, 1); 
    }
    if (status & 0x10) {
        // Máximo de retransmissões atingido
        error(3);
    }

    NRF24L01_ClearInterrupts();
}

/*
  Interrupt Handler for EX_INT0 - INT0
*/
void __attribute__ ( ( interrupt, no_auto_psv ) ) _INT0Interrupt(void)
{
    //***User Area Begin->code: INT0 - External Interrupt 0***
	
	EX_INT0_CallBack();
    
	//***User Area End->code: INT0 - External Interrupt 0***
    EX_INT0_InterruptFlagClear();
}
 void __attribute__ ((weak)) EX_INT1_CallBack(void)
{
    // Add your custom callback code here
    flag_mpu = true;
    tipo_interrupt(3);   // Chama a função para tratar a interrupção do MPU9250// Aqui você pode adicionar o código para tratar a interrupção
    MPU9250_ReadData(accel, gyro, mag, &temp);
}

/**
  Interrupt Handler for EX_INT1 - INT1
*/
void __attribute__ ( ( interrupt, no_auto_psv ) ) _INT1Interrupt(void)
{
    //***User Area Begin->code: INT1 - External Interrupt 1***
	
	EX_INT1_CallBack();
    
	//***User Area End->code: INT1 - External Interrupt 1***
    EX_INT1_InterruptFlagClear();
}
/**
    Section: External Interrupt Initializers
 */
/**
    void EXT_INT_Initialize(void)

    Initializer for the following external interrupts
    INT0
    INT1
*/
void EXT_INT_Initialize(void)
{
    /*******
     * INT0
     * Clear the interrupt flag
     * Set the external interrupt edge detect
     * Enable the interrupt, if enabled in the UI. 
     ********/
    EX_INT0_InterruptFlagClear();   
    EX_INT0_PositiveEdgeSet();
    EX_INT0_InterruptEnable();
    /*******
     * INT1
     * Clear the interrupt flag
     * Set the external interrupt edge detect
     * Enable the interrupt, if enabled in the UI. 
     ********/
    EX_INT1_InterruptFlagClear();   
    EX_INT1_PositiveEdgeSet();
    EX_INT1_InterruptEnable();
}
