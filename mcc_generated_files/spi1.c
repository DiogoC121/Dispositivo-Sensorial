
/**
  SPI1 Generated Driver API Source File

  Company:
    Microchip Technology Inc.

  File Name:
    spi1.c

  @Summary
    This is the generated source file for the SPI1 driver using PIC24 / dsPIC33 / PIC32MM MCUs

  @Description
    This source file provides APIs for driver for SPI1.
    Generation Information :
        Product Revision  :  PIC24 / dsPIC33 / PIC32MM MCUs - 1.171.5
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

#include "spi1.h"

/**
 Section: File specific functions
*/

inline __attribute__((__always_inline__)) SPI1_TRANSFER_MODE SPI1_TransferModeGet(void);
void SPI1_Exchange( uint8_t *pTransmitData, uint8_t *pReceiveData );
uint16_t SPI1_ExchangeBuffer(uint8_t *pTransmitData, uint16_t byteCount, uint8_t *pReceiveData);

/**
 Section: Driver Interface Function Definitions
*/


void SPI1_Initialize (void)
{
    // MSTEN Master; DISSDO disabled; PPRE 1:1; SPRE 4:1; MODE16 disabled; SMP Middle; DISSCK disabled; CKP Idle:Low, Active:High; CKE Idle to Active; 
    SPI1CON1 = 0x33;
    // SPIBEN enabled; SPIFPOL disabled; SPIFE disabled; 
    SPI1CON2 = 0x01;
    // SPITBF disabled; SISEL SPI_INT_SPIRBF; SPIRBF disabled; SPIROV disabled; SPIEN enabled; SRXMPT disabled; SRMPT disabled; SPISIDL disabled; SPIBEC disabled; 
    SPI1STAT = 0x800C;
}
void SPI1_Exchange( uint8_t *pTransmitData, uint8_t *pReceiveData )
{

    while( SPI1STATbits.SPITBF == true )
    {

    }

    SPI1BUF = *((uint8_t*)pTransmitData);

    while ( SPI1STATbits.SRXMPT == true);

    *((uint8_t*)pReceiveData) = SPI1BUF;

}

uint16_t SPI1_ExchangeBuffer(uint8_t *pTransmitData, uint16_t byteCount, uint8_t *pReceiveData)
{

    uint16_t dataSentCount = 0;
    uint16_t dataReceivedCount = 0;
    uint16_t dummyDataReceived = 0;
    uint16_t dummyDataTransmit = SPI1_DUMMY_DATA;

    uint8_t  *pSend, *pReceived;
    uint16_t addressIncrement;
    uint16_t receiveAddressIncrement, sendAddressIncrement;

    addressIncrement = 1;


    // set the pointers and increment delta 
    // for transmit and receive operations
    if (pTransmitData == NULL)
    {
        sendAddressIncrement = 0;
        pSend = (uint8_t*)&dummyDataTransmit;
    }
    else
    {
        sendAddressIncrement = addressIncrement;
        pSend = (uint8_t*)pTransmitData;
    }
        
    if (pReceiveData == NULL)
    {
       receiveAddressIncrement = 0;
       pReceived = (uint8_t*)&dummyDataReceived;
    }
    else
    {
       receiveAddressIncrement = addressIncrement;        
       pReceived = (uint8_t*)pReceiveData;
    }


    while( SPI1STATbits.SPITBF == true )
    {

    }

    while (dataSentCount < byteCount)
    {
        if ( SPI1STATbits.SPITBF != true )
        {
            SPI1BUF = *pSend;

            pSend += sendAddressIncrement;
            dataSentCount++;
        }

        if (SPI1STATbits.SRXMPT == false)
        {

            *pReceived = SPI1BUF;

            pReceived += receiveAddressIncrement;
            dataReceivedCount++;
        }

    }
    while (dataReceivedCount < byteCount)
    {
        if (SPI1STATbits.SRXMPT == false)
        {

            *pReceived = SPI1BUF;

            pReceived += receiveAddressIncrement;
            dataReceivedCount++;
        }
    }

    return dataSentCount;
}

uint8_t SPI1_Exchange8bit( uint8_t data )
{
    uint8_t receiveData;
    
    SPI1_Exchange(&data, &receiveData);

    return (receiveData);
}

uint16_t SPI1_Exchange8bitBuffer(uint8_t *dataTransmitted, uint16_t byteCount, uint8_t *dataReceived)
{
    return (SPI1_ExchangeBuffer(dataTransmitted, byteCount, dataReceived));
}

/**

    The module's transfer mode affects the operation
    of the exchange functions. The table below shows
    the effect on data sent or received:
    |=======================================================================|
    | Transfer Mode  |     Exchange Function      |        Comments         |
    |=======================================================================|
    |                | SPIx_Exchange8bitBuffer()  |                         |
    |                |----------------------------|  OK                     |
    |                | SPIx_Exchange8bit()        |                         |
    |     8 bits     |----------------------------|-------------------------|
    |                | SPIx_Exchange16bitBuffer() | Do not use. Only the    |
    |                |----------------------------| lower byte of the 16-bit|
    |                | SPIx_Exchange16bit()       | data will be sent or    |
    |                |                            | received.               |
    |----------------|----------------------------|-------------------------|
    |                | SPIx_Exchange8bitBuffer()  | Do not use. Additional  |
    |                |----------------------------| data byte will be       |
    |                | SPIx_Exchange8bit()        | inserted for each       |
    |                |                            | 8-bit data.             |
    |     16 bits    |----------------------------|-------------------------|
    |                | SPIx_Exchange16bitBuffer() |                         |
    |                |----------------------------|  OK                     |
    |                | SPIx_Exchange16bit()       |                         |
    |----------------|----------------------------|-------------------------|
*/
inline __attribute__((__always_inline__)) SPI1_TRANSFER_MODE SPI1_TransferModeGet(void)
{
	if (SPI1CON1bits.MODE16 == 0)
        return SPI1_DRIVER_TRANSFER_MODE_8BIT;
    else
        return SPI1_DRIVER_TRANSFER_MODE_16BIT;
}


SPI1_STATUS SPI1_StatusGet()
{
    return(SPI1STAT);
}
/**
 End of File
*/