
/**
  TMR1 Generated Driver API Source File 

  @Company
    Microchip Technology Inc.

  @File Name
    tmr1.c

  @Summary
    This is the generated source file for the TMR1 driver using PIC24 / dsPIC33 / PIC32MM MCUs

  @Description
    This source file provides APIs for driver for TMR1. 
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
  Section: Included Files
*/

#include <stdio.h>
#include "tmr1.h"
//#include "../main.h"
#include "../defines.h"
#include "../mpu9250.h"
#include "../nrf24l01.h" 

extern bool flag_timer;          // Flag interrupção timer
extern bool flag_mpu;            // Flag interrupção mpu
extern bool flag_nrf;            // Flag interrupção nrf
extern bool enviar_dados;        // Flag para enviar dados
extern bool dados_recebidos;     // Flag para dados recebidos   
extern float temperatura;           // Temperatura em °C
extern float tensao_bateria;        // Tensão da bateria em V
extern float strain_gauge_x;        // Leitura do strain gauge X
extern float strain_gauge_y;        // Leitura do strain gauge Y
extern float strain_gauge_z;        // Leitura do strain gauge Z

/**
 Section: File specific functions
*/
void (*TMR1_InterruptHandler)(void) = NULL;
void TMR1_CallBack(void);

/**
  Section: Data Type Definitions
*/

/** TMR Driver Hardware Instance Object

  @Summary
    Defines the object required for the maintenance of the hardware instance.

  @Description
    This defines the object required for the maintenance of the hardware
    instance. This object exists once per hardware instance of the peripheral.

  Remarks:
    None.
*/

typedef struct _TMR_OBJ_STRUCT
{
    /* Timer Elapsed */
    volatile bool           timerElapsed;
    /*Software Counter value*/
    volatile uint8_t        count;

} TMR_OBJ;

static TMR_OBJ tmr1_obj;

/**
  Section: Driver Interface
*/

void TMR1_Initialize (void)
{
    //TMR1 0; 
    TMR1 = 0x00;
    //Period = 0.25 s; Frequency = 4000000 Hz; PR1 15625; 
    PR1 = 0x3D08;
    //TCKPS 1:64; TON enabled; TSIDL disabled; TCS FOSC/2; TSYNC disabled; TGATE disabled; 
    T1CON = 0x8020; // 0x8020 = 1000 0000 0010 0000 (TON=1, TCKPS=10)
    
    if(TMR1_InterruptHandler == NULL)
    {
        TMR1_SetInterruptHandler(&TMR1_CallBack);
    }

    IFS0bits.T1IF = false;
    IEC0bits.T1IE = true;
	
    tmr1_obj.timerElapsed = false;

}


void __attribute__ ( ( interrupt, no_auto_psv ) ) _T1Interrupt (  )
{
    /* Check if the Timer Interrupt/Status is set */

    //***User Area Begin
    static volatile unsigned int CountCallBack = 0;

    // callback function - called every 15th pass
    if (++CountCallBack >= TMR1_INTERRUPT_TICKER_FACTOR)
    {
        // ticker function call
        if(TMR1_InterruptHandler) 
        { 
               TMR1_InterruptHandler(); 
        }

        // reset ticker counter
        CountCallBack = 0;
    }

    //***User Area End

    tmr1_obj.count++;
    tmr1_obj.timerElapsed = true;
    IFS0bits.T1IF = false;
}

void TMR1_Period16BitSet( uint16_t value )
{
    /* Update the counter values */
    PR1 = value;
    /* Reset the status information */
    tmr1_obj.timerElapsed = false;
}

uint16_t TMR1_Period16BitGet( void )
{
    return( PR1 );
}

void TMR1_Counter16BitSet ( uint16_t value )
{
    /* Update the counter values */
    TMR1 = value;
    /* Reset the status information */
    tmr1_obj.timerElapsed = false;
}

uint16_t TMR1_Counter16BitGet( void )
{
    return( TMR1 );
}


void __attribute__ ((weak)) TMR1_CallBack(void)
{
    TMR1_Stop();
    tipo_interrupt(1); // Chama a função para tratar a interrupção do Timer1
    flag_timer = true;
    enviar_dados = true;

    // Liga o ENABLE 50 ms antes da leitura
    LATAbits.LATA4 = 1;
    __delay_ms(50);

    // Realiza as leituras dos ADCs
    temperatura = NTC_To_Temperature(getADC(0));          // Canal AN0 (TEMP)
    tensao_bateria = (getADC(1) / 1023.0) * 3.3;          // Canal AN1 (SBAT)
    strain_gauge_x = (getADC(2) / 1023.0) * 3.3;          // Canal AN2 (SGX)
    strain_gauge_y = (getADC(3) / 1023.0) * 3.3;          // Canal AN3 (SGY)
    strain_gauge_z = (getADC(4) / 1023.0) * 3.3;          // Canal AN4 (SGZ)

    // Desliga o ENABLE 50 ms após as leituras
    __delay_ms(50);
    LATAbits.LATA4 = 0;
}

void  TMR1_SetInterruptHandler(void (* InterruptHandler)(void))
{ 
    IEC0bits.T1IE = false;
    TMR1_InterruptHandler = InterruptHandler; 
    IEC0bits.T1IE = true;
}

void TMR1_Start( void )
{
    /* Reset the status information */
    tmr1_obj.timerElapsed = false;

    /*Enable the interrupt*/
    IEC0bits.T1IE = true;

    /* Start the Timer */
    T1CONbits.TON = 1;
}

void TMR1_Stop( void )
{
    /* Stop the Timer */
    T1CONbits.TON = false;

    /*Disable the interrupt*/
    IEC0bits.T1IE = false;
}

bool TMR1_GetElapsedThenClear(void)
{
    bool status;
    
    status = tmr1_obj.timerElapsed;

    if(status == true)
    {
        tmr1_obj.timerElapsed = false;
    }
    return status;
}

int TMR1_SoftwareCounterGet(void)
{
    return tmr1_obj.count;
}

void TMR1_SoftwareCounterClear(void)
{
    tmr1_obj.count = 0; 
}

/**
 End of File
*/
