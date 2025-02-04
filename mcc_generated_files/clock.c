/**
  @Generated PIC24 / dsPIC33 / PIC32MM MCUs Source File

  @Company:
    Microchip Technology Inc.

  @File Name:
    clock.c

  @Summary:
    This is the clock.c file generated using PIC24 / dsPIC33 / PIC32MM MCUs

  @Description:
    This header file provides implementations for driver APIs for all modules selected in the GUI.
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

#include <stdint.h>
#include "xc.h"
#include "clock.h"

void CLOCK_Initialize(void)
{
    // RCDIV FRC/2; DOZE 1:16; DOZEN enabled; ROI enabled; 
    CLKDIV = 0xC900;
    // TUN Center frequency; 
    OSCTUN = 0x00;
    // ADC1MD enabled; T3MD disabled; T4MD disabled; T1MD enabled; U2MD disabled; T2MD disabled; U1MD disabled; SPI2MD disabled; SPI1MD enabled; T5MD disabled; I2C1MD disabled; 
    PMD1 = 0xF0F0;
    // OC5MD disabled; IC5MD disabled; IC4MD disabled; IC3MD disabled; OC1MD disabled; IC2MD disabled; OC2MD disabled; IC1MD disabled; OC3MD disabled; OC4MD disabled; 
    PMD2 = 0x1F1F;
    // PMPMD disabled; RTCCMD disabled; CMPMD disabled; CRCPMD disabled; I2C2MD disabled; 
    PMD3 = 0x782;
    // NOSC FRC; SOSCEN disabled; OSWEN Switch is Complete; 
    __builtin_write_OSCCONH((uint8_t) (0x00));
    __builtin_write_OSCCONL((uint8_t) (0x00));
}
